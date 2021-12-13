#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

void ClusterRgbPlot(Mat LabelsTable, const int EnImgHeight, const int EnImgWidth, const int NumOfRgbRegions, const int timersShowFlg)
{
	double timerVrabl = 0;
	if (timersShowFlg == 1)
	{
		timerVrabl = (double)getTickCount();
	}

	double NumOfRgbRegionsNew = double(NumOfRgbRegions);
	double RgBsTeP = 1.0 / pow(NumOfRgbRegionsNew, (1.0 / 3.0));
	double ClstrR = 3.0 * RgBsTeP;
	double ClstrG = 0.0;
	double ClstrB = 0.0;
	Mat ClusteredImg(EnImgHeight, EnImgWidth, CV_64FC3);
	int ClstrNr = 0;
	do
	{
		int RegionFlg = 0;
		int i = 0;
		do
		{
			int* LabelsTablePntr = LabelsTable.ptr<int>(i);
			double* rgbImgPntr = ClusteredImg.ptr<double>(i);
			int j = 0;
			do
			{
				if (LabelsTablePntr[j] == ClstrNr)
				{
					rgbImgPntr[3 * j] = ClstrR;
					rgbImgPntr[(3 * j) + 1] = ClstrG;
					rgbImgPntr[(3 * j) + 2] = ClstrB;
					RegionFlg = 1;
				}
				j = j + 1;
			} while (j < EnImgWidth);
			i = i + 1;
		} while (i < EnImgHeight);
		if (RegionFlg == 1)
		{
			ClstrR = ClstrR + RgBsTeP;
		}
		if (ClstrR > 1.0)
		{
			ClstrR = 0.0;
			ClstrG = ClstrG + RgBsTeP;
		}
		if (ClstrG > 1.0)
		{
			ClstrG = 0;
			ClstrB = ClstrB + RgBsTeP;
		}
		ClstrNr = ClstrNr + 1;
	} while (ClstrNr < NumOfRgbRegions - 1);           // az -1 motmaen nistam...!!! badam check kon. to matlab in bood...
	imshow("Clustered Image", ClusteredImg);

	if (timersShowFlg == 1)
	{
		timerVrabl = ((double)getTickCount() - timerVrabl) / getTickFrequency();
		std::cout << endl << "SeqMaker: " << timerVrabl << " s" << std::endl;
	}
}