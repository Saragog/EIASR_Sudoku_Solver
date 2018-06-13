/*
	Creators Andrzej Dackiewicz, Mateusz Jarzemski
	This file is a part of a university project Sudoku Solver
	that was developed as a EIASR project.
	The aim is to create a software program, that is capable of recognizing sudoku problems
	read from photographs. The program is using OpenCV library.

	This file describes a class that is used for digit recognition.
	One recognizer was not enough for proper detection of 9 digits + empty slot.
	This recognizer is run after the first one and is to additionally recognize the most problematic digits such as ones and sevens.
*/

#pragma once
#include "DigitRecognizer.h"

class RedundantOnesRecognizer :public DigitRecognizer
{
public:
	RedundantOnesRecognizer();
	~RedundantOnesRecognizer();

	void classifyOnes(cv::Mat** images, int** sudokuValues);	// Classifies if there are ones on the image
	cv::Mat preprocessSudokuDigitImage(cv::Mat img);
	bool train(cv::String path);  // Trains the model

private:

	// Preparation of a model for training
	void prepareTraining(cv::Mat* trainingImages, cv::Mat* classificationInts, cv::String path);

	// Reading different learning data folders
	void prepareOneFolder(cv::Mat * trainingImages, cv::Mat * classificationInts, cv::String path, int num);


};

