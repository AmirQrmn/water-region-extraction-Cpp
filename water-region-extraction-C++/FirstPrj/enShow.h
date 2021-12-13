#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

void enShow(const Mat matNormalizedEnergyFrames, const int NumberOfOrientations, const int EnImgHeight, const int EnImgWidth)
{
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	Mat matNormalizedEnergyFrames_toShow{ (NumberOfOrientations*EnImgHeight), EnImgWidth, CV_64F, Scalar(0) };
	int EnFrmCnt = 0;
	int y = 0;
	do
	{
		Mat temp;
		matNormalizedEnergyFrames(cv::Rect(0, EnFrmCnt, EnImgWidth, EnImgHeight)).copyTo(temp);
		double min, max;
		cv::minMaxLoc(temp, &min, &max);
		int i = 0;
		do
		{
			double* tempPntr = temp.ptr<double>(i);
			int j = 0;
			do
			{
				tempPntr[j] = (tempPntr[j] - min) / (max - min);
				j = j + 1;
			} while (j < EnImgWidth);
			i = i + 1;
		} while (i < EnImgHeight);
		temp.copyTo(matNormalizedEnergyFrames_toShow(cv::Rect(0, EnFrmCnt, EnImgWidth, EnImgHeight)));
		EnFrmCnt = EnFrmCnt + EnImgHeight;
		y = y + 1;
	} while (y < NumberOfOrientations);
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	double zz = 1;
	EnFrmCnt = 0;
	int q = 0;
	do
	{
		cv::String name = cv::format("Normalized Energy # %d", q);
		cv::namedWindow(name, 1);
		imshow(name, matNormalizedEnergyFrames_toShow(cv::Rect(0, EnFrmCnt, EnImgWidth, EnImgHeight)) / zz);
		waitKey(1000);
		EnFrmCnt = EnFrmCnt + EnImgHeight;
		q = q + 1;
	} while (q < NumberOfOrientations);
}