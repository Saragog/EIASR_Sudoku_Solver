#include "stdafx.h"
#include "DigitRecognizer.h"

using namespace cv;

/*
TODO
The implementation of digit recognizer will be written in this class
*/

DigitRecognizer::DigitRecognizer()
{

}

DigitRecognizer::~DigitRecognizer()
{
	
}

bool DigitRecognizer::train(char* trainPath, char* labelsPath)
{
	return true;
}

int DigitRecognizer::classify(Mat img)
{
	return 0;
}

Mat DigitRecognizer::preprocessImage(Mat img)
{
	return Mat();
}

int DigitRecognizer::readFlippedInteger(FILE *fp)
{
	return 0;
}