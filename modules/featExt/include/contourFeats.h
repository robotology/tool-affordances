#ifndef CONTOURFEATS_H
#define CONTOURFEATS_H

//#include <opencv\cv.h>
#include "opencv2/opencv.hpp"
#include <stdio.h>

//forward-declaration
class ConvexHullContour;
class PolyApproxContour;
class NormalizedContour;

static const int LENGTH = 1024;

class Contour
{
	// Contour Transformations	
	void computePoly();
	void computeHull();
	void computeMoments();		
	void computeSkeleton();
	void computeCentDist();
	void computeAngleSig(double refAngle = 0, int win = 2);
	void computeImg();

protected:
	std::vector<cv::Point> points;
	std::vector<int> *convHullInt;
	cv::Moments *moms;
	cv::Mat *skeleton;	
	std::vector<double> *centroidDist;
	std::vector<double> *angleSig;
	cv::Mat *image;

public:
	//Constructor
	Contour();
	Contour(const std::vector<cv::Point>& points);
	~Contour();

	//Pointers to subclasses
	Contour *contourNorm;
	Contour *contourPoly;
	Contour *convHull;

	// Contour transformations
	void normalize(int L=LENGTH);
	void normalize(const std::vector<cv::Point>& noNorm, std::vector<cv::Point>& norm, int L=LENGTH);
	void getPoly(const std::vector<cv::Point>& noPoly, std::vector<cv::Point>& poly, int maxDistance =3);
	void getHull(const std::vector<cv::Point>& noHull, std::vector<cv::Point>& Hull);
		
	// Retrieve info
	const std::vector<cv::Point>& getPoints();
	const cv::Moments& getMoments();		
	const cv::Mat& getSkeleton();
	const std::vector<double>& getCentDist();
	const std::vector<double>& getAngleSig(const double refAngle = 0);
	const cv::Mat& getImg();
	
	// Directly from contour
	double area();
	double perimeter();
	double compactness();
	double length();
	double width();
	double aspectRatio();
	double xtLeft();
	double xtRight();
	double elongation();
	double rectangularity();
	
	// Based on Convex Hull
	double convHullArea();
	double solidity();
	void convDefs(std::vector<cv::Vec4i>& outDefs);
	void convDefPos(const std::vector<cv::Vec4i>& convDefs, std::vector<double>& defDepth, std::vector<int>& defIndx );
	void convDir(const std::vector<int>& defIndx, std::vector<double>& defDirs, const double refAngle = 0);	
    void convDirHist(const std::vector<double>& defDirs, std::vector<double>& dirsHist);	
	
	// From the Thinning algorithm
	void jointPoints(std::vector<cv::Point>& outJoints, std::vector<cv::Point>& ends, bool drawJointsF = false);
	int findBranches(cv::Mat jntROI);
	void drawJoints(std::vector<cv::Point>& joints, std::vector<cv::Point>& ends);

	// From moments
	double nu20();
	double nu02();
	double nu11();
	cv::Point massCenter(bool norm=false);

	// Polygonal Approximation

	// From the segments angle's signature
    void getAngleSigHist(std::vector<double>& angleHist);
	double bendEnergy();		

	// From 1D centroid distance Signature
	void getDFT(std::vector<double>& outDFT , int P = -1);
	void getWavelet(std::vector<double>& outWL);

    // Utils
	void drawOnImg(cv::Mat img, const cv::Scalar& color = cv::Scalar(255, 255, 255), const int thickness = 2 );
	void drawText(cv::Mat img, std::string text,  const cv::Scalar& color = cv::Scalar(255, 255, 255));	
    void drawArrow(cv::Mat image, cv::Point ini, double angle, cv::Scalar color, int arrowMagnitude = 9, int thickness=1, int line_type=8, int shift=0);
    void drawArrow(cv::Mat image, cv::Point ini, cv::Point end, cv::Scalar color, int arrowMagnitude = 9, int thickness=1, int line_type=8, int shift=0);

};

class PolyApproxContour : public Contour
{
public:
	PolyApproxContour(const std::vector<cv::Point>& originalPoints, int maxDistance=3);
};

class ConvexHullContour : public Contour
{
public:
	ConvexHullContour(const std::vector<cv::Point>& originalPoints);
};

class NormalizedContour : public Contour
{
	int nPoints;
public:
	NormalizedContour(const std::vector<cv::Point>& originalPoints, int nPoints =LENGTH);
};
#endif
