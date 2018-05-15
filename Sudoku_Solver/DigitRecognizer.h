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
	int** classifyAll(cv::Mat** images); // testing function for classification of 2d arrays of images
	cv::Mat preprocessSudokuDigitImage(cv::Mat img);

private:
	void addTrainingImage(cv::Mat* trainingImages, cv::Mat img);
	void prepareTraining(cv::Mat* trainingImages, cv::Mat* classificationInts, cv::String path);
	cv::Mat preprocessImage(cv::Mat img);

private:
	cv::Ptr<cv::ml::KNearest> classifier;
	int numRows, numCols, numImages;

};

