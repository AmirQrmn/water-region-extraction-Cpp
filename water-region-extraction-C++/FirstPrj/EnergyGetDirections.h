#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>

std::vector<double> EnergyGetDirections(const int NumberOfOrientations, const int timersShowFlg)
{
	double timerVrabl = 0;
	if (timersShowFlg == 1)
	{
		timerVrabl = (double)getTickCount();
	}
	int t = 0;
	double a, b, c;
	std::vector<double> Directions;
	Directions.resize(3 * (NumberOfOrientations-1));
	if (NumberOfOrientations == 5)
	{
		int t = 0;
		do
		{
			if (t == 0)
			{
				a = 1 / (sqrt(2));
				b = 0;
				c = 1 / (sqrt(2));
			}
			else if (t == 1)
			{
				a = -1 / (sqrt(2));
				b = 0;
				c = 1 / (sqrt(2));
			}
			else if (t == 2)
			{
				a = 0;
				b = 1 / (sqrt(2));
				c = 1 / (sqrt(2));
			}
			else if (t == 3)
			{
				a = 0;
				b = -1 / (sqrt(2));
				c = 1 / (sqrt(2));
			}
			Directions[t * 3] = a;
			Directions[(t * 3) + 1] = b;
			Directions[(t * 3) + 2] = c;
			t = t + 1;
		} while (t < NumberOfOrientations - 1);
	}
	else if (NumberOfOrientations == 6)
	{
		int t = 0;
		do
		{
			if (t == 0)
			{
				a = 0;
				b = 0;
				c = 1;
			}
			else if (t == 1)
			{
				a = 1 / (sqrt(2));
				b = 0;
				c = 1 / (sqrt(2));
			}
			else if (t == 2)
			{
				a = -1 / (sqrt(2));
				b = 0;
				c = 1 / (sqrt(2));
			}
			else if (t == 3)
			{
				a = 0;
				b = 1 / (sqrt(2));
				c = 1 / (sqrt(2));
			}
			else if (t == 4)
			{
				a = 0;
				b = -1 / (sqrt(2));
				c = 1 / (sqrt(2));
			}
			Directions[t * 3] = a;
			Directions[(t * 3) + 1] = b;
			Directions[(t * 3) + 2] = c;
			t = t + 1;
		} while (t < NumberOfOrientations - 1);
	}
	else if (NumberOfOrientations == 8)
	{
		int t = 0;
		do
		{
			if (t == 0)
			{
				a = 1;
				b = 0;
				c = 0;
			}
			else if (t == 1)
			{
				a = 0;
				b = 1;
				c = 0;
			}
			else if (t == 2)
			{
				a = 0;
				b = 0;
				c = 1;
			}
			else if (t == 3)
			{
				a = 1 / (sqrt(2));
				b = 0;
				c = 1 / (sqrt(2));
			}
			else if (t == 4)
			{
				a = -1 / (sqrt(2));
				b = 0;
				c = 1 / (sqrt(2));
			}
			else if (t == 5)
			{
				a = 0;
				b = 1 / (sqrt(2));
				c = 1 / (sqrt(2));
			}
			else if (t == 6)
			{
				a = 0;
				b = -1 / (sqrt(2));
				c = 1 / (sqrt(2));
			}
			Directions[t * 3] = a;
			Directions[(t * 3) + 1] = b;
			Directions[(t * 3) + 2] = c;
			
			t = t + 1;
		} while (t < NumberOfOrientations - 1);
	}

	if (timersShowFlg == 1)
	{
		timerVrabl = ((double)getTickCount() - timerVrabl) / getTickFrequency();
		std::cout << endl << "EnergyGetDirections: " << timerVrabl << " s" << std::endl;
	}

	return Directions;
}