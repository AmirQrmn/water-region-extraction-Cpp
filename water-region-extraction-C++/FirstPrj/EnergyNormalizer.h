#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

Mat EnergyNormalizer(const Mat ImgOrientedEnergyPlanes, const int NumberOfOrientations, const double epsln, const int EnImgHeight, const int EnImgWidth, const int timersShowFlg)
{
	double timerVrabl = 0;
	if (timersShowFlg == 1)
	{
		timerVrabl = (double)getTickCount();
	}
	Mat matNormalizedEnergyFrames{ (NumberOfOrientations*EnImgHeight), EnImgWidth, CV_64F, Scalar(0) };
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	double min, max;
	cv::minMaxLoc(ImgOrientedEnergyPlanes, &min, &max);
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	double FrameEpsilon = epsln * max;
	Mat denominator(EnImgHeight, EnImgWidth, CV_64F, cv::Scalar(0));
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	int q = 0; 
	do
	{
		denominator = denominator + ImgOrientedEnergyPlanes(cv::Rect(0, (q*EnImgHeight), EnImgWidth, EnImgHeight));
		q = q + 1;
	} while (q < NumberOfOrientations - 1);
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	denominator = denominator + FrameEpsilon;
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	int r = 0;
	do
	{
		divide(ImgOrientedEnergyPlanes(cv::Rect(0, (r*EnImgHeight), EnImgWidth, EnImgHeight)), denominator, matNormalizedEnergyFrames(cv::Rect(0, (r*EnImgHeight), EnImgWidth, EnImgHeight)));
		r = r + 1;
	} while (r < NumberOfOrientations - 1);
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	divide(FrameEpsilon, denominator, matNormalizedEnergyFrames(cv::Rect(0, ((NumberOfOrientations - 1) * EnImgHeight), EnImgWidth, EnImgHeight)));
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	if (timersShowFlg == 1)
	{
		timerVrabl = ((double)getTickCount() - timerVrabl) / getTickFrequency();
		std::cout << endl << "EnergyNormalizer: " << timerVrabl << " s" << std::endl;
	}
	return matNormalizedEnergyFrames;
}