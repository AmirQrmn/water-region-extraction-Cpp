#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

void MatrixNot(Mat &Maritimeframe, const int EnImgHeight, const int EnImgWidth)
{
	int i = 0;
	do
	{
		int* MaritimeframePntr = Maritimeframe.ptr<int>(i);
		int j = 0;
		do
		{
			int doneFlg = 0;
			if (MaritimeframePntr[j] == 0)
			{
				MaritimeframePntr[j] = 1;
				doneFlg = 1;
			}
			if (doneFlg == 0)
			{
				if (MaritimeframePntr[j] == 1)
				{
					MaritimeframePntr[j] = 0;
				}
			}
			j = j + 1;
		} while (j < EnImgWidth);
		i = i + 1;
	} while (i < EnImgHeight);
}