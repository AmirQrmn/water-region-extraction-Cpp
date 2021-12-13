#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

Mat MaritimeClusterIdentificationCriteria(Mat msSmoothedEnergyFrames, cv::Mat LabelsTable, const int EnImgHeight, const int EnImgWidth, const double NrmFlickerSumTr, const double NrmStaticSumUpTr, const double NrmStaticSumDownTr, const int NumOfRgbRegions, const int FlickerAmpFlg, const int timersShowFlg)
{
	double timerVrabl = 0;
	if (timersShowFlg == 1)
	{
		timerVrabl = (double)getTickCount();
	}
	Mat Maritimeframe(EnImgHeight, EnImgWidth, CV_32S, cv::Scalar(0));
	Mat HorStaticSmoothedEnFrame = msSmoothedEnergyFrames(cv::Rect_<double>(0, 0, EnImgWidth, EnImgHeight));
	Mat VerStaticSmoothedEnFrame = msSmoothedEnergyFrames(cv::Rect_<double>(0, EnImgHeight, EnImgWidth, EnImgHeight));
	Mat StaticSmoothedEnFrame = HorStaticSmoothedEnFrame + VerStaticSmoothedEnFrame;
	Mat FlickerFrame = msSmoothedEnergyFrames(cv::Rect_<double>(0, (2 * EnImgHeight), EnImgWidth, EnImgHeight));
	if (FlickerAmpFlg == 1)
	{
		FlickerFrame(cv::Rect_<double>(0, (int(2 * (EnImgHeight / 3))) - 5, EnImgWidth, (int(1 * (EnImgHeight / 3))))) = 2.0 * (FlickerFrame(cv::Rect_<double>(0, (int(2 * (EnImgHeight / 3))) - 5, EnImgWidth, (int(1 * (EnImgHeight / 3))))));
		//FlickerFrame(cv::Rect_<double>(0, 0, EnImgWidth, (int(2 * (EnImgHeight / 3))))) = 2.0 * (FlickerFrame(cv::Rect_<double>(0, 0, EnImgWidth, (int(2 * (EnImgHeight / 3))))));
		/*int i = 0;
		do
		{
		double* FlickerFramePntr = FlickerFrame.ptr<double>(i);
		int j = 0;
		do
		{
		FlickerFramePntr[j] = 2.0 * (FlickerFramePntr[j]);
		j = j + 1;
		} while (j < EnImgWidth);
		i = i + 1;
		} while (i < (int(2 * (EnImgHeight / 3))));*/
	}
	int ClstrCntr = 0;
	do
	{
		double FlickerBin = 0;
		double HorStaticBin = 0;
		double VerStatisBin = 0;
		double StaticBin = 0;
		double CmstrMemberCntr = 0;
		int i = 0;
		do
		{
			int* LabelsTablePntr = LabelsTable.ptr<int>(i);
			double* FlickerFramePntr = FlickerFrame.ptr<double>(i);
			double* HorStaticSmoothedEnFramePntr = HorStaticSmoothedEnFrame.ptr<double>(i);
			double* VerStaticSmoothedEnFramePntr = VerStaticSmoothedEnFrame.ptr<double>(i);
			double* StaticSmoothedEnFramePntr = StaticSmoothedEnFrame.ptr<double>(i);
			int j = 0;
			do
			{
				if (LabelsTablePntr[j] == ClstrCntr)
				{
					CmstrMemberCntr = CmstrMemberCntr + 1.0;
					FlickerBin = FlickerBin + FlickerFramePntr[j];
					HorStaticBin = HorStaticBin + HorStaticSmoothedEnFramePntr[j];
					VerStatisBin = VerStatisBin + VerStaticSmoothedEnFramePntr[j];
					StaticBin = StaticBin + StaticSmoothedEnFramePntr[j];
				}
				j = j + 1;
			} while (j < EnImgWidth);
			i = i + 1;
		} while (i < EnImgHeight);
		if (CmstrMemberCntr != 0.0)
		{
			FlickerBin = FlickerBin / CmstrMemberCntr;
			HorStaticBin = HorStaticBin / CmstrMemberCntr;
			VerStatisBin = VerStatisBin / CmstrMemberCntr;
			StaticBin = StaticBin / CmstrMemberCntr;
		}
		int backgrFlg = 0;
		if (HorStaticBin > 0.1)
		{
			if (VerStatisBin > 0.1)
			{
				backgrFlg = 1;
			}
		}
		if (CmstrMemberCntr != 0.0)
		{
			if (backgrFlg == 0)
			{
				int i = 0;
				do
				{
					int* LabelsTablePntr = LabelsTable.ptr<int>(i);
					int* MaritimeframePntr = Maritimeframe.ptr<int>(i);
					int j = 0;
					do
					{
						if (LabelsTablePntr[j] == ClstrCntr)
						{
							if (FlickerBin > NrmFlickerSumTr)
							{
								if (StaticBin < NrmStaticSumUpTr)
								{
									if (StaticBin > NrmStaticSumDownTr)
									{
										int d = 0;
										do
										{
											MaritimeframePntr[j] = 1;
											d = d + 1;
										} while (d < CmstrMemberCntr);
									}
								}
							}
						}
						j = j + 1;
					} while (j < EnImgWidth);
					i = i + 1;
				} while (i < EnImgHeight);
			}
		}
		ClstrCntr = ClstrCntr + 1;
	} while (ClstrCntr < NumOfRgbRegions);
	if (timersShowFlg == 1)
	{
		timerVrabl = ((double)getTickCount() - timerVrabl) / getTickFrequency();
		std::cout << endl << "MaritimeClusterIdentificationCriteria: " << timerVrabl << " s" << std::endl;
	}
	return Maritimeframe;
}