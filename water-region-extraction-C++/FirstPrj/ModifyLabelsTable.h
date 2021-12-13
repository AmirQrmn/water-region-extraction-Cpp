#ifndef ModifyLabelsTable__
#define ModifyLabelsTable__

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

void ModifyLabelsTable(Mat &LabelsTable, int &LabelCntr, const int i, const int j, const int EnImgHeight, const int EnImgWidth, const int NeihgborID)
{
	int ProperIup;
	int ProperIdown;
	if (i == 0)
	{
		ProperIdown = 1;
	}
	else
	{
		ProperIdown = i;
	}
	if (i == EnImgHeight-1)
	{
		ProperIup = EnImgHeight - 2;
	}
	else
	{
		ProperIup = i;
	}

	int* LabelsTablePntr = LabelsTable.ptr<int>(i);
	int* nextLineLabelsTablePntr = LabelsTable.ptr<int>(ProperIup + 1);
	int* previousLineLabelsTablePntr = LabelsTable.ptr<int>(ProperIdown - 1);

	int FirstSide = LabelsTablePntr[j];
	int SecondSide;
	if (NeihgborID == 2)
		SecondSide = previousLineLabelsTablePntr[j];
	else if (NeihgborID == 4)
		SecondSide = LabelsTablePntr[j - 1];
	else if (NeihgborID == 5)
		SecondSide = LabelsTablePntr[j + 1];
	else if (NeihgborID == 6)
		SecondSide = nextLineLabelsTablePntr[j - 1];
	else if (NeihgborID == 7)
		SecondSide = nextLineLabelsTablePntr[j];
	else if (NeihgborID == 8)
		SecondSide = nextLineLabelsTablePntr[j + 1];

	int ModifiedLabel = min(FirstSide, SecondSide);
	int DiscardedLabel = max(FirstSide, SecondSide);
	LabelCntr = LabelCntr - 1;
	int t = 0;
	do
	{
		int* newLabelsTablePntr = LabelsTable.ptr<int>(t);
		int u = 0;
		do
		{
			if (newLabelsTablePntr[u] == DiscardedLabel)
				newLabelsTablePntr[u] = ModifiedLabel;

			if (newLabelsTablePntr[u] > DiscardedLabel)
				newLabelsTablePntr[u] = newLabelsTablePntr[u] - 1;
			u = u + 1;
		} while (u < EnImgWidth);
		t = t + 1;
	} while (t < EnImgHeight);
}

#endif