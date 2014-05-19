//=================================
// include guard
#ifndef __THINNING_H_INCLUDED__
#define __THINNING_H_INCLUDED__

//=================================
// included dependencies
#include "opencv2/opencv.hpp"


//=================================
// forward declared dependencies
void thinningIteration(cv::Mat& im, int iter);
void thinning(cv::Mat& im);

#endif // __THINNING_H_INCLUDED__ 