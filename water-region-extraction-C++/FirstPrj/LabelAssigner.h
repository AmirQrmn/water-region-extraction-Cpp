#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>
#include "SetLabelsTable.h"
#include "ModifyLabelsTable.h"
#include "BhattacharyyaCoefCalculator.h"

using namespace cv;
using namespace std;

Mat LabelAssigner(Mat msSmoothedEnergyFrames, int &NumOfRgbRegions, const int EnImgHeight, const int EnImgWidth, const int NumberOfOrientations, const double LblEnSimilarNeighborTr, const int timersShowFlg)
{
	double timerVrabl = 0;
	if (timersShowFlg == 1)
	{
		timerVrabl = (double)getTickCount();
	}
	
	Mat LabelsTable(EnImgHeight, EnImgWidth, CV_32S, cv::Scalar(-1));
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
		int* LabelsTablePntr = LabelsTable.ptr<int>(i);
		int* PerviousLineLabelsTablePntr = LabelsTable.ptr<int>(ProperI - 1);
		int j = 0;
		do
		{
			//-----------------------------------------------------------------------------
			if ((i == 0) && (j == 0))
			{
				LabelsTablePntr[j] = LabelCntr;
				LabelCntr = LabelCntr + 1;
			}

			if (LabelsTablePntr[j] == -1)
			{
				LabelsTablePntr[j] = LabelCntr;
				LabelCntr = LabelCntr + 1;
			}

			if (LabelsTablePntr[j] != -1)
			{
				Mat MainHist = { 1, NumberOfOrientations, CV_64F, Scalar(0) };
				double* MainHistPntr = MainHist.ptr<double>(0);
				int q = 0;
				do
				{
					double* msSmEnFrPntr = msSmoothedEnergyFrames.ptr<double>(i + (q*EnImgHeight));
					MainHistPntr[q] = msSmEnFrPntr[j];
					q = q + 1;
				} while (q < NumberOfOrientations);

				if (((i == 0) && (j == 0)) || \
					((i != 0) && (j == 0) && (LabelsTablePntr[j] == PerviousLineLabelsTablePntr[j])) || \
					((j != 0) && (LabelsTablePntr[j] == LabelsTablePntr[j - 1])))
				{
					SetLabelsTable(LabelsTable, LabelCntr, i, j, NumberOfOrientations, MainHist, msSmoothedEnergyFrames, EnImgHeight, EnImgWidth, LblEnSimilarNeighborTr);
				}
				else
				{
					Mat TargHist = { 1, NumberOfOrientations, CV_64F, Scalar(0) };
					double* TargHistPntr = TargHist.ptr<double>(0);
					if (j == 0)
					{
						int q = 0;
						do
						{
							double* msSmEnFrPntr = msSmoothedEnergyFrames.ptr<double>(i + (q*EnImgHeight) - 1);
							TargHistPntr[q] = msSmEnFrPntr[j];
							q = q + 1;
						} while (q < NumberOfOrientations);
						NeihgborID = 2;
					}
					else
					{
						int q = 0;
						do
						{
							double* msSmEnFrPntr = msSmoothedEnergyFrames.ptr<double>(i + (q*EnImgHeight));
							TargHistPntr[q] = msSmEnFrPntr[j - 1];
							q = q + 1;
						} while (q < NumberOfOrientations);
						NeihgborID = 4;
					}

					double BhtRes = BhattacharyyaCoefCalculator(MainHist, TargHist);

					if (BhtRes >= LblEnSimilarNeighborTr)
					{
						ModifyLabelsTable(LabelsTable, LabelCntr, i, j, EnImgHeight, EnImgWidth, NeihgborID);
					}
					else
					{
						SetLabelsTable(LabelsTable, LabelCntr, i, j, NumberOfOrientations, MainHist, msSmoothedEnergyFrames, EnImgHeight, EnImgWidth, LblEnSimilarNeighborTr);
					}
				}
			}
			//	-----------------------------------------------------------------------------
			j = j + 1;
		} while (j < EnImgWidth);
		i = i + 1;
	} while (i < EnImgHeight);
	double min, max;
	cv::minMaxLoc(LabelsTable, &min, &max);
	int maxx = int(max);
	NumOfRgbRegions = maxx + 1;
	if (timersShowFlg == 1)
	{
		timerVrabl = ((double)getTickCount() - timerVrabl) / getTickFrequency();
		std::cout << endl << "LabelAssigner: " << timerVrabl << " s" << std::endl;
	}
	return LabelsTable;
}