#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>
#include "BwSetLabelsTable.h"
#include "ModifyLabelsTable.h"

using namespace cv;
using namespace std;

Mat MyBwLabeler(Mat Maritimeframe, int &NumberOfMaritimeRegions, const int EnImgHeight, const int EnImgWidth, const int GrayLevelDiffTr)
{
	Mat bwLabelsTable(EnImgHeight, EnImgWidth, CV_32S, cv::Scalar(-1));
	int LabelCntr = 0;
	int NeihgborID;
	int ProperI;
	int i = 0;
	do
	{
		if (i == 0)
		{
			ProperI = 1;
		}
		else
		{
			ProperI = i;
		}
		int* bwLabelsTablePntr = bwLabelsTable.ptr<int>(i);
		int* PerviousLinebwLabelsTablePntr = bwLabelsTable.ptr<int>(ProperI - 1);
		int* MaritimeframePntr = Maritimeframe.ptr<int>(i);
		int* PerviousLineMaritimeframePntr = Maritimeframe.ptr<int>(ProperI - 1);
		int j = 0;
		do
		{
			//-----------------------------------------------------------------------------
			if ((i == 0) && (j == 0))
			{
				bwLabelsTablePntr[j] = LabelCntr;
				LabelCntr = LabelCntr + 1;
			}

			if (bwLabelsTablePntr[j] == -1)
			{
				bwLabelsTablePntr[j] = LabelCntr;
				LabelCntr = LabelCntr + 1;
			}

			if (bwLabelsTablePntr[j] != -1)
			{
				int CurrentPixelVal = MaritimeframePntr[j];
				if (((i == 0) && (j == 0)) || \
					((i != 0) && (j == 0) && (bwLabelsTablePntr[j] == PerviousLinebwLabelsTablePntr[j])) || \
					((j != 0) && (bwLabelsTablePntr[j] == bwLabelsTablePntr[j - 1])))
				{
					BwSetLabelsTable(bwLabelsTable, LabelCntr, i, j, CurrentPixelVal, Maritimeframe, EnImgHeight, EnImgWidth, GrayLevelDiffTr);
				}
				else
				{
					int TargetPixelVal;
					if (j == 0)
					{
						TargetPixelVal = PerviousLineMaritimeframePntr[j];
						NeihgborID = 2;
					}
					else
					{
						TargetPixelVal = MaritimeframePntr[j-1];
						NeihgborID = 4;
					}
					int GrayLevelDifferenceVal = GrayLevelDiffCalculator(CurrentPixelVal, TargetPixelVal);


					if (GrayLevelDifferenceVal <= GrayLevelDiffTr)
					{
						ModifyLabelsTable(bwLabelsTable, LabelCntr, i, j, EnImgHeight, EnImgWidth, NeihgborID);
					}
					else
					{
						BwSetLabelsTable(bwLabelsTable, LabelCntr, i, j, CurrentPixelVal, Maritimeframe, EnImgHeight, EnImgWidth, GrayLevelDiffTr);
					}
				}
			}
			//	-----------------------------------------------------------------------------
			j = j + 1;
		} while (j < EnImgWidth);
		i = i + 1;
	} while (i < EnImgHeight);
	double min, max;
	cv::minMaxLoc(bwLabelsTable, &min, &max);
	int maxx = int(max);
	NumberOfMaritimeRegions = maxx + 1;
	return bwLabelsTable;
}