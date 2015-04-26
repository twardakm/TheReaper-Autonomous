#include "LineFollower.h"

LineFollower::LineFollower()
{
	speed = 0;
	servo = 0;
}


LineFollower::~LineFollower()
{
}

void LineFollower::nextFrame(Mat &frame)
{
	// https://github.com/karan259/open_cv_pi/blob/master/line_folower_1.cpp
	cvtColor(frame, frame, CV_BGR2GRAY);
	GaussianBlur(frame, frame, Size(9, 9), 2, 2);
	threshold(frame, frame, thresh, 255, 0);

	frame = Scalar::all(255) - frame;

	Moments mu;
	mu = moments(frame, true);

	Point2f center(mu.m10 / mu.m00, mu.m01 / mu.m00);

	float offset = (1 - 2 * center.x / width);
	if (isnan(offset))
		offset = 0;

	calculate(offset); // calculate new values for motor and servo

	cvtColor(frame, frame, CV_GRAY2RGB);

	circle(frame, center, 5, Scalar(0, 0, 255), 2);

	std::ostringstream buff;
	buff << offset;
	std::stringstream tempString;
	tempString << speed_max;
	putText(frame, buff.str(), center, FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 255), 3);
	putText(frame, string("Max speed: ") + tempString.str(), Point2f(0, 50), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 255), 2);
	tempString.str(string());
	tempString << servo;
	putText(frame, string("Servo: ") + tempString.str(), Point2f(0,100), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 255), 2);
}

void LineFollower::calculate(float offset)
{
	servo = 100.0 * offset * P_value;
	speed = speed_max;
}

void LineFollower::setMaxSpeed(int max)
{
	speed_max = max;
}

int LineFollower::getSpeed()
{
	return speed;
}

int LineFollower::getServo()
{
	return servo;
}