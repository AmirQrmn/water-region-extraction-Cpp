#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>
#include "EnergyBasicFilteringCalculator.h"
#include "EnergyGetDirections.h"
#include "EnergySteer3DG2H2.h"
#include "EnergyNormalizer.h"
#include "enShow.h"

using namespace cv;
using namespace std;

Mat EnergyExtractor(const std::vector<cv::Mat> inputImgsForEnCalcPlanes, const int NumberOfOrientations, const int hgt, const int wdth, const int depth, const int MyFilterFlg, const int origImgWidth, const int origImgHeight, const int EnImgHeight, const int EnImgWidth, const double epsln, const int EnFramesShowFlg, const int NrmForBhtFlg, const int timersShowFlg, const int EnFeaturesMagnifyingEnabler)
{
	double timerVrabl = 0;
	if (timersShowFlg == 1)
	{
		timerVrabl = (double)getTickCount();
	}
	
	Mat ImgEnBasicPlanes = EnergyBasicFilteringCalculator(inputImgsForEnCalcPlanes, hgt, wdth, depth, MyFilterFlg, origImgWidth, origImgHeight, EnImgHeight, EnImgWidth, timersShowFlg);
	std::vector<double> Directions = EnergyGetDirections(NumberOfOrientations, timersShowFlg);
	Mat ImgOrientedEnergyPlanes = EnergySteer3DG2H2(ImgEnBasicPlanes, Directions, NumberOfOrientations, EnImgHeight, EnImgWidth, timersShowFlg);
	Mat matNormalizedEnergyFrames = EnergyNormalizer(ImgOrientedEnergyPlanes, NumberOfOrientations, epsln, EnImgHeight, EnImgWidth, timersShowFlg);
	
	if (EnFeaturesMagnifyingEnabler == 1)
	{
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
			temp.copyTo(matNormalizedEnergyFrames(cv::Rect(0, EnFrmCnt, EnImgWidth, EnImgHeight)));
			EnFrmCnt = EnFrmCnt + EnImgHeight;
			y = y + 1;
		} while (y < NumberOfOrientations);
	}
	
	if (EnFramesShowFlg == 1)
	{
		enShow(matNormalizedEnergyFrames, NumberOfOrientations, EnImgHeight, EnImgWidth);
	}

	if (timersShowFlg == 1)
	{
		timerVrabl = ((double)getTickCount() - timerVrabl) / getTickFrequency();
		std::cout << endl << "EnergyExtractor, total: " << timerVrabl << " s" << std::endl << "--------------------------------------------------------------------------------";
	}

	return matNormalizedEnergyFrames;
}