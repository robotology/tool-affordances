
#include "featExt.h"
#include <cv.h>
//#include "opencv2/opencv.hpp"

#include "contourFeats.h"

#include <fstream>
#include <iostream>

using namespace yarp::sig;
using namespace yarp::os;
using namespace cv;
using namespace std;

void FeatExt::loop()
{
	Bottle cmd, val, reply;
	rpcCmd.read(cmd, true);
	int rxCmd=processRpcCmd(cmd,val);
	if (rxCmd==Vocab::encode("fext")){

		ImageOf<PixelRgb> *imageIn = inImPort.read();  // read an image
		//ImageOf<PixelRgb> *binImIn = binImPort.read(false);  // read an image

		if(imageIn == NULL)
		{
			printf("No input image \n"); 
			return;
		}
		//printf("Image loaded \n");
		featExtractor(*imageIn);
	}
}

bool FeatExt::open()
{
    bool ret=true;
    ret = rpcCmd.open("/featExt/rpc:i");					// Port for receiving rpc commands
	ret = ret && inImPort.open("/featExt/img:i");			// give the port a name
    ret = ret && binImPort.open("/featExt/binImg:i");		//port to receive info about the blobs in the image.
	ret = ret && imPropOutPort.open("/featExt/imgProp:o");
	ret = ret && imFeatOutPort.open("/featExt/imgFeat:o");
	ret = ret && featPort.open("/featExt/feats:o");			//port to receive info confirming reception of template.

	
    return ret;
}

bool FeatExt::close()
{
	
	printf("Closing");

	rpcCmd.close();
    
	featPort.setStrict();
	featPort.write();
	

    inImPort.close();
	imPropOutPort.close();
	imFeatOutPort.close();
	binImPort.close();
    featPort.close();
	printf("Closed");
    return true;
}

bool FeatExt::interrupt()
{
	inImPort.interrupt();
	binImPort.interrupt();
	imFeatOutPort.interrupt();
	imPropOutPort.interrupt();


    return true;
}

int FeatExt::processRpcCmd(const Bottle &cmd, Bottle &b)
{
    int ret=Vocab::encode(cmd.get(0).asString().c_str());
    b.clear();
    if (cmd.size()>1)
    {
        if (cmd.get(1).isList())
            b=*cmd.get(1).asList();
        else
            b=cmd.tail();
    }
    return ret;
}

void FeatExt::featExtractor(const ImageOf<PixelRgb>& imageIn)
{
	//ofstream featFile;
	//featFile.open ("features.txt");
	
	//prepare ports
	VecVec &featSend= featPort.prepare();						// Prepare port to send features	
	ImageOf<PixelRgb> &imPropOut = imPropOutPort.prepare();	// Prepare the port for propagated output image 
	ImageOf<PixelRgb> &imFeatOut = imFeatOutPort.prepare();	// Prepare the port for features output image	

	imFeatOut.resize(imageIn.width(), imageIn.height());	// Initialize features image
	imFeatOut.zero();
	Mat contoursIm = cvarrToMat(imFeatOut.getIplImage(),false);
	//Mat contoursIm((IplImage*) imFeatOut.getIplImage());	// Create an image to draw the visual features
	imPropOut = imageIn;									// Set the pointer to the original image which will be modified
	Mat src = cvarrToMat(imPropOut.getIplImage(),false);
	//Mat src((IplImage*) imPropOut.getIplImage());				// Create the src image form the received frame		
	
	//Prepare image variable
	RNG rng(12345);
		
	// Find contours out of the received blobs
	//printf("Finding Contours \n");
	int cannyThresh = 100;
	int minBlobSize = 10;
	Mat edges;
	vector<Contour > contours;
	vector<Vec4i> hierarchy;
	vector<vector<Point > > contoursPoint;
	cvtColor(src, edges, CV_BGR2GRAY);								// Create greyscale image for further analysis
	GaussianBlur(edges, edges, Size(3,3), 1.5, 1.5);				// Blur to smooth contours	
	Canny( edges, edges, cannyThresh, cannyThresh*3, 3 );			// Detect edges using canny
	findContours( edges, contoursPoint, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	//printf(" Found %d contours\n", contoursPoint.size() );

	// draw it on offset and thick line to close possible apertures
	drawContours( contoursIm, contoursPoint, -1, Scalar(128), 2, 8); 
	// Recomputing the contour needed to close possible 
	findContours( edges, contoursPoint, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	drawContours( edges, contoursPoint, -1, Scalar(255), 1 , 8); 


	//Mat contoursIm = Mat::zeros( edges.size(), CV_8UC3 );
	
	for( int i = 0; i< contoursPoint.size(); i++ )					// Iterate through each contour. 
	{
		double a=contourArea(contoursPoint[i]);						// Find the area of contour
		if(a>minBlobSize){											// Keep only big contours
			contours.push_back(Contour(contoursPoint[i]));			// Generate a vector of Contour objects
			drawContours( contoursIm, contoursPoint, i, Scalar(255,0,0), 2, 8, hierarchy, 0, Point() );
		}
	}
	printf(" Found %d large contours out of %d contours \n", contours.size(), contoursPoint.size() );
	//featFile << " Found " << contours.size() << " large contours out of " << contoursPoint.size() << " contours. \n";
	
	// Initializationg vector of features
	vector<Feature> feats (contours.size() );

	// ==== Mutual similarities ====
	Mat similarityMatrix = Mat::zeros(contours.size(),contours.size(), CV_32F);
	for( int i = 0; i < contours.size(); i++ ){
		char cntNum[10]; sprintf_s(cntNum,"Object %d",i);
		feats[i].name.assign(cntNum);
		for( int j = 0; j < contours.size(); j++ ){
			double similarity = matchShapes(contours[i].getPoints(),contours[j].getPoints(),CV_CONTOURS_MATCH_I3,0);
			similarityMatrix.at<float>(i,j) = similarity;
			feats[i].content.push_back(similarity);
		}
	}
	//featFile << "Similarity Matrix \n";
	//featFile << "M = "<< endl << " "  << similarityMatrix << endl << endl;		
	//printf("Similarity Matrix:");
	//cout << "M = "<< endl << " "  << similarityMatrix << endl << endl;

	// ++++++++++ LOOP THROUGH CONTOURS +++++++++++++
	for( int i = 0; i < contours.size(); i++ )
	{
		//printf(" * Contour[%d] \n", i);
		//featFile << "\n CONTOUR " << i << "\n";
		Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );	
		Rect boundRect = boundingRect( contours[i].getPoints());

		//==== Convex Hull and Convexity Defects ====
		vector<Vec4i> convDefVec;					// Convexity defects
		vector<double> defsDepth;					// Depth of convexity defects points
		vector<int> defsIndx;						// Index of Convexity defect points
		vector<double> defsDirs;					// Bisector of convexity defect
		
		contours[i].convDefs(convDefVec);
		contours[i].convDefPos(convDefVec, defsDepth,defsIndx);
		contours[i].convDir(defsIndx, defsDirs);
		for ( int c = 0; c < defsDepth.size(); c++ ){
			feats[i].content.push_back(defsDepth[c]);
			feats[i].content.push_back(defsDirs[c]);
			circle(contoursIm, contours[i].getPoints()[defsIndx[c]], 5, Scalar(0, 255, 0), -1, 8, 0 );
		}

		// ==== Skeleton and joints ====
		vector<Point> jntPoints;					// Skeleton Joint Points
		vector<Point> endPoints;					// Skeleton End Points
		contours[i].jointPoints(jntPoints, endPoints);
		Point mc = contours[i].massCenter(true);
		for(int m=0; m < jntPoints.size(); m++) {
			feats[i].content.push_back(jntPoints[m].x-mc.x);	//Normalize the points to the blobs center of mass
			feats[i].content.push_back(jntPoints[m].y-mc.y);
			circle(contoursIm, jntPoints[m]+boundRect.tl(), 4, Scalar(0, 255, 0), 1, 8, 0 );

		}
		for(int n=0; n < endPoints.size(); n++) {
			feats[i].content.push_back(endPoints[n].x-mc.x);
			feats[i].content.push_back(endPoints[n].y-mc.y);
			circle(contoursIm, endPoints[n]+boundRect.tl(), 4, Scalar(255, 0, 0), 1, 8, 0 );
		}


	
		// ==== Polygon approximation + Bounding rects and circles ====
				
		// ====  Moments ====
		feats[i].content.push_back(contours[i].nu02());		// Moment nu02
		feats[i].content.push_back(contours[i].nu20());		// Moment nu20
		feats[i].content.push_back(contours[i].nu11());		// Moment nu11

		// ==== Compute Shape descriptors ====
		// Original Shape
		feats[i].content.push_back(contours[i].area());				// Contour area
		feats[i].content.push_back(contours[i].perimeter());		// Contour perimeter
		feats[i].content.push_back(contours[i].compactness());		// Shape compactness
		feats[i].content.push_back(contours[i].length());			// Length
		feats[i].content.push_back(contours[i].width());			// Width
		feats[i].content.push_back(contours[i].aspectRatio());		// Contour aspectRatio
		feats[i].content.push_back(contours[i].xtRight());			// Extension of the object to the right w.r.t the mass center
		feats[i].content.push_back(contours[i].xtLeft());			// Extension of the object to the left w.r.t the mass center
		feats[i].content.push_back(contours[i].elongation());		// Elongation
		feats[i].content.push_back(contours[i].rectangularity());	// Rectangularity
		//Convex Hull Area
		feats[i].content.push_back(contours[i].convHullArea());		// Convex Hull area
		feats[i].content.push_back(contours[i].solidity());			// Solidity
		// From centroid distance signature
		contours[i].normalize();
		feats[i].content.push_back(contours[i].contourNorm->bendEnergy());	// Bending Energy
		

		// ==== Transformed Domain Features from Signature ====		
		vector<double> fourierDesc;											// Fourier descriptors
		vector<double> waveletDesc;											// Wavelet descriptors
		int P = 15;		//Number of FD to save
		contours[i].getDFT(fourierDesc);
		contours[i].getWavelet(waveletDesc);

		for ( int p = 0; p <P; p++ ){
			feats[i].content.push_back(fourierDesc[p]);
			feats[i].content.push_back(waveletDesc[p]);
		}


		// Draw results on images
		contours[i].drawOnImg(src, color);
		contours[i].convHull->drawOnImg(contoursIm, color);
		contours[i].contourNorm->drawOnImg(contoursIm,color);

		char strI[4]; sprintf_s(strI,"%d",i);
		contours[i].drawText(contoursIm, strI , color);	

		featSend.content.push_back(feats[i]);
	} //end contours loop
	

	printf("\n DONE \n");
	
	featPort.write();
	imPropOutPort.write();
	imFeatOutPort.write();
}


