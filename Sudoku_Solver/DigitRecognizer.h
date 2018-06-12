/*
	Creators Andrzej Dackiewicz, Mateusz Jarzemski
	This file is a part of a university project Sudoku Solver
	that was developed as a EIASR project.
	The aim is to create a software program, that is capable of recognizing sudoku problems
	read from photographs. The program is using OpenCV library.

	This file describes a class that is used for digit recognition.
*/

#pragma once

#define MAX_NUM_IMAGES    60000

#include<opencv2/opencv.hpp>
#include<opencv2/ml.hpp>

class DigitRecognizer
{
public:
	DigitRecognizer();

	~DigitRecognizer();

	bool train(cv::String trainPath);

	int classify(cv::Mat img); //przyjmuje kolorowe obrazy
	int** classifyAll(cv::Mat** images); // testing function for classification of 2d arrays of images
	virtual cv::Mat preprocessSudokuDigitImage(cv::Mat img);
	void addTrainingImage(cv::Mat* trainingImages, cv::Mat img);

private:
	
	virtual void prepareTraining(cv::Mat* trainingImages, cv::Mat* classificationInts, cv::String path);
	cv::Mat preprocessImage(cv::Mat img);

private:
	cv::Ptr<cv::ml::SVM> svm;
	int numRows, numCols, numImages;

};

