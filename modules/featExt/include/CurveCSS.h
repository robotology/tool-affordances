//=================================
// include guard
#ifndef __CURVECSS_H_INCLUDED__
#define __CURVECSS_H_INCLUDED__

#include "opencv2/opencv.hpp"

//#pragma mark Curves Utilities
template<typename T, typename V>
void PolyLineSplit(const std::vector<cv::Point_<T> >& pl,std::vector<V>& contourx, std::vector<V>& contoury) {
	contourx.resize(pl.size()); 
	contoury.resize(pl.size());
	
	for (int j=0; j<pl.size(); j++) 
	{ 
		contourx[j] = (V)(pl[j].x); 
		contoury[j] = (V)(pl[j].y); 
	}
}

template<typename T, typename V>
void PolyLineMerge(std::vector<cv::Point_<T> >& pl, const std::vector<V>& contourx, const std::vector<V>& contoury) {
	assert(contourx.size()==contoury.size());
	pl.resize(contourx.size());
	for (int j=0; j<contourx.size(); j++) {
		pl[j].x = (T)(contourx[j]);
		pl[j].y = (T)(contoury[j]);
	}
}

template<typename T, typename V>
void ConvertCurve(const std::vector<cv::Point_<T> >& curve, std::vector<cv::Point_<V> >& output) {
	output.clear();
	for (int j=0; j<curve.size(); j++) {
		output.push_back(Point_<V>(curve[j].x,curve[j].y));
	}
}

void ResampleCurve(const std::vector<double>& curvex, const std::vector<double>& curvey,
				   std::vector<double>& resampleX, std::vector<double>& resampleY,
				   int N, bool isOpen = false
				   );

void ResampleCurve2D(const std::vector<cv::Point>& curve,
				   std::vector<cv::Point>& resampled,
				   int N, bool isOpen = false
				   );

template<typename T>
void drawOpenCurve(cv::Mat& img, const std::vector<cv::Point_<T> >& curve, cv::Scalar color, int thickness) {
	std::vector<Point> curve2i;
	ConvertCurve(curve, curve2i);
	for (int i=0; i<curve2i.size()-1; i++) {
		line(img, curve2i[i], curve2i[i+1], color, thickness);
	}
}

//#pragma mark CSS Image

void ComputeCurveCSS(const std::vector<double>& curvex, 
					 const std::vector<double>& curvey, 
					 std::vector<double>& kappa, 
					 std::vector<double>& smoothX,std::vector<double>& smoothY,
					 double sigma = 1.0,
					 bool isOpen = false);

std::vector<int> FindCSSInterestPoints(const std::vector<double>& kappa);

std::vector<int> ComputeCSSImageMaximas(const std::vector<double>& contourx_, const std::vector<double>& contoury_,
								   std::vector<double>& contourx, std::vector<double>& contoury, bool isClosedCurve = true);

template<typename T>
void ComputeCurveCSS(const std::vector<cv::Point_<T> >& curve, 
					 std::vector<double>& kappa, 
					 std::vector<cv::Point_<T> >& smooth,
					 double sigma,
					 bool isOpen = false
					 ) 
{
	std::vector<double> contourx(curve.size()),contoury(curve.size());
	PolyLineSplit(curve, contourx, contoury);
	
	std::vector<double> smoothx, smoothy;
	ComputeCurveCSS(contourx, contoury, kappa, smoothx, smoothy, sigma, isOpen);
	
	PolyLineMerge(smooth, smoothx, smoothy);	
}

//#pragma mark Curve Segments

template<typename T, typename V>
void GetCurveSegments(const std::vector<cv::Point_<T> >& curve, const std::vector<int>& interestPoints, std::vector<std::vector<cv::Point_<V> > >& segments, bool closedCurve = true) {
	if (closedCurve) {
		segments.resize(interestPoints.size());
	} else {
		segments.resize(interestPoints.size()+1);
	}

	for (int i = (closedCurve)?0:1; i<segments.size()-1; i++) {
		int intpt_idx = (closedCurve)?i:i-1;
		segments[i].clear();
		for (int j=interestPoints[intpt_idx]; j<interestPoints[intpt_idx+1]; j++) {
			segments[i].push_back(Point_<V>(curve[j].x,curve[j].y));
		}
	}
	if (closedCurve) {
		//put in the segment that passes the 0th point
		segments.back().clear();
		for (int j=interestPoints.back(); j<curve.size(); j++) {
			segments.back().push_back(Point_<V>(curve[j].x,curve[j].y));
		}
		for (int j=0; j<interestPoints[0]; j++) {
			segments.back().push_back(Point_<V>(curve[j].x,curve[j].y));
		}
	} else {
		//put in the segment after the last point
		segments.back().clear();
		for (int j=interestPoints.back(); j<curve.size(); j++) {
			segments.back().push_back(Point_<V>(curve[j].x,curve[j].y));
		}
		//put in the segment before the 1st point
		segments.front().clear();
		for (int j=0; j<interestPoints[0]; j++) {
			segments.front().push_back(Point_<V>(curve[j].x,curve[j].y));
		}
	}
	for (int i=0; i<segments.size(); i++) {
		std::vector<double> x,y;
		cout <<"segments[i].size() " << segments[i].size() << endl;
		PolyLineSplit(segments[i], x, y); ResampleCurve(x, y, x, y, 50,true); PolyLineMerge(segments[i], x, y);
	}
}
template<typename T, typename V>
void GetCurveSegmentsWithCSSImage(std::vector<cv::Point_<T> >& curve, std::vector<int>& interestPoints, std::vector<std::vector<cv::Point_<V> > >& segments, bool closedCurve = true) {
	std::vector<double> contourx(curve.size()),contoury(curve.size());
	PolyLineSplit(curve, contourx, contoury);
	
	std::vector<double> smoothx, smoothy;
	interestPoints = ComputeCSSImageMaximas(contourx, contoury, smoothx, smoothy);
	
	PolyLineMerge(curve, smoothx, smoothy);
	
	double minx,maxx; minMaxLoc(smoothx, &minx, &maxx);
	double miny,maxy; minMaxLoc(smoothy, &miny, &maxy);
	cv::Mat drawing(maxy,maxx,CV_8UC3,Scalar(0));
	RNG rng(time(NULL));
	Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
//	vector<vector<Point_<T> > > contours(1,curve);
//	drawContours( drawing, contours, 0, color, 2, 8);
	drawOpenCurve(drawing, curve, color, 2);
	
	for (int m=0; m<interestPoints.size() ; m++) {
		circle(drawing, curve[interestPoints[m]], 5, Scalar(0,255), CV_FILLED);
	}
	imshow("curve interests", drawing);
	waitKey();
	
	GetCurveSegments(curve, interestPoints, segments, closedCurve);
}

//#pragma mark Matching

double MatchTwoSegments(const std::vector<cv::Point2d>& a, const std::vector<cv::Point2d>& b);
double MatchCurvesSmithWaterman(const std::vector<std::vector<cv::Point2d> >& a, const std::vector<std::vector<cv::Point2d> >& b, std::vector<cv::Point>& traceback); 
double AdaptedMatchCurvesSmithWaterman(const std::vector<std::vector<cv::Point2d> >& a, const std::vector<std::vector<cv::Point2d> >& b, std::vector<cv::Point>& traceback);


#endif // __CURVECSS_H_INCLUDED__ 