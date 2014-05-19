#include "contourFeats.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <thinning.h>
#include <CurveCSS.h>
#include <iCub/ctrl/functionEncoder.h>

using namespace cv;
using namespace std;

// Constructor
Contour::Contour()
{
	this->contourNorm = NULL;
	this->contourPoly = NULL;
	this->convHull = NULL;
	this->convHullInt = NULL;
	this->moms = NULL;
	this->skeleton = NULL;
	this->centroidDist = NULL;
	this->angleSig = NULL;
	this->image = NULL;
}

Contour::Contour(const vector<cv::Point>& points)
{
	this->contourNorm = NULL;
	this->contourPoly = NULL;
	this->convHull = NULL;
	this->convHullInt = NULL;
	this->moms = NULL;
	this->skeleton = NULL;
	this->centroidDist = NULL;
	this->angleSig = NULL;
	this->image = NULL;
	this->points=points;
}

Contour::~Contour()
{
	if(contourNorm)
		delete contourNorm;
	if(contourPoly)
		delete contourPoly;
	if(convHull)
		delete convHull;
	if(convHullInt)
		delete convHullInt;
	if(moms)
		delete moms;
	if(skeleton)
		delete skeleton;
	if(centroidDist)
		delete centroidDist;
	if(angleSig)
		delete angleSig;
	if(image)
		delete image;
}


// Normalization
void Contour::normalize(int L)
{
	if (this->contourNorm==NULL){
		contourNorm = new NormalizedContour(points, L);
	}
}

void Contour::normalize(const vector<Point>& noNorm, vector<Point>& norm, int L)
{
	ResampleCurve2D(noNorm, norm, L);
}

// Polygonal Approximation
void Contour::computePoly()
{
	if (this->contourPoly==NULL){
			contourPoly = new PolyApproxContour( points );
	}
}

void Contour::getPoly(const vector<Point>& noPoly, vector<Point>& poly, int maxDistance)
{
	approxPolyDP( noPoly, poly, maxDistance, true );
}

// Convex Hull
void Contour::computeHull()
{
	if (this->convHullInt==NULL){
		convHull = new ConvexHullContour(points);
		convHullInt = new vector<int>;
		convexHull(points, *convHullInt, false);
	}
}

void Contour::getHull(const vector<Point>& noHull, vector<Point>& hull)
{	
	convexHull(noHull, hull, false);
}

// Compute characteristics
void Contour::computeMoments()
{
	if (this->moms==NULL)
	{
		moms =new Moments(moments(points, false));
	}
}

// Retreieve info form class
const vector<Point>& Contour::getPoints()
{
	return points;
}

const Moments& Contour::getMoments()
{
	computeMoments();
	return *moms;
}

const Mat& Contour::getSkeleton()
{
	computeSkeleton();
	return *skeleton;
}

const vector<double>& Contour::getCentDist()
{
	computeCentDist();
	return *centroidDist;
}
const vector<double>& Contour::getAngleSig()
{
	computeAngleSig();
	return *angleSig;
}


const Mat& Contour::getImg()
{
	computeImg();
	return *image;
}


// Directly from contour
double Contour::area()
{
	return contourArea(points);
}

double Contour::perimeter()
{
	return arcLength(points, true );
}

double Contour::compactness()
{
	double a = this->area();
	double p = this->perimeter();
	return 4*CV_PI*a/pow(p,2);
}

double Contour::length()
{
	RotatedRect minEllipse = fitEllipse( points );
	return minEllipse.size.height;
}

double Contour::width()
{
	RotatedRect minEllipse = fitEllipse( points );
	return minEllipse.size.width;
}

double Contour::aspectRatio()
{
	Rect boundRect = boundingRect(this->points);
	double width = abs(boundRect.tl().x-boundRect.br().x);
	double height = abs(boundRect.tl().y-boundRect.br().y);
	return width/height;
}

double Contour::xtLeft()
{
	Rect boundRect = boundingRect( points );
	computeMoments();
	Point mc = massCenter();
	return abs(boundRect.tl().x - mc.x);
}

double Contour::xtRight()
{
	Rect boundRect = boundingRect( points );
	computeMoments();
	Point mc = massCenter();
	return abs(boundRect.br().x - mc.x);
}

double Contour::elongation()
{
	return length()/width();
}

double Contour::rectangularity()
{
	return area()/(width()*length());
}

// Based on moments
double Contour::nu20()
{
	computeMoments();
	return moms->nu20;
}

double Contour::nu02()
{
	computeMoments();
	return moms->nu02;
}

double Contour::nu11()
{
	computeMoments();
	return moms->nu11;
}

Point Contour::massCenter(bool norm)
{
	computeMoments();
	if (norm){
		Rect boundRect = boundingRect( points );
		Point mc = Point( moms->m10/moms->m00 , moms->m01/moms->m00 );
		return mc - Point(boundRect.tl());
	}
	return Point( moms->m10/moms->m00 , moms->m01/moms->m00 );  //Center Mass

}

// Based on Convex Hull
double Contour::convHullArea()
{
	computeHull();
	return convHull->area();
}

double Contour::solidity()
{
	computeHull();
	double a = this->area();
	double ca = this->convHullArea();
	return a/ca;
}

void Contour::convDefs(vector<Vec4i>& convexDefects)
{
	computeHull();	
	if (convHullInt->size()>4){
		convexityDefects(points, *convHullInt, convexDefects);
	}
}



void Contour::convDefPos(const std::vector<Vec4i>& convDefs, std::vector<double>& defDepth, std::vector<int>& defIndx )
{
	for(int j=0; j<convDefs.size();j++)				// Save depth of convexity defects
	{				
		if (convDefs[j].val[3] > 1024){			// min depth of 4 points (256*4)
			defDepth.push_back(convDefs[j].val[3]/256.0);
			defIndx.push_back(convDefs[j].val[2]);
			// XXXX Find a way to represent in a meaningful way the convex points. Like polar coords from the tip or from the base...
		}
	}
}

// Get the direction of the convexity defects
void Contour::convDir(const std::vector<int>& defIndx, std::vector<double>& defDirs)
{
	if (defIndx.size()==0){
		defDirs.clear();
	}
	else{
		vector<double> angleSeg = getAngleSig();
		for(int j=0; j<defIndx.size();j++){
			//Use the indices of the convexity defects to check the direction of the convex points using the normal at those points.
			double tanAng = (angleSeg[defIndx[j]]+angleSeg[defIndx[j]-1])/2;
			defDirs.push_back(tanAng);
		}
	}
}

// From the Thinning algorithm
void Contour::computeSkeleton()
{	
	computeImg();				// Skeleton is based on the MAT image from the contour
	if (this->skeleton==NULL){		
		Mat binary = this->getImg().clone();
		thinning(binary);
		skeleton = new Mat(binary);
	}	
}


void Contour::jointPoints(vector<Point>& joints, vector<Point>& ends, bool drawJointsF)
{	// Takes a 1-pixel wide skeleton binary image
	// Finds the end and joint points (pixels from where either only one or 3 or more branches emerge, respectively) 
	computeSkeleton();
	for( int y = 1; y < skeleton->rows-1; y++ ){ 
		for( int x = 1; x < skeleton->cols-1; x++ ){
			if (skeleton->at<uchar>(y,x)==255){			// We only look for branches from white pixels for effciency.
				Rect roi(x-1,y-1, 3, 3);
				Mat jntROIaux(*skeleton,roi);			// Select the pixels around the bright one as the ROI		
				Mat jntROI= jntROIaux.clone();			// Copy ROI because it needs to be modified
				int neigN = findBranches(jntROI);		// Find the number of branches starting on that pixel
				if (neigN>=3){							// 3 or more branches mean a joint 
					joints.push_back(Point(x,y));	}
				if (neigN==1){							// a single branch means an end point.
					ends.push_back(Point(x,y));		}
			}
		}
	}
	if (drawJointsF)
		drawJoints(joints, ends);
}


int Contour::findBranches(Mat jntROI)
{	// Takes a ROI from a binary image and finds the number of gaps between branches by flooding
	// Being a closed circle, the number of gaps is equal to the number of branches
	int branches = 0;
	int label_count = 240;							// label to flood with
	for(int y=0; y < jntROI.rows; y++) {
        for(int x=0; x < jntROI.cols; x++) {			
            uchar checker = jntROI.at<uchar>(y,x);	// retrieves pixel value 
            Rect rect;
            if(checker == 0) {						// Floods from that seed if pixel is 0
                floodFill(jntROI, Point(x,y), Scalar(label_count), &rect, Scalar(0), Scalar(0), 4);
                label_count = label_count - 20;		// change label
				branches++;							// add 1 to branch count
            }
        }
    }
	return branches;
}

void Contour::drawJoints(vector<Point>& joints, vector<Point>& ends)
{	// Draws circles around the joints and end points
	for(int i=0; i < joints.size(); i++) {
		circle(*skeleton, joints[i], 5, Scalar(128), 1, 8, 0 );
	}
	for(int j=0; j < ends.size(); j++) {
		circle(*skeleton, ends[j], 5, Scalar(255), 1, 8, 0 );
	}
}


// Get centroid distance Signature
void Contour::computeCentDist()
{	// Computes and saves the distance from the centroid to all points in the contour
	normalize();						// Always extract signature from normalized vector
	if (this->centroidDist==NULL){
		centroidDist = new vector<double>;
		for( int p = 0; p < contourNorm->points.size(); p++ ){		// Compute distance from all points to centroid
			double distDent = norm(contourNorm->points[p]- this->massCenter());
			centroidDist->push_back(distDent);
		}
	}	
}

// Compute the normal vector to the contour at each point, and return its angle.
void Contour::computeAngleSig()
{
	//normalize();
	if (this->angleSig==NULL){		
		angleSig = new vector<double>;
		vector<Point> pp = points;	
		for( int i = 0; i < pp.size(); i++ ){	
			double angle;
			if (i==pp.size())
				angle = atan2(double(pp[0].y - pp[i].y),double( pp[0].x - pp[i].x)) * 180.0 / CV_PI; 
			else
				angle = atan2(double(pp[i+1].y - pp[i].y),double( pp[i+1].x - pp[i].x)) * 180.0 / CV_PI;
			angleSig->push_back(angle);
		}
	}
}

double Contour::bendEnergy()
{	// computes the bending energy: intregral of the contour curvature (differential of the contours angle)
	// Normalized by the length of the contour.
	computeAngleSig();
	vector<double> angles = *(this->angleSig);
	double accumBend = 0;
	double angDiff;
	for( int i = 0; i < angles.size(); i++ ){
		// Compute accumulated angle by adding each segments angle  to the previous one.
		if (i==angles.size()){
			angDiff = angles[0]-angles[i];
			accumBend += pow(angDiff,2);
		}
		else{
			angDiff = angles[i+1]-angles[i];
			accumBend += pow(angDiff,2);
		}
	}
	return accumBend / angles.size();
}


void Contour::getDFT(vector<double>& outDFT, int P )
{		
	computeCentDist();
	if (P>centroidDist->size() || P<0 )			//Limit L to vector size
		P=centroidDist->size();
	vector<double> DFT;
	dft(*centroidDist, DFT);//, DFT_REAL_OUTPUT );
	for( int p = 0; p < P; p++ ){		// return only the L first ones
		outDFT.push_back(DFT[p]);}
}


void Contour::getWavelet(vector<double>& outWL)
{		
	computeCentDist();	
	double WAVE_R = contourNorm->points.size();
	yarp::sig::Vector vecSig;
	for ( int p = 0; p < centroidDist->size(); p++ )
		vecSig.push_back(centroidDist->operator[](p));
	// --------WAVED
	iCub::ctrl::WaveletEncoder enc;
	iCub::ctrl::Code wavedCoeffs = enc.encode(vecSig);
    //fprintf(stdout,"%s \n\n", wavedCoeffs.coefficients.toString().c_str());
	for ( int k = 0; k < wavedCoeffs.coefficients.size(); k++ )
		outWL.push_back(wavedCoeffs.coefficients[k]);
}


// Utils
void Contour::drawOnImg(Mat img, const Scalar& color)
{
	vector<vector<Point>> all;
	all.push_back(points);
	drawContours( img, all, -1,  color, 2, 8);
}

void Contour::drawText(Mat img, string text, const Scalar& color)
{
	putText( img, text, massCenter(), CV_FONT_HERSHEY_COMPLEX, 0.5,color);
	vector<vector<Point>> all;
	all.push_back(points);
	drawContours( img, all, -1,  color, 2, 8);
}

void Contour::computeImg()
{	// Prepares a Mat image from the vector<Point> contour, of size the upright bounding rectangle
	if (this->image==NULL)
	{
		vector<vector<Point>> cntVec;	
		Rect boundRect = boundingRect( points );
		// Shift cooordinates of the contour to its bounding box coordintates
		vector<Point> cntDispl (points.size());
		for ( int i = 0; i < points.size(); i++ )		{
			cntDispl[i].x = points[i].x-Point(boundRect.tl()).x;
			cntDispl[i].y = points[i].y-Point(boundRect.tl()).y;
		}
		// Create image size of the bouding rectangle and initalize it black
		this->image = new Mat(boundRect.height,boundRect.width,CV_8UC1);
		image->setTo(Scalar(0));			
		//Push vector in vector of vectors to draw it
		cntVec.clear();
		cntVec.push_back(cntDispl);
		drawContours( *image, cntVec, 0, Scalar(255), 1, 8); 
		// Fill with white points inside the contour
		for( int m = 0; m < image->rows; m++ ){ 
			for( int n = 0; n < image->cols; n++ ){ 
				image->at<uchar>(m,n) = pointPolygonTest( cntVec[0], Point2f(n,m), false )*255; 
			}
		}
	}
}

// Inherited classes definition
PolyApproxContour::PolyApproxContour(const std::vector<cv::Point>& originalPoints, int maxDistance)
{
	//printf(" Creating poly approx object \n");
	approxPolyDP( originalPoints, points, maxDistance, true );
}

ConvexHullContour::ConvexHullContour(const std::vector<cv::Point>& originalPoints)
{
	//printf(" Creating hull object \n");
	convexHull(originalPoints, points, false);
}

NormalizedContour::NormalizedContour(const std::vector<cv::Point>& originalPoints, int nPoints)
{
	//printf(" Creating norm object \n");
	ResampleCurve2D(originalPoints, points, nPoints);
}


