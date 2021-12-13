#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>
#include "MyBwLabeler.h"
#include "MatrixNot.h"

using namespace cv;
using namespace std;

Mat MaritimeClusterSizeCheck(Mat Maritimeframe, const int FinalMaritimeRegionSizeTr, const int FarFlg, const int EnImgHeight, const int EnImgWidth, const double GrayLevelDiffTr, const int timersShowFlg)
{
	double timerVrabl = 0;
	if (timersShowFlg == 1)
	{
		timerVrabl = (double)getTickCount();
	}
	int NumberOfMaritimeRegions;
	Mat bwLabelsTable = MyBwLabeler(Maritimeframe, NumberOfMaritimeRegions, EnImgHeight, EnImgWidth, GrayLevelDiffTr);
	int ClstrNr = 0;
	do
	{
		int ClstrCounter = 0;
		int i = 0;
		do
		{
			int* bwLabelsTablePntr = bwLabelsTable.ptr<int>(i);
			int j = 0;
			do
			{
				if (bwLabelsTablePntr[j] == ClstrNr)
				{
					ClstrCounter = ClstrCounter + 1;
				}
				j = j + 1;
			} while (j < EnImgWidth);
			i = i + 1;
		} while (i < EnImgHeight);
		if (ClstrCounter < FinalMaritimeRegionSizeTr)
		{
			int i = 0;
			do
			{
				int* bwLabelsTablePntr = bwLabelsTable.ptr<int>(i);
				int* MaritimeframePntr = Maritimeframe.ptr<int>(i);
				int j = 0;
				do
				{
					if (bwLabelsTablePntr[j] == ClstrNr)
					{
						MaritimeframePntr[j] = 0;
					}
					j = j + 1;
				} while (j < EnImgWidth);
				i = i + 1;
			} while (i < EnImgHeight);
		}
		ClstrNr = ClstrNr + 1;
	} while (ClstrNr < NumberOfMaritimeRegions);
	
	MatrixNot(Maritimeframe, EnImgHeight, EnImgWidth);

	bwLabelsTable = MyBwLabeler(Maritimeframe, NumberOfMaritimeRegions, EnImgHeight, EnImgWidth, GrayLevelDiffTr);

	ClstrNr = 0;
	do
	{
		int ClstrCounter = 0;
		int i = 0;
		do
		{
			int* bwLabelsTablePntr = bwLabelsTable.ptr<int>(i);
			int j = 0;
			do
			{
				if (bwLabelsTablePntr[j] == ClstrNr)
				{
					ClstrCounter = ClstrCounter + 1;
				}
				j = j + 1;
			} while (j < EnImgWidth);
			i = i + 1;
		} while (i < EnImgHeight);
		if (ClstrCounter < FinalMaritimeRegionSizeTr)
		{
			int i = 0;
			do
			{
				int* bwLabelsTablePntr = bwLabelsTable.ptr<int>(i);
				int* MaritimeframePntr = Maritimeframe.ptr<int>(i);
				int j = 0;
				do
				{
					if (bwLabelsTablePntr[j] == ClstrNr)
					{
						MaritimeframePntr[j] = 0;
					}
					j = j + 1;
				} while (j < EnImgWidth);
				i = i + 1;
			} while (i < EnImgHeight);
		}
		ClstrNr = ClstrNr + 1;
	} while (ClstrNr < NumberOfMaritimeRegions);

	MatrixNot(Maritimeframe, EnImgHeight, EnImgWidth);
	if (timersShowFlg == 1)
	{
		timerVrabl = ((double)getTickCount() - timerVrabl) / getTickFrequency();
		std::cout << endl << "MaritimeClusterSizeCheck: " << timerVrabl << " s" << std::endl;
	}
	return Maritimeframe;
}