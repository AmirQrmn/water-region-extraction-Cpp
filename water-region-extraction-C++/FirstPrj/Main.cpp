#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <vector>
#include "VideoToFramesSeq.h"
#include "EnergyExtraction.h"
#include "Grouping.h"
#include "MaritimeClusterIdentifier.h"

using namespace cv;
using namespace std;

int main()
{
	double t = (double)getTickCount();
	int timersShowFlg = 1;
	int RGB_notThermal = 1;
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	int VdoNr = 3;
	int resizeFactor;
	int FrmSkpRate;
	if (RGB_notThermal == 1)
	{
		resizeFactor = 4;		
		FrmSkpRate = 25;
	}
	else if (RGB_notThermal == 0)
	{
		resizeFactor = 1;		
		FrmSkpRate = 10;
	}
	int origFrmShowFlg = 1;
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	int EnFeaturesMagnifyingEnabler = 0; //dont change!
	int MyFilterFlg = 0;	//dont change!
	int hgt = 2;
	int wdth = 2;
	int depth = 2;
	int NumberOfOrientations = 8;
	int NoF = ((2 * depth) + 1);
	double epsln = 0.01;
	int EnFramesShowFlg = 0;
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	int NrmForBhtFlg = 0; // khodesh dare normal mikone! check kon badan ...
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	int msI = 1;
	int msJ = 1;
	int msK = 0;
	int RGBplotFlg = 0;
	int meanSshiftRowNumPrint;
	double msBhtSimilarityTr;
	double msHistSimTr;
	int msItrMax;
	double LblEnSimilarNeighborTr;
	if (RGB_notThermal == 1)
	{
		meanSshiftRowNumPrint = 0;	
		msBhtSimilarityTr = 0.9;
		msHistSimTr = 0.92;
		msItrMax = 10;
		LblEnSimilarNeighborTr = 0.995;
	}
	else if (RGB_notThermal == 0)
	{
		meanSshiftRowNumPrint = 0;	
		msBhtSimilarityTr = 0.92;
		msHistSimTr = 0.95;
		msItrMax = 10;
		LblEnSimilarNeighborTr = 0.997;
	}
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	int FlickerAmpFlg = 0;
	int FarFlg = 0;
	double NrmFlickerSumTr;
	double NrmStaticSumUpTr;
	double NrmStaticSumDownTr;
	if (RGB_notThermal == 1)
	{
		NrmFlickerSumTr = 0.01;		
		NrmStaticSumUpTr = 0.3;		
		NrmStaticSumDownTr = 0.01;
	}
	else if (RGB_notThermal == 0)
	{
		NrmFlickerSumTr = 0.008;			
		NrmStaticSumUpTr = 0.3;			
		NrmStaticSumDownTr = 0;
	}
	int FinalMaritimeRegionSizeTr = 400;
	int GrayLevelDiffTr = 0;
	int TempMaritimeFramePlot = 0;
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	int origImgWidth;
	int origImgHeight;
	int EnImgHeight;
	int EnImgWidth;
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	{
		int FrmCntr = 0;
		vector<Mat> inputImgsForEnCalcPlanes = SeqMaker(VdoNr, FrmCntr, FrmSkpRate, NoF, depth, origImgWidth, origImgHeight, resizeFactor, origFrmShowFlg, timersShowFlg);
		EnImgHeight = origImgHeight - (2 * hgt);
		EnImgWidth = origImgWidth - (2 * wdth);
		Mat matEnergyHolder{ ((2 * msK) + 1)*(NumberOfOrientations*EnImgHeight), EnImgWidth, CV_64F, Scalar(0) };
		for (FrmCntr = -msK; FrmCntr < (msK + 1); FrmCntr++)
		{
			vector<Mat> inputImgsForEnCalcPlanes = SeqMaker(VdoNr, FrmCntr, FrmSkpRate, NoF, depth, origImgWidth, origImgHeight, resizeFactor, origFrmShowFlg, timersShowFlg);
			Mat matNormalizedEnergyFrames = EnergyExtractor(inputImgsForEnCalcPlanes, NumberOfOrientations, hgt, wdth, depth, MyFilterFlg, origImgWidth, origImgHeight, EnImgHeight, EnImgWidth, epsln, EnFramesShowFlg, NrmForBhtFlg, timersShowFlg, EnFeaturesMagnifyingEnabler);
			matNormalizedEnergyFrames.copyTo(matEnergyHolder(cv::Rect_<double>(0, ((FrmCntr + msK)*(EnImgHeight*NumberOfOrientations)), EnImgWidth, (NumberOfOrientations*EnImgHeight))));
		}
		Mat msSmoothedEnergyFrames;
		int NumOfRgbRegions;
		Mat LabelsTable = Grouping(matEnergyHolder, msSmoothedEnergyFrames, NumOfRgbRegions, EnImgHeight, EnImgWidth, NumberOfOrientations, depth, msI, msJ, msK, msBhtSimilarityTr, msHistSimTr, msItrMax, LblEnSimilarNeighborTr, RGBplotFlg, meanSshiftRowNumPrint, timersShowFlg);
		Mat Maritimeframe = MaritimeClusterIdentifier(msSmoothedEnergyFrames, LabelsTable, EnImgHeight, EnImgWidth, NrmFlickerSumTr, NrmStaticSumUpTr, NrmStaticSumDownTr, NumOfRgbRegions, FlickerAmpFlg, FinalMaritimeRegionSizeTr, FarFlg, GrayLevelDiffTr, timersShowFlg, TempMaritimeFramePlot);
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	}

	t = ((double)getTickCount() - t) / getTickFrequency();
	std::cout << endl << "Times passed in seconds: " << t << std::endl;

	//cin.get();
	waitKey(0);
	return 0;
}