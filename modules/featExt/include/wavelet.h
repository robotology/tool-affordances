//=================================
// include guard
#ifndef __WAVELET_H_INCLUDED__
#define __WAVELET_H_INCLUDED__

//=================================
// included dependencies
#include "opencv2/opencv.hpp"

//=================================
// forward declared dependencies
float sgn(float x);
float soft_shrink(float d,float T);
float hard_shrink(float d,float T);
float Garrot_shrink(float d,float T);
static void cvHaarWavelet(cv::Mat &src,cv::Mat &dst,int NIter);
static void cvInvHaarWavelet(cv::Mat &src,cv::Mat &dst,int NIter, int shrinkage_type=0, float shrinkage_t=50);

void Haar1DNorm(std::vector<double>& src, std::vector<double>& dst);			// Extra implementation added - Haar 1 Normalized
void Haar1D(std::vector<double>& src, std::vector<double>& dst);				// Extra implementation added


#endif // __WAVELET_H_INCLUDED__ 