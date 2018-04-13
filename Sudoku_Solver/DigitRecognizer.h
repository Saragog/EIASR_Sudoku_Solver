#pragma once

#define MAX_NUM_IMAGES    60000

#include<opencv2/opencv.hpp>
#include<opencv2/ml.hpp>


/*
TODO
The implementation of digit recognizer will be written in this class
*/

class DigitRecognizer
{
public:
	DigitRecognizer();

	~DigitRecognizer();

	bool train(cv::String trainPath);

	int classify(cv::Mat img); //przyjmuje kolorowe obrazy

private:
	cv::Mat preprocessImage(cv::Mat img);
	void addTrainingImage(cv::Mat* trainingImages, cv::Mat img);
	void prepareTraining(cv::Mat* trainingImages, cv::Mat* classificationInts, cv::String path);


private:
	cv::Ptr<cv::ml::KNearest> classifier;
	int numRows, numCols, numImages;

};

