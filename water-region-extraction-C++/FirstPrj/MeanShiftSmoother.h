#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>
#include "BhattacharyyaCoefCalculator.h"
#include <stdio.h>

using namespace cv;
using namespace std;

Mat MeanShiftSmoother(Mat matEnergyHolder, const int EnImgHeight, const int EnImgWidth, const int NumberOfOrientations, const int depth, const int msI, const int msJ, const int msK, const double msBhtSimilarityTr, const double msHistSimTr, const int msItrMax, const int meanSshiftRowNumPrint, const int timersShowFlg)
{
	double timerVrabl = 0;
	if (timersShowFlg == 1)
	{
		timerVrabl = (double)getTickCount();
	}
	
	Mat msSmoothedEnergyFrames{ (NumberOfOrientations*EnImgHeight), EnImgWidth, CV_64F, Scalar(0) };
	Mat matEnergyHolder_temp{ (NumberOfOrientations*EnImgHeight), EnImgWidth, CV_64F, Scalar(0) };
	int q = 0;
	do
	{
		matEnergyHolder(cv::Rect_<double>(0, ((msK)*(NumberOfOrientations*EnImgHeight)), EnImgWidth, (NumberOfOrientations*EnImgHeight))).copyTo(matEnergyHolder_temp(cv::Rect_<double>(0, 0, EnImgWidth, (NumberOfOrientations*EnImgHeight))));
		q = q + 1;
	} while (q < NumberOfOrientations);
	Mat SimilarityFlags = { EnImgHeight, EnImgWidth, CV_64F, Scalar(0) };
	int i1;
	int i2;
	int j1;
	int j2;
	int itr = 0;
	do
	{
		double min, max;
		cv::minMaxLoc(SimilarityFlags, &min, &max);
		if (min == 0)
		{
			int i = 0;
			do
			{
				double* SimFlgPntr = SimilarityFlags.ptr<double>(i);
				if (meanSshiftRowNumPrint == 1)
				{
					cout << itr << " , " << i << endl;
				}
				int j = 0;
				do
				{
					if (SimFlgPntr[j] == 0)
					{
						//--------------------------------------------------------------
						if ((i - msI) < 0)
						{
							i1 = 0;
						}
						else if (i - msI >= 0)
						{
							i1 = i - msI;
						}
						//--------------------------------------------------------------
						if (i + msI > EnImgHeight - 1)
						{
							i2 = EnImgHeight - 1;
						}
						else if (i + msI <= EnImgHeight - 1)
						{
							i2 = i + msI;
						}
						//--------------------------------------------------------------	
						if (j - msJ < 0)
						{
							j1 = 0;
						}
						else if (j - msJ >= 0)
						{
							j1 = j - msJ;
						}
						//--------------------------------------------------------------	
						if (j + msJ > EnImgWidth - 1)
						{
							j2 = EnImgWidth - 1;
						}
						else if (j + msJ <= EnImgWidth - 1)
						{
							j2 = j + msJ;
						}
						//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++	
						Mat MainHist = { 1, NumberOfOrientations, CV_64F, Scalar(0) };
						Mat MeanHist = { 1, NumberOfOrientations, CV_64F, Scalar(0) };
						double SelectedNegbrNr = 0.0;
						int q = 0;
						double* MainHistPntr = MainHist.ptr<double>(0);
						double* MeanHistPntr = MeanHist.ptr<double>(0);
						do
						{
							double* matEnHldrForMainHistPntr = matEnergyHolder.ptr<double>(((msK)*(NumberOfOrientations*EnImgHeight)) + i + (q*EnImgHeight));
							MainHistPntr[q] = matEnHldrForMainHistPntr[j];
							q = q + 1;
						} while (q < NumberOfOrientations);

						int kIndx = 0;
						do
						{
							int iIndx = i1;
							do
							{
								int jIndx = j1;
								do
								{
									Mat NegbrHist = { 1, NumberOfOrientations, CV_64F, Scalar(0) };
									Mat tempBht = { 1, NumberOfOrientations, CV_64F, Scalar(0) };
									int q = 0;
									double* NghbrHistPntr = NegbrHist.ptr<double>(0);
									do
									{
										double* matEnHldrForNghbrHistPntr = matEnergyHolder.ptr<double>(((kIndx)*(NumberOfOrientations*EnImgHeight)) + iIndx + (q*EnImgHeight));
										NghbrHistPntr[q] = matEnHldrForNghbrHistPntr[jIndx];
										q = q + 1;
									} while (q < NumberOfOrientations);
									double BhtRes = BhattacharyyaCoefCalculator(MainHist, NegbrHist);
									if (BhtRes >= msBhtSimilarityTr)
									{
										MeanHist = MeanHist + NegbrHist;
										SelectedNegbrNr = SelectedNegbrNr + 1.0;
									}
									jIndx = jIndx + 1;
								} while (jIndx < j2 + 1);
								iIndx = iIndx + 1;
							} while (iIndx < i2 + 1);
							kIndx = kIndx + 1;
						} while (kIndx < (2 * msK) + 1);
						if (SelectedNegbrNr > 0)
						{
							MeanHist = MeanHist / SelectedNegbrNr;
						}
						q = 0;
						do
						{
							double* TempMatEnHldrForMainHistPntr = matEnergyHolder_temp.ptr<double>(i + (q*EnImgHeight));
							TempMatEnHldrForMainHistPntr[j] = MeanHistPntr[q];
							q = q + 1;
						} while (q < NumberOfOrientations);
						if (SelectedNegbrNr > 0)		// in sharto gozashtam ke mogheei ke MeanHist sefr hast alaki vaght talaf nakone ba mohasebate bhat.
						{
							double MeanBhtRes = BhattacharyyaCoefCalculator(MainHist, MeanHist);
							if (MeanBhtRes >= msHistSimTr)
							{
								SimFlgPntr[j] = 1;
							}
						}
					}
					j = j + 1;
				} while (j < EnImgWidth);
				i = i + 1;
			} while (i < EnImgHeight);
		}
		matEnergyHolder_temp.copyTo(matEnergyHolder(cv::Rect(0, ((msK)*(EnImgHeight*NumberOfOrientations)), EnImgWidth, (NumberOfOrientations*EnImgHeight))));
		itr = itr + 1;
	} while (itr < msItrMax);
	msSmoothedEnergyFrames = matEnergyHolder(cv::Rect(0, ((msK)*(EnImgHeight*NumberOfOrientations)), EnImgWidth, (NumberOfOrientations*EnImgHeight)));	
	if (timersShowFlg == 1)
	{
		timerVrabl = ((double)getTickCount() - timerVrabl) / getTickFrequency();
		std::cout << endl << "MeanShiftSmoother: " << timerVrabl << " s" << std::endl;
	}
	return msSmoothedEnergyFrames;
}