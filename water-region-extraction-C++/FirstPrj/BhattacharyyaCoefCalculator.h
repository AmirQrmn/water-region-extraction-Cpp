#ifndef BhattacharyyaCoefCalculator__
#define BhattacharyyaCoefCalculator__

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

double BhattacharyyaCoefCalculator(Mat FirstHist, Mat SecondHist)
{
	Mat BhtTmp;
	double BhtRes;

	sqrt((FirstHist.mul(SecondHist)), BhtTmp);
	BhtRes = sum(BhtTmp)[0];

	return BhtRes;
}


#endif