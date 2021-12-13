#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

Mat EnergyBasicFilteringCalculator(const std::vector<cv::Mat> inputImgsForEnCalcPlanes, const int hgt, const int wdth, const int depth, const int MyFilterFlg, const int origImgWidth, const int origImgHeight, const int EnImgHeight, const int EnImgWidth, const int timersShowFlg)
{
	double timerVrabl = 0;
	if (timersShowFlg == 1)
	{
		timerVrabl = (double)getTickCount();
	}
	Mat ImgEnBasicPlanes((16 * EnImgHeight), EnImgWidth, CV_64F, cv::Scalar(0));      // 6 G2 and 10 H2 basic filtering outputs are stored in this Mat.
	{
		//in "G2a.at<double>(row,col)", first var inside the paranthesis is row and second one is col. But, when we ask for "G2a.Size()", we receive [cols * rows]! we can also use "G2a.rows" and "G2a.cols"!
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		double Ng = 0.41146;
		double Nh = 0.877776;
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		cv::Mat G2a((((2 * depth) + 1) * ((2 * hgt) + 1)), ((2 * wdth) + 1), CV_64F, cv::Scalar(0));
		int k = 0;
		do
		{
			int i = 0;
			do
			{
				double* G2aPntr = G2a.ptr<double>(i + (k*((2 * depth) + 1)));
				int j = 0;
				do
				{
					G2aPntr[j] = cv::saturate_cast<double> (Ng*((2 * pow((i - hgt), 2)) - 1) * exp(-(pow((i - hgt), 2) + pow((j - wdth), 2) + pow((k - depth), 2))));
					j = j + 1;
				} while (j < ((2 * wdth) + 1));
				i = i + 1;
			} while (i < ((2 * hgt) + 1));
			k = k + 1;
		} while (k < ((2 * depth) + 1));
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		cv::Mat G2b((((2 * depth) + 1) * ((2 * hgt) + 1)), ((2 * wdth) + 1), CV_64F, cv::Scalar(0));
		k = 0;
		do
		{
			int i = 0;
			do
			{
				double* G2bPntr = G2b.ptr<double>(i + (k*((2 * depth) + 1)));
				int j = 0;
				do
				{
					G2bPntr[j] = cv::saturate_cast<double> (Ng * 2 * (i - hgt)*(j - wdth)*exp(-(pow((i - hgt), 2) + pow((j - wdth), 2) + pow((k - depth), 2))));
					j = j + 1;
				} while (j < ((2 * wdth) + 1));
				i = i + 1;
			} while (i < ((2 * hgt) + 1));
			k = k + 1;
		} while (k < ((2 * depth) + 1));
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		cv::Mat G2c((((2 * depth) + 1) * ((2 * hgt) + 1)), ((2 * wdth) + 1), CV_64F, cv::Scalar(0));
		k = 0;
		do
		{
			int i = 0;
			do
			{
				double* G2cPntr = G2c.ptr<double>(i + (k*((2 * depth) + 1)));
				int j = 0;
				do
				{
					G2cPntr[j] = cv::saturate_cast<double> (Ng*((2 * pow((j - wdth), 2)) - 1)*exp(-(pow((i - hgt), 2) + pow((j - wdth), 2) + pow((k - depth), 2))));
					j = j + 1;
				} while (j < ((2 * wdth) + 1));
				i = i + 1;
			} while (i < ((2 * hgt) + 1));
			k = k + 1;
		} while (k < ((2 * depth) + 1));
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		cv::Mat G2d((((2 * depth) + 1) * ((2 * hgt) + 1)), ((2 * wdth) + 1), CV_64F, cv::Scalar(0));
		k = 0;
		do
		{
			int i = 0;
			do
			{
				double* G2dPntr = G2d.ptr<double>(i + (k*((2 * depth) + 1)));
				int j = 0;
				do
				{
					G2dPntr[j] = cv::saturate_cast<double> (Ng * 2 * (i - hgt)*(k - depth)*exp(-(pow((i - hgt), 2) + pow((j - wdth), 2) + pow((k - depth), 2))));
					j = j + 1;
				} while (j < ((2 * wdth) + 1));
				i = i + 1;
			} while (i < ((2 * hgt) + 1));
			k = k + 1;
		} while (k < ((2 * depth) + 1));
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		cv::Mat G2e((((2 * depth) + 1) * ((2 * hgt) + 1)), ((2 * wdth) + 1), CV_64F, cv::Scalar(0));
		k = 0;
		do
		{
			int i = 0;
			do
			{
				double* G2ePntr = G2e.ptr<double>(i + (k*((2 * depth) + 1)));
				int j = 0;
				do
				{
					G2ePntr[j] = cv::saturate_cast<double> (Ng * 2 * (j - wdth)*(k - depth)*exp(-(pow((i - hgt), 2) + pow((j - wdth), 2) + pow((k - depth), 2))));
					j = j + 1;
				} while (j < ((2 * wdth) + 1));
				i = i + 1;
			} while (i < ((2 * hgt) + 1));
			k = k + 1;
		} while (k < ((2 * depth) + 1));
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		cv::Mat G2f((((2 * depth) + 1) * ((2 * hgt) + 1)), ((2 * wdth) + 1), CV_64F, cv::Scalar(0));
		k = 0;
		do
		{
			int i = 0;
			do
			{
				double* G2fPntr = G2f.ptr<double>(i + (k*((2 * depth) + 1)));
				int j = 0;
				do
				{
					G2fPntr[j] = cv::saturate_cast<double> (Ng*((2 * pow((k - depth), 2)) - 1)*exp(-(pow((i - hgt), 2) + pow((j - wdth), 2) + pow((k - depth), 2))));
					j = j + 1;
				} while (j < ((2 * wdth) + 1));
				i = i + 1;
			} while (i < ((2 * hgt) + 1));
			k = k + 1;
		} while (k < ((2 * depth) + 1));
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		cv::Mat H2a((((2 * depth) + 1) * ((2 * hgt) + 1)), ((2 * wdth) + 1), CV_64F, cv::Scalar(0));
		k = 0;
		do
		{
			int i = 0;
			do
			{
				double* H2aPntr = H2a.ptr<double>(i + (k*((2 * depth) + 1)));
				int j = 0;
				do
				{
					H2aPntr[j] = cv::saturate_cast<double> (Nh*((pow((i - hgt), 3)) - (2.254*(i - hgt)))*exp(-(pow((i - hgt), 2) + pow((j - wdth), 2) + pow((k - depth), 2))));
					j = j + 1;
				} while (j < ((2 * wdth) + 1));
				i = i + 1;
			} while (i < ((2 * hgt) + 1));
			k = k + 1;
		} while (k < ((2 * depth) + 1));
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		cv::Mat H2b((((2 * depth) + 1) * ((2 * hgt) + 1)), ((2 * wdth) + 1), CV_64F, cv::Scalar(0));
		k = 0;
		do
		{
			int i = 0;
			do
			{
				double* H2bPntr = H2b.ptr<double>(i + (k*((2 * depth) + 1)));
				int j = 0;
				do
				{
					H2bPntr[j] = cv::saturate_cast<double> (Nh*((pow((i - hgt), 2)) - 0.751333)*(j - wdth)*exp(-(pow((i - hgt), 2) + pow((j - wdth), 2) + pow((k - depth), 2))));
					j = j + 1;
				} while (j < ((2 * wdth) + 1));
				i = i + 1;
			} while (i < ((2 * hgt) + 1));
			k = k + 1;
		} while (k < ((2 * depth) + 1));
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		cv::Mat H2c((((2 * depth) + 1) * ((2 * hgt) + 1)), ((2 * wdth) + 1), CV_64F, cv::Scalar(0));
		k = 0;
		do
		{
			int i = 0;
			do
			{
				double* H2cPntr = H2c.ptr<double>(i + (k*((2 * depth) + 1)));
				int j = 0;
				do
				{
					H2cPntr[j] = cv::saturate_cast<double> (Nh*((pow((j - wdth), 2)) - 0.751333)*(i - hgt)*exp(-(pow((i - hgt), 2) + pow((j - wdth), 2) + pow((k - depth), 2))));
					j = j + 1;
				} while (j < ((2 * wdth) + 1));
				i = i + 1;
			} while (i < ((2 * hgt) + 1));
			k = k + 1;
		} while (k < ((2 * depth) + 1));
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		cv::Mat H2d((((2 * depth) + 1) * ((2 * hgt) + 1)), ((2 * wdth) + 1), CV_64F, cv::Scalar(0));
		k = 0;
		do
		{
			int i = 0;
			do
			{
				double* H2dPntr = H2d.ptr<double>(i + (k*((2 * depth) + 1)));
				int j = 0;
				do
				{
					H2dPntr[j] = cv::saturate_cast<double> (Nh*((pow((j - wdth), 3)) - 2.254*(j - wdth))*exp(-(pow((i - hgt), 2) + pow((j - wdth), 2) + pow((k - depth), 2))));
					j = j + 1;
				} while (j < ((2 * wdth) + 1));
				i = i + 1;
			} while (i < ((2 * hgt) + 1));
			k = k + 1;
		} while (k < ((2 * depth) + 1));
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		cv::Mat H2e((((2 * depth) + 1) * ((2 * hgt) + 1)), ((2 * wdth) + 1), CV_64F, cv::Scalar(0));
		k = 0;
		do
		{
			int i = 0;
			do
			{
				double* H2ePntr = H2e.ptr<double>(i + (k*((2 * depth) + 1)));
				int j = 0;
				do
				{
					H2ePntr[j] = cv::saturate_cast<double> (Nh*((pow((i - hgt), 2)) - 0.751333)*(k - depth)*exp(-(pow((i - hgt), 2) + pow((j - wdth), 2) + pow((k - depth), 2))));
					j = j + 1;
				} while (j < ((2 * wdth) + 1));
				i = i + 1;
			} while (i < ((2 * hgt) + 1));
			k = k + 1;
		} while (k < ((2 * depth) + 1));
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		cv::Mat H2f((((2 * depth) + 1) * ((2 * hgt) + 1)), ((2 * wdth) + 1), CV_64F, cv::Scalar(0));
		k = 0;
		do
		{
			int i = 0;
			do
			{
				double* H2fPntr = H2f.ptr<double>(i + (k*((2 * depth) + 1)));
				int j = 0;
				do
				{
					H2fPntr[j] = cv::saturate_cast<double> (Nh*(i - hgt)*(j - wdth)*(k - depth)*exp(-(pow((i - hgt), 2) + pow((j - wdth), 2) + pow((k - depth), 2))));
					j = j + 1;
				} while (j < ((2 * wdth) + 1));
				i = i + 1;
			} while (i < ((2 * hgt) + 1));
			k = k + 1;
		} while (k < ((2 * depth) + 1));
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		cv::Mat H2g((((2 * depth) + 1) * ((2 * hgt) + 1)), ((2 * wdth) + 1), CV_64F, cv::Scalar(0));
		k = 0;
		do
		{
			int i = 0;
			do
			{
				double* H2gPntr = H2g.ptr<double>(i + (k*((2 * depth) + 1)));
				int j = 0;
				do
				{
					H2gPntr[j] = cv::saturate_cast<double> (Nh*((pow((j - wdth), 2)) - 0.751333)*(k - depth)*exp(-(pow((i - hgt), 2) + pow((j - wdth), 2) + pow((k - depth), 2))));
					j = j + 1;
				} while (j < ((2 * wdth) + 1));
				i = i + 1;
			} while (i < ((2 * hgt) + 1));
			k = k + 1;
		} while (k < ((2 * depth) + 1));
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		cv::Mat H2h((((2 * depth) + 1) * ((2 * hgt) + 1)), ((2 * wdth) + 1), CV_64F, cv::Scalar(0));
		k = 0;
		do
		{
			int i = 0;
			do
			{
				double* H2hPntr = H2h.ptr<double>(i + (k*((2 * depth) + 1)));
				int j = 0;
				do
				{
					H2hPntr[j] = cv::saturate_cast<double> (Nh*((pow((k - depth), 2)) - 0.751333)*(i - hgt)*exp(-(pow((i - hgt), 2) + pow((j - wdth), 2) + pow((k - depth), 2))));
					j = j + 1;
				} while (j < ((2 * wdth) + 1));
				i = i + 1;
			} while (i < ((2 * hgt) + 1));
			k = k + 1;
		} while (k < ((2 * depth) + 1));
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		cv::Mat H2i((((2 * depth) + 1) * ((2 * hgt) + 1)), ((2 * wdth) + 1), CV_64F, cv::Scalar(0));
		k = 0;
		do
		{
			int i = 0;
			do
			{
				double* H2iPntr = H2i.ptr<double>(i + (k*((2 * depth) + 1)));
				int j = 0;
				do
				{
					H2iPntr[j] = cv::saturate_cast<double> (Nh*((pow((k - depth), 2)) - 0.751333)*(j - wdth)*exp(-(pow((i - hgt), 2) + pow((j - wdth), 2) + pow((k - depth), 2))));
					j = j + 1;
				} while (j < ((2 * wdth) + 1));
				i = i + 1;
			} while (i < ((2 * hgt) + 1));
			k = k + 1;
		} while (k < ((2 * depth) + 1));
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		cv::Mat H2j((((2 * depth) + 1) * ((2 * hgt) + 1)), ((2 * wdth) + 1), CV_64F, cv::Scalar(0));
		k = 0;
		do
		{
			int i = 0;
			do
			{
				double* H2jPntr = H2j.ptr<double>(i + (k*((2 * depth) + 1)));
				int j = 0;
				do
				{
					H2jPntr[j] = cv::saturate_cast<double> (Nh*((pow((k - depth), 3)) - (2.254*(k - depth)))*exp(-(pow((i - hgt), 2) + pow((j - wdth), 2) + pow((k - depth), 2))));
					j = j + 1;
				} while (j < ((2 * wdth) + 1));
				i = i + 1;
			} while (i < ((2 * hgt) + 1));
			k = k + 1;
		} while (k < ((2 * depth) + 1));
		//*******************************************************************************************************************************************************************************************************************************
		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		{
			int EnPlnNmr = 0;
			Mat inputImgSeqPln(origImgHeight*((2 * depth) + 1), origImgWidth, CV_64F, cv::Scalar(0));
			int FrmCntr = 0;
			do
			{
				inputImgsForEnCalcPlanes[FrmCntr].copyTo(inputImgSeqPln(cv::Rect_<double>(0, (FrmCntr*origImgHeight), origImgWidth, origImgHeight)));
				FrmCntr = FrmCntr + 1;
			} while (FrmCntr < ((2 * depth) + 1));
			//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			{
				cv::Mat G2Output(origImgHeight, origImgWidth, CV_64F, cv::Scalar(0));
				cv::Mat tempContainer(origImgHeight, origImgWidth, CV_64F, cv::Scalar(0));
				int fltrIndx = 0;
				do
				{
					cv::filter2D(inputImgSeqPln(cv::Rect_<double>(0, (fltrIndx*origImgHeight), origImgWidth, origImgHeight)), tempContainer, CV_64F, G2a(cv::Rect_<double>(0, (fltrIndx*((2 * depth) + 1)), ((2 * wdth) + 1), ((2 * hgt) + 1))));
					G2Output = G2Output + tempContainer;
					fltrIndx = fltrIndx + 1;
				} while (fltrIndx < ((2 * depth) + 1));
				G2Output(cv::Rect(wdth, hgt, EnImgWidth, EnImgHeight)).copyTo(ImgEnBasicPlanes(cv::Rect(0, (EnPlnNmr * EnImgHeight), EnImgWidth, EnImgHeight)));
				EnPlnNmr = EnPlnNmr + 1;
			}
			//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			{
				cv::Mat G2Output(origImgHeight, origImgWidth, CV_64F, cv::Scalar(0));
				cv::Mat tempContainer(origImgHeight, origImgWidth, CV_64F, cv::Scalar(0));
				int fltrIndx = 0;
				do
				{
					cv::filter2D(inputImgSeqPln(cv::Rect_<double>(0, (fltrIndx*origImgHeight), origImgWidth, origImgHeight)), tempContainer, CV_64F, G2b(cv::Rect_<double>(0, (fltrIndx*((2 * depth) + 1)), ((2 * wdth) + 1), ((2 * hgt) + 1))));
					G2Output = G2Output + tempContainer;
					fltrIndx = fltrIndx + 1;
				} while (fltrIndx < ((2 * depth) + 1));
				G2Output(cv::Rect(wdth, hgt, EnImgWidth, EnImgHeight)).copyTo(ImgEnBasicPlanes(cv::Rect(0, (EnPlnNmr * EnImgHeight), EnImgWidth, EnImgHeight)));
				EnPlnNmr = EnPlnNmr + 1;
			}
			//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			{
				cv::Mat G2Output(origImgHeight, origImgWidth, CV_64F, cv::Scalar(0));
				cv::Mat tempContainer(origImgHeight, origImgWidth, CV_64F, cv::Scalar(0));
				int fltrIndx = 0;
				do
				{
					cv::filter2D(inputImgSeqPln(cv::Rect_<double>(0, (fltrIndx*origImgHeight), origImgWidth, origImgHeight)), tempContainer, CV_64F, G2c(cv::Rect_<double>(0, (fltrIndx*((2 * depth) + 1)), ((2 * wdth) + 1), ((2 * hgt) + 1))));
					G2Output = G2Output + tempContainer;
					fltrIndx = fltrIndx + 1;
				} while (fltrIndx < ((2 * depth) + 1));
				G2Output(cv::Rect(wdth, hgt, EnImgWidth, EnImgHeight)).copyTo(ImgEnBasicPlanes(cv::Rect(0, (EnPlnNmr * EnImgHeight), EnImgWidth, EnImgHeight)));
				EnPlnNmr = EnPlnNmr + 1;
			}
			//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			{
				cv::Mat G2Output(origImgHeight, origImgWidth, CV_64F, cv::Scalar(0));
				cv::Mat tempContainer(origImgHeight, origImgWidth, CV_64F, cv::Scalar(0));
				int fltrIndx = 0;
				do
				{
					cv::filter2D(inputImgSeqPln(cv::Rect_<double>(0, (fltrIndx*origImgHeight), origImgWidth, origImgHeight)), tempContainer, CV_64F, G2d(cv::Rect_<double>(0, (fltrIndx*((2 * depth) + 1)), ((2 * wdth) + 1), ((2 * hgt) + 1))));
					G2Output = G2Output + tempContainer;
					fltrIndx = fltrIndx + 1;
				} while (fltrIndx < ((2 * depth) + 1));
				G2Output(cv::Rect(wdth, hgt, EnImgWidth, EnImgHeight)).copyTo(ImgEnBasicPlanes(cv::Rect(0, (EnPlnNmr * EnImgHeight), EnImgWidth, EnImgHeight)));
				EnPlnNmr = EnPlnNmr + 1;
			}
			//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			{
				cv::Mat G2Output(origImgHeight, origImgWidth, CV_64F, cv::Scalar(0));
				cv::Mat tempContainer(origImgHeight, origImgWidth, CV_64F, cv::Scalar(0));
				int fltrIndx = 0;
				do
				{
					cv::filter2D(inputImgSeqPln(cv::Rect_<double>(0, (fltrIndx*origImgHeight), origImgWidth, origImgHeight)), tempContainer, CV_64F, G2e(cv::Rect_<double>(0, (fltrIndx*((2 * depth) + 1)), ((2 * wdth) + 1), ((2 * hgt) + 1))));
					G2Output = G2Output + tempContainer;
					fltrIndx = fltrIndx + 1;
				} while (fltrIndx < ((2 * depth) + 1));
				G2Output(cv::Rect(wdth, hgt, EnImgWidth, EnImgHeight)).copyTo(ImgEnBasicPlanes(cv::Rect(0, (EnPlnNmr * EnImgHeight), EnImgWidth, EnImgHeight)));
				EnPlnNmr = EnPlnNmr + 1;
			}
			//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			{
				cv::Mat G2Output(origImgHeight, origImgWidth, CV_64F, cv::Scalar(0));
				cv::Mat tempContainer(origImgHeight, origImgWidth, CV_64F, cv::Scalar(0));
				int fltrIndx = 0;
				do
				{
					cv::filter2D(inputImgSeqPln(cv::Rect_<double>(0, (fltrIndx*origImgHeight), origImgWidth, origImgHeight)), tempContainer, CV_64F, G2f(cv::Rect_<double>(0, (fltrIndx*((2 * depth) + 1)), ((2 * wdth) + 1), ((2 * hgt) + 1))));
					G2Output = G2Output + tempContainer;
					fltrIndx = fltrIndx + 1;
				} while (fltrIndx < ((2 * depth) + 1));
				G2Output(cv::Rect(wdth, hgt, EnImgWidth, EnImgHeight)).copyTo(ImgEnBasicPlanes(cv::Rect(0, (EnPlnNmr * EnImgHeight), EnImgWidth, EnImgHeight)));
				EnPlnNmr = EnPlnNmr + 1;
			}
			//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			{
				cv::Mat H2Output(origImgHeight, origImgWidth, CV_64F, cv::Scalar(0));
				cv::Mat tempContainer(origImgHeight, origImgWidth, CV_64F, cv::Scalar(0));
				int fltrIndx = 0;
				do
				{
					cv::filter2D(inputImgSeqPln(cv::Rect_<double>(0, (fltrIndx*origImgHeight), origImgWidth, origImgHeight)), tempContainer, CV_64F, H2a(cv::Rect_<double>(0, (fltrIndx*((2 * depth) + 1)), ((2 * wdth) + 1), ((2 * hgt) + 1))));
					H2Output = H2Output + tempContainer;
					fltrIndx = fltrIndx + 1;
				} while (fltrIndx < ((2 * depth) + 1));
				H2Output(cv::Rect(wdth, hgt, EnImgWidth, EnImgHeight)).copyTo(ImgEnBasicPlanes(cv::Rect(0, (EnPlnNmr * EnImgHeight), EnImgWidth, EnImgHeight)));
				EnPlnNmr = EnPlnNmr + 1;
			}
			//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			{
				cv::Mat H2Output(origImgHeight, origImgWidth, CV_64F, cv::Scalar(0));
				cv::Mat tempContainer(origImgHeight, origImgWidth, CV_64F, cv::Scalar(0));
				int fltrIndx = 0;
				do
				{
					cv::filter2D(inputImgSeqPln(cv::Rect_<double>(0, (fltrIndx*origImgHeight), origImgWidth, origImgHeight)), tempContainer, CV_64F, H2b(cv::Rect_<double>(0, (fltrIndx*((2 * depth) + 1)), ((2 * wdth) + 1), ((2 * hgt) + 1))));
					H2Output = H2Output + tempContainer;
					fltrIndx = fltrIndx + 1;
				} while (fltrIndx < ((2 * depth) + 1));
				H2Output(cv::Rect(wdth, hgt, EnImgWidth, EnImgHeight)).copyTo(ImgEnBasicPlanes(cv::Rect(0, (EnPlnNmr * EnImgHeight), EnImgWidth, EnImgHeight)));
				EnPlnNmr = EnPlnNmr + 1;
			}
			//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			{
				cv::Mat H2Output(origImgHeight, origImgWidth, CV_64F, cv::Scalar(0));
				cv::Mat tempContainer(origImgHeight, origImgWidth, CV_64F, cv::Scalar(0));
				int fltrIndx = 0;
				do
				{
					cv::filter2D(inputImgSeqPln(cv::Rect_<double>(0, (fltrIndx*origImgHeight), origImgWidth, origImgHeight)), tempContainer, CV_64F, H2c(cv::Rect_<double>(0, (fltrIndx*((2 * depth) + 1)), ((2 * wdth) + 1), ((2 * hgt) + 1))));
					H2Output = H2Output + tempContainer;
					fltrIndx = fltrIndx + 1;
				} while (fltrIndx < ((2 * depth) + 1));
				H2Output(cv::Rect(wdth, hgt, EnImgWidth, EnImgHeight)).copyTo(ImgEnBasicPlanes(cv::Rect(0, (EnPlnNmr * EnImgHeight), EnImgWidth, EnImgHeight)));
				EnPlnNmr = EnPlnNmr + 1;
			}
			//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			{
				cv::Mat H2Output(origImgHeight, origImgWidth, CV_64F, cv::Scalar(0));
				cv::Mat tempContainer(origImgHeight, origImgWidth, CV_64F, cv::Scalar(0));
				int fltrIndx = 0;
				do
				{
					cv::filter2D(inputImgSeqPln(cv::Rect_<double>(0, (fltrIndx*origImgHeight), origImgWidth, origImgHeight)), tempContainer, CV_64F, H2d(cv::Rect_<double>(0, (fltrIndx*((2 * depth) + 1)), ((2 * wdth) + 1), ((2 * hgt) + 1))));
					H2Output = H2Output + tempContainer;
					fltrIndx = fltrIndx + 1;
				} while (fltrIndx < ((2 * depth) + 1));
				H2Output(cv::Rect(wdth, hgt, EnImgWidth, EnImgHeight)).copyTo(ImgEnBasicPlanes(cv::Rect(0, (EnPlnNmr * EnImgHeight), EnImgWidth, EnImgHeight)));
				EnPlnNmr = EnPlnNmr + 1;
			}
			//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			{
				cv::Mat H2Output(origImgHeight, origImgWidth, CV_64F, cv::Scalar(0));
				cv::Mat tempContainer(origImgHeight, origImgWidth, CV_64F, cv::Scalar(0));
				int fltrIndx = 0;
				do
				{
					cv::filter2D(inputImgSeqPln(cv::Rect_<double>(0, (fltrIndx*origImgHeight), origImgWidth, origImgHeight)), tempContainer, CV_64F, H2e(cv::Rect_<double>(0, (fltrIndx*((2 * depth) + 1)), ((2 * wdth) + 1), ((2 * hgt) + 1))));
					H2Output = H2Output + tempContainer;
					fltrIndx = fltrIndx + 1;
				} while (fltrIndx < ((2 * depth) + 1));
				H2Output(cv::Rect(wdth, hgt, EnImgWidth, EnImgHeight)).copyTo(ImgEnBasicPlanes(cv::Rect(0, (EnPlnNmr * EnImgHeight), EnImgWidth, EnImgHeight)));
				EnPlnNmr = EnPlnNmr + 1;
			}
			//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			{
				cv::Mat H2Output(origImgHeight, origImgWidth, CV_64F, cv::Scalar(0));
				cv::Mat tempContainer(origImgHeight, origImgWidth, CV_64F, cv::Scalar(0));
				int fltrIndx = 0;
				do
				{
					cv::filter2D(inputImgSeqPln(cv::Rect_<double>(0, (fltrIndx*origImgHeight), origImgWidth, origImgHeight)), tempContainer, CV_64F, H2f(cv::Rect_<double>(0, (fltrIndx*((2 * depth) + 1)), ((2 * wdth) + 1), ((2 * hgt) + 1))));
					H2Output = H2Output + tempContainer;
					fltrIndx = fltrIndx + 1;
				} while (fltrIndx < ((2 * depth) + 1));
				H2Output(cv::Rect(wdth, hgt, EnImgWidth, EnImgHeight)).copyTo(ImgEnBasicPlanes(cv::Rect(0, (EnPlnNmr * EnImgHeight), EnImgWidth, EnImgHeight)));
				EnPlnNmr = EnPlnNmr + 1;
			}
			//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			{
				cv::Mat H2Output(origImgHeight, origImgWidth, CV_64F, cv::Scalar(0));
				cv::Mat tempContainer(origImgHeight, origImgWidth, CV_64F, cv::Scalar(0));
				int fltrIndx = 0;
				do
				{
					cv::filter2D(inputImgSeqPln(cv::Rect_<double>(0, (fltrIndx*origImgHeight), origImgWidth, origImgHeight)), tempContainer, CV_64F, H2g(cv::Rect_<double>(0, (fltrIndx*((2 * depth) + 1)), ((2 * wdth) + 1), ((2 * hgt) + 1))));
					H2Output = H2Output + tempContainer;
					fltrIndx = fltrIndx + 1;
				} while (fltrIndx < ((2 * depth) + 1));
				H2Output(cv::Rect(wdth, hgt, EnImgWidth, EnImgHeight)).copyTo(ImgEnBasicPlanes(cv::Rect(0, (EnPlnNmr * EnImgHeight), EnImgWidth, EnImgHeight)));
				EnPlnNmr = EnPlnNmr + 1;
			}
			//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			{
				cv::Mat H2Output(origImgHeight, origImgWidth, CV_64F, cv::Scalar(0));
				cv::Mat tempContainer(origImgHeight, origImgWidth, CV_64F, cv::Scalar(0));
				int fltrIndx = 0;
				do
				{
					cv::filter2D(inputImgSeqPln(cv::Rect_<double>(0, (fltrIndx*origImgHeight), origImgWidth, origImgHeight)), tempContainer, CV_64F, H2h(cv::Rect_<double>(0, (fltrIndx*((2 * depth) + 1)), ((2 * wdth) + 1), ((2 * hgt) + 1))));
					H2Output = H2Output + tempContainer;
					fltrIndx = fltrIndx + 1;
				} while (fltrIndx < ((2 * depth) + 1));
				H2Output(cv::Rect(wdth, hgt, EnImgWidth, EnImgHeight)).copyTo(ImgEnBasicPlanes(cv::Rect(0, (EnPlnNmr * EnImgHeight), EnImgWidth, EnImgHeight)));
				EnPlnNmr = EnPlnNmr + 1;
			}
			//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			{
				cv::Mat H2Output(origImgHeight, origImgWidth, CV_64F, cv::Scalar(0));
				cv::Mat tempContainer(origImgHeight, origImgWidth, CV_64F, cv::Scalar(0));
				int fltrIndx = 0;
				do
				{
					cv::filter2D(inputImgSeqPln(cv::Rect_<double>(0, (fltrIndx*origImgHeight), origImgWidth, origImgHeight)), tempContainer, CV_64F, H2i(cv::Rect_<double>(0, (fltrIndx*((2 * depth) + 1)), ((2 * wdth) + 1), ((2 * hgt) + 1))));
					H2Output = H2Output + tempContainer;
					fltrIndx = fltrIndx + 1;
				} while (fltrIndx < ((2 * depth) + 1));
				H2Output(cv::Rect(wdth, hgt, EnImgWidth, EnImgHeight)).copyTo(ImgEnBasicPlanes(cv::Rect(0, (EnPlnNmr * EnImgHeight), EnImgWidth, EnImgHeight)));
				EnPlnNmr = EnPlnNmr + 1;
			}
			//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			{
				cv::Mat H2Output(origImgHeight, origImgWidth, CV_64F, cv::Scalar(0));
				cv::Mat tempContainer(origImgHeight, origImgWidth, CV_64F, cv::Scalar(0));
				int fltrIndx = 0;
				do
				{
					cv::filter2D(inputImgSeqPln(cv::Rect_<double>(0, (fltrIndx*origImgHeight), origImgWidth, origImgHeight)), tempContainer, CV_64F, H2j(cv::Rect_<double>(0, (fltrIndx*((2 * depth) + 1)), ((2 * wdth) + 1), ((2 * hgt) + 1))));
					H2Output = H2Output + tempContainer;
					fltrIndx = fltrIndx + 1;
				} while (fltrIndx < ((2 * depth) + 1));
				H2Output(cv::Rect(wdth, hgt, EnImgWidth, EnImgHeight)).copyTo(ImgEnBasicPlanes(cv::Rect(0, (EnPlnNmr * EnImgHeight), EnImgWidth, EnImgHeight)));
				EnPlnNmr = EnPlnNmr + 1;
			}
			//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		}
	}
	if (timersShowFlg == 1)
	{
		timerVrabl = ((double)getTickCount() - timerVrabl) / getTickFrequency();
		std::cout << endl << "EnergyBasicFilteringCalculator: " << timerVrabl << " s" << std::endl;
	}
	return ImgEnBasicPlanes;
}