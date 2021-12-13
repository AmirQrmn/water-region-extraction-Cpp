#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>
#include "BhattacharyyaCoefCalculator.h"
#include "ModifyLabelsTable.h"

using namespace cv;
using namespace std;

int GrayLevelDiffCalculator(const int CurrentPixelVal, const int TargetPixelVal)
{
	int GrayLevelDifferenceVal = abs(CurrentPixelVal - TargetPixelVal);
	return GrayLevelDifferenceVal;
}