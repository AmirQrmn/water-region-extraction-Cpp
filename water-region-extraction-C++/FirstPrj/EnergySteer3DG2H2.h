#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

Mat EnergySteer3DG2H2(Mat ImgEnBasicPlanes, std::vector<double>Directions, int NumberOfOrientations, const int EnImgHeight, const int EnImgWidth, const int timersShowFlg)
{
	double timerVrabl = 0;
	if (timersShowFlg == 1)
	{
		timerVrabl = (double)getTickCount();
	}
	Mat ImgOrientedEnergyPlanes((NumberOfOrientations * EnImgHeight), EnImgWidth, CV_64F, cv::Scalar(0));
	{
		Mat imgG(EnImgHeight, EnImgWidth, CV_64F, cv::Scalar(0));
		Mat imgH(EnImgHeight, EnImgWidth, CV_64F, cv::Scalar(0));
		Mat imgEnergy;
		double a, b, c;
		int i = 0;
		do
		{
			a = Directions[i * 3];
			b = Directions[(i * 3) + 1];
			c = Directions[(i * 3) + 2];

			imgG = ((pow(a, 2)*(ImgEnBasicPlanes(cv::Rect(0, 0, EnImgWidth, EnImgHeight)))) + \
				(2 * a*b*(ImgEnBasicPlanes(cv::Rect(0, EnImgHeight, EnImgWidth, EnImgHeight)))) + \
				(pow(b, 2)*(ImgEnBasicPlanes(cv::Rect(0, (2 * EnImgHeight), EnImgWidth, EnImgHeight)))) + \
				(2 * a*c*(ImgEnBasicPlanes(cv::Rect(0, (3 * EnImgHeight), EnImgWidth, EnImgHeight)))) + \
				(2 * b*c*(ImgEnBasicPlanes(cv::Rect(0, (4 * EnImgHeight), EnImgWidth, EnImgHeight)))) + \
				(pow(c, 2)*(ImgEnBasicPlanes(cv::Rect(0, (5 * EnImgHeight), EnImgWidth, EnImgHeight)))));
			
			imgH = (pow(a, 3)*(ImgEnBasicPlanes(cv::Rect(0, (6 * EnImgHeight), EnImgWidth, EnImgHeight)))) \
				+ (3 * pow(a, 2)*b*(ImgEnBasicPlanes(cv::Rect(0, (7 * EnImgHeight), EnImgWidth, EnImgHeight)))) \
				+ (3 * a*pow(b, 2)*(ImgEnBasicPlanes(cv::Rect(0, (8 * EnImgHeight), EnImgWidth, EnImgHeight)))) \
				+ (pow(b, 3)*(ImgEnBasicPlanes(cv::Rect(0, (9 * EnImgHeight), EnImgWidth, EnImgHeight)))) \
				+ (3 * pow(a, 2)*c*(ImgEnBasicPlanes(cv::Rect(0, (10 * EnImgHeight), EnImgWidth, EnImgHeight)))) \
				+ (6 * a*b*c*(ImgEnBasicPlanes(cv::Rect(0, (11 * EnImgHeight), EnImgWidth, EnImgHeight)))) \
				+ (3 * pow(b, 2)*c*(ImgEnBasicPlanes(cv::Rect(0, (12 * EnImgHeight), EnImgWidth, EnImgHeight)))) \
				+ (3 * a*pow(c, 2)*(ImgEnBasicPlanes(cv::Rect(0, (13 * EnImgHeight), EnImgWidth, EnImgHeight)))) \
				+ (3 * b*pow(c, 2)*(ImgEnBasicPlanes(cv::Rect(0, (14 * EnImgHeight), EnImgWidth, EnImgHeight)))) \
				+ (pow(c, 3)*(ImgEnBasicPlanes(cv::Rect(0, (15 * EnImgHeight), EnImgWidth, EnImgHeight))));
			
			pow(imgG, 2, imgG);
			pow(imgH, 2, imgH);
			imgEnergy = (imgG + imgH);
			imgEnergy.copyTo(ImgOrientedEnergyPlanes(cv::Rect(0, (i * EnImgHeight), EnImgWidth, EnImgHeight)));
			i = i + 1;
		} while (i < NumberOfOrientations - 1);
	}
	if (timersShowFlg == 1)
	{
		timerVrabl = ((double)getTickCount() - timerVrabl) / getTickFrequency();
		std::cout << endl << "Energysteer3DG2H2: " << timerVrabl << " s" << std::endl;
	}
	return ImgOrientedEnergyPlanes;
}