#pragma once

#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <sstream>
#include <stdint.h>

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

using namespace cv;

class LineFollower
{
public:
	LineFollower();
	~LineFollower();

	void nextFrame(Mat &frame);
	void setMaxSpeed(int max);

	int getSpeed();
	int getServo();

private:
	void calculate(float offset);
	int speed; // speed in percents
	int servo; // servo angle in percents

	const int thresh = 100;
	const int width = 640;

	int speed_max;
	const double P_value = 1;
	const double I_value = 0;
};

