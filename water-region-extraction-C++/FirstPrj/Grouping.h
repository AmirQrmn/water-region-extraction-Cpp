#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>
#include "MeanShiftSmoother.h"
#include "LabelAssigner.h"
#include "ClusterRgbPlot.h"

using namespace cv;
using namespace std;

Mat Grouping(Mat matEnergyHolder, Mat &msSmoothedEnergyFrames, int &NumOfRgbRegions, const int EnImgHeight, const int EnImgWidth, const int NumberOfOrientations, const int depth, const int msI, const int msJ, const int msK, const double msBhtSimilarityTr, const double msHistSimTr, const int msItrMax, const double LblEnSimilarNeighborTr, const int RGBplotFlg, const int meanSshiftRowNumPrint, const int timersShowFlg)
{
	double timerVrabl = 0;
	if (timersShowFlg == 1)
	{
		timerVrabl = (double)getTickCount();
	}
	msSmoothedEnergyFrames = MeanShiftSmoother(matEnergyHolder, EnImgHeight, EnImgWidth, NumberOfOrientations, depth, msI, msJ, msK, msBhtSimilarityTr, msHistSimTr, msItrMax, meanSshiftRowNumPrint, timersShowFlg);
	Mat LabelsTable = LabelAssigner(msSmoothedEnergyFrames, NumOfRgbRegions, EnImgHeight, EnImgWidth, NumberOfOrientations, LblEnSimilarNeighborTr, timersShowFlg);
	if (RGBplotFlg == 1)
	{
		ClusterRgbPlot(LabelsTable, EnImgHeight, EnImgWidth, NumOfRgbRegions, timersShowFlg);
	}	
	if (timersShowFlg == 1)
	{
		timerVrabl = ((double)getTickCount() - timerVrabl) / getTickFrequency();
		std::cout << endl << "Grouping, total: " << timerVrabl << " s" << std::endl << "--------------------------------------------------------------------------------";
	}
	return LabelsTable;
}