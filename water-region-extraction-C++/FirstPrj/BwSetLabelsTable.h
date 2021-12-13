#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>
#include "ModifyLabelsTable.h"
#include "GrayLevelDiffCalculator.h"

using namespace cv;
using namespace std;

void BwSetLabelsTable(Mat &bwLabelsTable, int &LabelCntr, const int i, const int j, const int CurrentPixelVal, Mat Maritimeframe, const int EnImgHeight, const int EnImgWidth, const int GrayLevelDiffTr)
{
	int NeihgborID;
	int* MaritimeframePntr = Maritimeframe.ptr<int>(i);
	int* bwLabelsTablePntr = bwLabelsTable.ptr<int>(i);
	int ProperIup;
	if (i == EnImgHeight - 1)
	{
		ProperIup = EnImgHeight - 2;
	}
	else
	{
		ProperIup = i;
	}
	int* nextLineMaritimeframePntr = Maritimeframe.ptr<int>(ProperIup + 1);
	int* nextLinebwLabelsTablePntr = bwLabelsTable.ptr<int>(ProperIup + 1);
	//*************************************************************************************************************************************************
	if (j < EnImgWidth - 1)
	{
		NeihgborID = 5;
		int TargetPixelVal = MaritimeframePntr[j + 1];
		int GrayLevelDifferenceVal = GrayLevelDiffCalculator(CurrentPixelVal, TargetPixelVal);
		if (bwLabelsTablePntr[j + 1] == -1)
		{
			if (GrayLevelDifferenceVal <= GrayLevelDiffTr)
			{
				bwLabelsTablePntr[j + 1] = bwLabelsTablePntr[j];
			}
		}
		else
		{
			if ((bwLabelsTablePntr[j + 1] != bwLabelsTablePntr[j]) && (GrayLevelDifferenceVal <= GrayLevelDiffTr))
			{
				ModifyLabelsTable(bwLabelsTable, LabelCntr, i, j, EnImgHeight, EnImgWidth, NeihgborID);
			}
		}
	}
	//----------------------------------------------------------------------------------------------------------------------------------------------------
	if ((i < EnImgHeight - 1) && (j > 1))
	{
		NeihgborID = 6;
		int TargetPixelVal = nextLineMaritimeframePntr[j - 1];
		int GrayLevelDifferenceVal = GrayLevelDiffCalculator(CurrentPixelVal, TargetPixelVal);
		if (nextLinebwLabelsTablePntr[j-1] == -1)
		{
			if (GrayLevelDifferenceVal <= GrayLevelDiffTr)
			{
				nextLinebwLabelsTablePntr[j-1] = bwLabelsTablePntr[j];
			}
		}
		else
		{
			if ((nextLinebwLabelsTablePntr[j-1] != bwLabelsTablePntr[j]) && (GrayLevelDifferenceVal <= GrayLevelDiffTr))
			{
				ModifyLabelsTable(bwLabelsTable, LabelCntr, i, j, EnImgHeight, EnImgWidth, NeihgborID);
			}
		}
	}
	//----------------------------------------------------------------------------------------------------------------------------------------------------
	if (i < EnImgHeight - 1)
	{
		NeihgborID = 7;
		int TargetPixelVal = nextLineMaritimeframePntr[j];
		int GrayLevelDifferenceVal = GrayLevelDiffCalculator(CurrentPixelVal, TargetPixelVal);
		if (nextLinebwLabelsTablePntr[j] == -1)
		{
			if (GrayLevelDifferenceVal <= GrayLevelDiffTr)
			{
				nextLinebwLabelsTablePntr[j] = bwLabelsTablePntr[j];
			}
		}
		else
		{
			if ((nextLinebwLabelsTablePntr[j] != bwLabelsTablePntr[j]) && (GrayLevelDifferenceVal <= GrayLevelDiffTr))
			{
				ModifyLabelsTable(bwLabelsTable, LabelCntr, i, j, EnImgHeight, EnImgWidth, NeihgborID);
			}
		}
	}
	//----------------------------------------------------------------------------------------------------------------------------------------------------
	if ((i < EnImgHeight - 1) && (j < EnImgWidth - 1))
	{
		NeihgborID = 8;
		int TargetPixelVal = nextLineMaritimeframePntr[j + 1];
		int GrayLevelDifferenceVal = GrayLevelDiffCalculator(CurrentPixelVal, TargetPixelVal);
		if (nextLinebwLabelsTablePntr[j+1] == -1)
		{
			if (GrayLevelDifferenceVal <= GrayLevelDiffTr)
			{
				nextLinebwLabelsTablePntr[j+1] = bwLabelsTablePntr[j];
			}
		}
		else
		{
			if ((nextLinebwLabelsTablePntr[j+1] != bwLabelsTablePntr[j]) && (GrayLevelDifferenceVal <= GrayLevelDiffTr))
			{
				ModifyLabelsTable(bwLabelsTable, LabelCntr, i, j, EnImgHeight, EnImgWidth, NeihgborID);
			}
		}
	}
	//----------------------------------------------------------------------------------------------------------------------------------------------------
}