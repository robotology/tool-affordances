/*
 * 2D GEOMETRIC FEATURE EXTRACTION
 * Copyright (C) 2014 iCub Facility - Istituto Italiano di Tecnologia
 * Author: Tanis Mar
 * email: tanis.mar@iit.it
 * Permission is granted to copy, distribute, and/or modify this program
 * under the terms of the GNU General Public License, version 2 or any
 * later version published by the Free Software Foundation.
 *
 * A copy of the license can be found at
 * http://www.robotcub.org/icub/license/gpl.txt
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details
*/

/** 
\defgroup feat2DExt 2D geometric feature extraction module.

Feature Extraction Module: Extracts a set of 75 geometrical and contour features from the biggest blob in the image.
 

\section intro_sec Description 
This modules receives an image, either binary or not, extracts the blobs and performs feature Extraction 
of the biggest blob found. Features can be ordered in the following categories: 
Convexity, Skeleton, Moments, Shape, Angle Signature, Fourier and Wavelet.
By default is idle until a particular process order is received (snapshot or click), but can be set to process all incoming images with 'go'. 
 
\section lib_sec Libraries 
- YARP libraries. 
- openCV libraries

\section parameters_sec Parameters 
No inline parameters
  
\section portsa_sec Ports Accessed
Assumes an input image port (straming or not) connected to /feat2DExt/img:i

\section portsc_sec Ports Created 
- \e /feat2DExt/rpc:i receives the information to execute the different possible tasks as a Bottle.
It manages the following commands through respond interface:

    -# <b>go</b>: Enables feature extraction on streaming images. Once this option is activated, it won't accept further rpc commands. \n
     
    -# <b>label</b>  <i>(string) label</i> (default = 'undef'): sets the label that will be given to subsequently extracted feature vectors to string 'label'. \n
        
    -# <b>setROI</b> <i>(int) tl.x (int) tl.y (int) br.x (int) br.y</i>: Sets a region of interest for feature extraction from the coordinates given as pixel positions. \n

    -# <b>refAngle</b>  <i>(int) angle</i>: Sets the refernece angle for prespective nomalization of the blob to 'angle'. \n 
        
    -# <b>snapshot</b>: Performs feature extraction on a single frame. \n
    
    -# <b>click</b>: "Asks user to click on viewer and performs feature extraction on the selected blob on one frame.\n

    -# <b>verbose</b> <i>(string) ON/OFF</i> : ON/OFF sets printfs of the extraction process on or off, respectively.

    -# <b>reset</b>: Resets ROI and all other set values to default. \n

    -# <b>help </b>: Produces help on rpc commands. \n
    
- \e /feat2DExt/img:i             Port receiving incoming images to be processed
- \e /feat2DExt/coords:i          Port to receive coordinates to specify blob
- \e /feat2DExt/imgProp:o         Port propagating the frame on which the feature extraction is peformed, with the selected blob marked on top
- \e /feat2DExt/imgFeat:o         Port outputting an image with a visual rpresentation of some of the features extracted, for assessment
- \e /feat2DExt/feats:o			Port which outputs the vector containing all the extracted features
 
\section tested_os_sec Tested OS
Windows, Linux

\author Tanis Mar
*/ 

#include "featExt.h"
#include "contourFeats.h"

using namespace yarp::sig;
using namespace yarp::os;
using namespace cv;
using namespace std;

void FeatExt::loop()
{
    if (processing){        		
        ImageOf<PixelRgb> *imageIn = inImPort.read(false);  // read an image
		if(imageIn == NULL)		{
            printf("No input image \n");
			return;
		}		
        VecVec feats;
        printf("Calling Extractor \n"); 
		featExtractor(*imageIn, feats);
        return;
	}
    else            //accept commands if the module is idle
    {
        
	    Bottle cmd, val, reply;
	    rpcCmd.read(cmd, true);
	    int rxCmd=processRpcCmd(cmd,val);

        if (rxCmd==Vocab::encode("setROI")){                    // sets Region of Interest for Feature Extraction
            Point tl,br;
			tl.x=(int)val.get(0).asInt();
            printf("Setting tl x to = %d \n", tl.x);
			tl.y=(int)val.get(1).asInt();
            printf("Setting tl y to = %d \n", tl.y);
			br.x=(int)val.get(2).asInt();
            printf("Setting br x to = %d \n", br.x);
			br.y=(int)val.get(3).asInt();
            printf("Setting br y to = %d \n", br.y);

            //ImageOf<PixelRgb> *imageIn = inImPort.read();  // read an image
            imWidth = 320; //imageIn->width();
            imHeight = 240; //imageIn->height();

            if (tl.x < 0) {tl.x = 0; printf("ROI tl x smaller than 0, set to 0 \n");}
            if (tl.y < 0) {tl.y = 0; printf("ROI tl y smaller than 0, set to 0 \n");}
            if (br.x > imWidth) {br.x = imWidth; printf("ROI br x outside boundaries. Max = %d \n", imWidth);}
            if (br.y > imHeight) {br.y = imHeight; printf("ROI br y outside boundaries. Max = %d \n", imHeight);}
            
			ROI = Rect(tl,br);
            ROIinit = true;

            reply.addString("ack");
            reply.addString(" ROI set to given values");
            rpcCmd.reply(reply);
            return;

        } else if (rxCmd==Vocab::encode("refAngle")){               // sets vector feature label
            Point tl,br;
			refAngle = (int)val.get(0).asInt();
            printf("reference Angle set to %i \n", refAngle);

            reply.addString("ack");
            reply.addString("reference angle set to given value");
            rpcCmd.reply(reply);
            return;

        } else if (rxCmd==Vocab::encode("label")){                  // sets vector feature label
		    objName = val.get(0).asString();
            return;

        } else if (rxCmd==Vocab::encode("snapshot")){               // process one signle frame and return feats in rpc reply
		    ImageOf<PixelRgb> *imageIn = inImPort.read();  
		    if(imageIn == NULL)		    {
			    printf("No input image \n"); 
                reply.addString("nack");
			    return;
		    }		    
            VecVec feats;
		    featExtractor(*imageIn, feats);
            rpcCmd.reply(feats);
            return;

        } else if (rxCmd==Vocab::encode("click")){                  //asks for blob coordinates and returns feats in rpc reply
		    
            printf("Click on the image \n"); 
            Bottle *coordsIn = coordsInPort.read();       // read coordinates            
            coords.x = coordsIn->get(0).asInt();
            coords.y = coordsIn->get(1).asInt();
            printf("Selected coords are %i, %i \n", coords.x, coords.y); 
            coordsInit = true;
            ImageOf<PixelRgb> *imageIn = inImPort.read();
		    if(imageIn == NULL)		    {
			    printf("No input image \n"); 
                reply.addString("nack");
			    return;
		    }		    
            VecVec feats;
		    featExtractor(*imageIn, feats);
            rpcCmd.reply(feats);
            coordsInit = false;
            return;


        } else if (rxCmd==Vocab::encode("verbose")){            // sets verbose on or off
            string verb = val.get(0).asString();
            if (verb == "ON"){
                verbose = true;
                fprintf(stdout,"Verbose is : %s\n", verb.c_str());
                reply.addString("ack");
                rpcCmd.reply(reply);
                return;
            } else if (verb == "OFF"){
                verbose = false;
                fprintf(stdout,"Verbose is : %s\n", verb.c_str());
                reply.addString("ack");
                rpcCmd.reply(reply);
                return;
            }   
            fprintf(stdout,"Verbose can only be set to ON or OFF. \n");
            reply.addString("nack");
            rpcCmd.reply(reply);
			return;

        } else if (rxCmd==Vocab::encode("reset")){        
            processing = false;
            coordsInit = false;
            ROI = Rect(0,0, imWidth, imHeight);
            ROIinit = false;
            reply.addString("ack");
            rpcCmd.reply(reply);
			return;

	    } else if (rxCmd==Vocab::encode("go")){        
            processing = true;
            reply.addString("ack");
            rpcCmd.reply(reply);
			return;

	    /*} else if (rxCmd==Vocab::encode("stop")){        
            processing = false;
            reply.addString("ack");
            rpcCmd.reply(reply);
			return;
        */
        } else if (rxCmd==Vocab::encode("help")){ 
            reply.addVocab(Vocab::encode("many"));
            reply.addString("Available commands are: \n");
            reply.addString("setROI (int) (int) (int) (int) - sets a region of interest of feature extraction");
            reply.addString("refAngle (int) - sets the refernece angle for prespective nomalization of the blob.");
            reply.addString("label (string) - sets the label that will be given to subsequently extracted feature vectors.");
            reply.addString("snapshot - Performs feature extracton on a single frame.");
            reply.addString("click - Ask user to click on viewer and performs feature extraction on the selected blob.");
            reply.addString("verbose - ON/OFF Sets printfs of the extraction process on or off.");
            reply.addString("reset - Resets ROI and all other set values.");
            reply.addString("go - Enables feature extraction on streaming images. Once this option is activated, it won't accept further rpc commands");
            //reply.addString("stop - Disables feature extraction on streaming images.");
            reply.addString("help - Produces this help.");
            //reply.addString("quit -Quits the module. \n");
            rpcCmd.reply(reply);
			return;
        } else {
            reply.addString("command not recognized, try 'help': \n");
            return;
        }    
    }

}

bool FeatExt::open()
{
    bool ret=true;
    ret = rpcCmd.open("/feat2DExt/rpc:i");					// Port for receiving rpc commands
    ret = ret && inImPort.open("/feat2DExt/img:i");			// Port receiving incoming images to be processed
    ret = ret && coordsInPort.open("/feat2DExt/coords:i");	// Port to receive coordinates to specify blob on 'click'
    ret = ret && imPropOutPort.open("/feat2DExt/imgProp:o");  // Port propagating the frame on which the feature extraction is peformed
    ret = ret && imFeatOutPort.open("/feat2DExt/imgFeat:o");  // Port outputting an image with a visual rpresentation of some of the features extracted, for assessment
    ret = ret && featPort.open("/feat2DExt/feats:o");			// Port which outputs the vector containing all the extracted features

    if (!ret){
        printf("Problems opening ports\n");
        return false;
    }


	processing = false;
    ROIinit = false;
    coordsInit = false;
    verbose = false;
    refAngle = 0.0;
    objName = "0";
    return ret;
}

bool FeatExt::close()
{
	
	printf("Closing");

	rpcCmd.close();
    

	featPort.close();

    inImPort.close();
    coordsInPort.close();
	imPropOutPort.close();
	imFeatOutPort.close();

	printf("Closed");
    return true;
}

bool FeatExt::interrupt()
{
    rpcCmd.interrupt();
    featPort.interrupt();
	inImPort.interrupt();
    coordsInPort.interrupt();
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

void FeatExt::featExtractor(const ImageOf<PixelRgb>& imageIn, VecVec& featSend)
{
	//ofstream featFile;
	//featFile.open ("features.txt");
	
    mutex.wait();

    Scalar red = Scalar(255,0,0);
    Scalar green = Scalar(0,255,0);
    Scalar blue = Scalar(0,0,255);
    Scalar white = Scalar(255,255,255);

	//prepare ports
	ImageOf<PixelRgb> &imPropOut = imPropOutPort.prepare();	// Prepare the port for propagated output image 
	ImageOf<PixelRgb> &imFeatOut = imFeatOutPort.prepare();	// Prepare the port for features output image	

    // Set the pointer to the original image which will be modified
	imPropOut = imageIn;									
	Mat src = cvarrToMat(imPropOut.getIplImage(),false);    // Create the src image form the received frame	

    // Create an image to draw the visual features
    imFeatOut.resize(imageIn.width(), imageIn.height());	
	imFeatOut.zero();
	Mat contoursIm = cvarrToMat(imFeatOut.getIplImage(),false);

    if (ROIinit){
        src = src(ROI);
    }

    //Prepare random generator
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
	findContours( edges, contoursPoint, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0) );
	//printf(" Found %d contours\n", contoursPoint.size() );

	// draw it on offset and thick line to close possible apertures
	//drawContours( contoursIm, contoursPoint, -1, Scalar(0, 0, 255), 2, 8); 
    drawContours( edges, contoursPoint, -1, white, 2, 8); //
	// Recomputing the contour needed to close possible open contours
	findContours( edges, contoursPoint, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0));
	drawContours( contoursIm, contoursPoint, -1, white, 1 , 8); // Paint all found contours in thin white
    	
    int biggerBlobSize = 0;
    int biggerBlobIdx = 0;
	for( int i = 0; i< contoursPoint.size(); i++ )					// Iterate through each contour. 
	{
        if (coordsInit){
           double in = pointPolygonTest( contoursPoint[i], coords, false );
           if (in>=0){
               contours.push_back(Contour(contoursPoint[i]));			// Include only the contour that contains the coords, i.e. the one we clicked on.
               circle(contoursIm, coords, 2, Scalar(255, 0 ,0),-1);
               drawContours( contoursIm, contoursPoint, i, red, 2, 8, hierarchy, 0, Point() );    // paint the seleted contour in thick red
           }
        } else{            
		    double a=contourArea(contoursPoint[i]);						// Find the area of contour
		    //if(a>minBlobSize){											// Keep only big contours
			//    contours.push_back(Contour(contoursPoint[i]));			// Generate a vector of Contour objects
			//    drawContours( contoursIm, contoursPoint, i, blue, 2, 8, hierarchy, 0, Point() ); //Paint all big contours in blue
            //}
            if(a > biggerBlobSize){                                          // Keep only the biggest contour
                biggerBlobSize = a;
                biggerBlobIdx = i;
            }
        }
	}
    contours.push_back(Contour(contoursPoint[biggerBlobIdx]));          // Generate a vector of Contour objects
    drawContours( contoursIm, contoursPoint, biggerBlobIdx, blue, 2, 8, hierarchy, 0, Point() ); //Paint all big contours in blue
	cout << "Largest contour on blob " << biggerBlobIdx << " out of "<< contoursPoint.size() << " blobs." << endl;
	// Initializationg vector of features
	vector<Feature> feats (contours.size() );

    // ++++++++++ LOOP THROUGH CONTOURS +++++++++++++
	for( int i = 0; i < contours.size(); i++ )
	{
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
		printf(" * Contour[%i] \n", i);
		//featFile << "\n CONTOUR " << i << "\n";
		feats[i].name.assign(objName);
		feats[i].content.push_back(' ');		// Include a space after opening parenthesis to make reading easier
		Rect boundRect = boundingRect( contours[i].getPoints());

		//==== Convex Hull and Convexity Defects ====
        int convDepthNum = 5;                       // Number of values of depth in convexity defects to keep.
		vector<Vec4i> convDefVec;					// Convexity defects
		vector<double> defsDepth;                   // Depth of convexity defects points. 
		vector<int> defsIndx;						// Index of Convexity defect points
		vector<double> defsDirs;					// Bisector angle of convexity defect
		vector<double> defsDirsHist;	    		// Histogram of bisector angles

        // compute the convexity defects, their depth and their bisector angles 
		contours[i].convDefs(convDefVec);
		contours[i].convDefPos(convDefVec, defsDepth, defsIndx);
        contours[i].convDir(defsIndx, defsDirs);

        while (defsDepth.size()<convDepthNum) {      // Pad defsDepth vector with 0s up to size convDepthNum
            defsDepth.push_back(0.0);
        }  

        // Push in the convexity defects depths features
		for ( int c = 0; c < convDepthNum; c++ ){
			feats[i].content.push_back(defsDepth[c]);			
            if (verbose) {printf("Depth conv point %i = %g \n", c, defsDepth[c]);}
            if (c < defsIndx.size()){
                circle(contoursIm, contours[i].getPoints()[defsIndx[c]], 3, green, -1, 8, 0 );
                contours[i].drawArrow(contoursIm,contours[i].getPoints()[defsIndx[c]], defsDirs[c], red, 15);
            }
		}

        // Compute and push the histogram of the bisector angles at the convexity defects
        contours[i].convDirHist(defsDirs, defsDirsHist);
        for ( int h = 0; h < defsDirsHist.size(); h++ ){
            feats[i].content.push_back(defsDirsHist[h]);
            if (verbose) {printf(" Conv Def Histogram bin  %i = %g \n", h, defsDirsHist[h]); }
        }
        
		// ==== Skeleton and joints ====
		vector<Point> jntPoints;					// Skeleton Joint Points
		vector<Point> endPoints;					// Skeleton End Points
        double jntLeft = 0.0, jntRight = 0.0, jntBelow = 0.0, jntOver = 0.0;   // # of joints left/right/over/below the center of mass, respectively
        double endLeft = 0.0, endRight = 0.0, endBelow = 0.0, endOver = 0.0;    // # of end points left/right/over/below the center of mass, respectively

		contours[i].jointPoints(jntPoints, endPoints);
		Point mc = contours[i].massCenter(true);
        for(int m=0; m < jntPoints.size(); m++) { // Compare position of joints wrt center of mass
            if (verbose){printf(" Jnt Point [%i, %i] \n", jntPoints[m].x,  jntPoints[m].y);}
            if (jntPoints[m].x-mc.x <= 0 ) {jntLeft += 1;} else {jntRight += 1;}
            if (jntPoints[m].y-mc.y <= 0 ) {jntBelow += 1;} else {jntOver += 1;}
			circle(contoursIm, jntPoints[m]+boundRect.tl(), 4, white, 2, 8, 0 );
		}
		feats[i].content.push_back(jntLeft);	
        feats[i].content.push_back(jntRight);	
        feats[i].content.push_back(jntBelow);	
        feats[i].content.push_back(jntOver);
        
        for(int n=0; n < endPoints.size(); n++) { // Compare position of endPoints wrt center of mass
            if (verbose){printf(" End Point [%i, %i] \n", endPoints[n].x,  endPoints[n].y);}
            if (endPoints[n].x-mc.x <= 0 ) {endLeft += 1;} else {endRight += 1;}
            if (endPoints[n].y-mc.y <= 0 ) {endBelow += 1;} else {endOver += 1;}
			circle(contoursIm, endPoints[n]+boundRect.tl(), 4, white, 2, 8, 0 );
		}
		feats[i].content.push_back(endLeft);	
        feats[i].content.push_back(endRight);	
        feats[i].content.push_back(endBelow);	
        feats[i].content.push_back(endOver);	
        	
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
		
        // From angle signature
        vector<double> angSigHist;	                                  		// Histogram contour angles
		    //contours[i].normalize();                                          // Compute histogram from same number of total points
        feats[i].content.push_back(contours[i].bendEnergy());	            // Bending Energy
            //contours[i].contourNorm->getAngleSigHist(angSigHist);	            // angle histogram Energy
        contours[i].getAngleSigHist(angSigHist);	                        // Angle histogram 
        for ( int h = 0; h < angSigHist.size(); h++ ){
           feats[i].content.push_back(angSigHist[h]);
           if (verbose) {printf(" Norm Angle Histogram bin  %i = %g \n", h, angSigHist[h]); }
        }	

        vector<double> angSig = contours[i].getAngleSig();
        for ( int d = 0; d < angSig.size(); d+=10 ){
            contours[i].drawArrow(contoursIm,contours[i].getPoints()[d], angSig[d], green, 10);
        }


		// ==== Transformed Domain Features from Signature ====		
		vector<double> fourierDesc;											// Fourier descriptors
		vector<double> waveletDesc;											// Wavelet descriptors
		int P = 15;		//Number of FD to save
        if (verbose) {printf(" Computing DFT \n");}
		contours[i].getDFT(fourierDesc);
        if (verbose) {printf(" Computing Wavelet\n");}
		contours[i].getWavelet(waveletDesc);

		for ( int p = 0; p <P; p++ ){
			feats[i].content.push_back(fourierDesc[p]);
			feats[i].content.push_back(waveletDesc[p]);
		}

		feats[i].content.push_back(' ');		// Include a space before closing parenthesis to make reading easier

		// Draw results on images
		contours[i].drawOnImg(src, color);
		contours[i].convHull->drawOnImg(contoursIm, color, 1);
		//contours[i].contourNorm->drawOnImg(contoursIm, color);

		std::ostringstream cntN; 
        cntN << i;
		contours[i].drawText(contoursIm, cntN.str() , color);	
        contours[i].drawArrow(contoursIm,contours[i].getPoints()[0], refAngle, color, 30, 2);

		featSend.content.push_back(feats[i]);
        if (verbose){cout << "Feature Vector of length " << feats[i].content.size() << endl;}
    } //end contours loop

    if (ROIinit){
        rectangle(src, Point (0,0), Point(src.cols,src.rows),  red,2);
    }

	printf("\n DONE \n");
	featPort.write(featSend);
	imPropOutPort.write();
	imFeatOutPort.write();


    mutex.post();

}
