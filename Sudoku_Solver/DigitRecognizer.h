#pragma once

#define MAX_NUM_IMAGES    60000

#include<opencv2/opencv.hpp>
#include<opencv2\ml.hpp>

/*
TODO
The implementation of digit recognizer will be written in this class
*/

class DigitRecognizer
{
public:
	DigitRecognizer();

	~DigitRecognizer();

	bool train(char* trainPath, char* labelsPath);

	int classify(cv::Mat img);

	private:
		cv::Mat preprocessImage(cv::Mat img);

		int readFlippedInteger(FILE *fp);

	private:
		//ml::KNearest *knn;
		int numRows, numCols, numImages;

};

