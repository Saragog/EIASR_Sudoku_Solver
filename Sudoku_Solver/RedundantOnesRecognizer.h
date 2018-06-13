#pragma once
#include "DigitRecognizer.h"

class RedundantOnesRecognizer :public DigitRecognizer
{
public:
	RedundantOnesRecognizer();
	~RedundantOnesRecognizer();

	void classifyOnes(cv::Mat** images, int** sudokuValues);
	cv::Mat preprocessSudokuDigitImage(cv::Mat img);
	bool train(cv::String path);

private:
	void prepareTraining(cv::Mat* trainingImages, cv::Mat* classificationInts, cv::String path);

	void prepareOneFolder(cv::Mat * trainingImages, cv::Mat * classificationInts, cv::String path, int num);


};

