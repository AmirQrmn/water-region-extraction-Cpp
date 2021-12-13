#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>
#include "MaritimeClusterIdentificationCriteria.h"
#include "MaritimeClusterSizeCheck.h"

using namespace cv;
using namespace std;

Mat MaritimeClusterIdentifier(Mat msSmoothedEnergyFrames, const cv::Mat LabelsTable, const int EnImgHeight, const int EnImgWidth, const double NrmFlickerSumTr, const double NrmStaticSumUpTr, const double NrmStaticSumDownTr, const int NumOfRgbRegions, const int FlickerAmpFlg, const int FinalMaritimeRegionSizeTr, const int FarFlg, const int GrayLevelDiffTr, const int timersShowFlg, const int TempMaritimeFramePlot)
{
	double timerVrabl = 0;
	if (timersShowFlg == 1)
	{
		timerVrabl = (double)getTickCount();
	}
	Mat Maritimeframe = MaritimeClusterIdentificationCriteria(msSmoothedEnergyFrames, LabelsTable, EnImgHeight, EnImgWidth, NrmFlickerSumTr, NrmStaticSumUpTr, NrmStaticSumDownTr, NumOfRgbRegions, FlickerAmpFlg, timersShowFlg);
	{
		Mat MaritimeframeTempp(EnImgHeight, EnImgWidth, CV_64F, cv::Scalar(0));
		int i = 0;
		do
		{
			int* MaritimeframePntr = Maritimeframe.ptr<int>(i);
			double* MaritimeframeTemppPntr = MaritimeframeTempp.ptr<double>(i);
			int j = 0;
			do
			{
				MaritimeframeTemppPntr[j] = MaritimeframePntr[j];
				j = j + 1;
			} while (j < EnImgWidth);
			i = i + 1;
		} while (i < EnImgHeight);
		if (TempMaritimeFramePlot == 1)
		{
			imshow("Maritimeframe before size check", MaritimeframeTempp);
		}
	}
	Maritimeframe = MaritimeClusterSizeCheck(Maritimeframe, FinalMaritimeRegionSizeTr, FarFlg, EnImgHeight, EnImgWidth, GrayLevelDiffTr, timersShowFlg);
	{
		Mat MaritimeframeTempp(EnImgHeight, EnImgWidth, CV_64F, cv::Scalar(0));
		int i = 0;
		do
		{
			int* MaritimeframePntr = Maritimeframe.ptr<int>(i);
			double* MaritimeframeTemppPntr = MaritimeframeTempp.ptr<double>(i);
			int j = 0;
			do
			{
				MaritimeframeTemppPntr[j] = MaritimeframePntr[j];
				j = j + 1;
			} while (j < EnImgWidth);
			i = i + 1;
		} while (i < EnImgHeight);
		imshow("Maritimeframe", MaritimeframeTempp);
	}
	if (timersShowFlg == 1)
	{
		timerVrabl = ((double)getTickCount() - timerVrabl) / getTickFrequency();
		std::cout << endl << "MaritimeClusterIdentification, total: " << timerVrabl << " s" << std::endl << "--------------------------------------------------------------------------------";
	}
	return Maritimeframe;
}