/*
	Creators Andrzej Dackiewicz, Mateusz Jarzemski
	This file is a part of a university project Sudoku Solver
	that was developed as a EIASR project.
	The aim is to create a software program, that is capable of recognizing sudoku problems
	read from photographs. The program is using OpenCV library.

	This file describes a class that is used for digit recognition.
*/

#pragma once

#include<opencv2/opencv.hpp> 

class ImageWindowCreator
{
public:
	ImageWindowCreator();
	~ImageWindowCreator();

	static void showImage(cv::String, cv::Mat);
	static void showImage(cv::String, int**, bool);
	static void showImage(cv::String, cv::Mat**);
	static cv::Mat joinImagesIntoOne(cv::Mat**);
	static cv::Mat** initializeFullImage(bool);
	static void showDetectedSudoku(cv::String, int**);
private:
	static void drawCorners(cv::Mat&, bool isBackgroundWhite = false);
	static void drawCorrectnessToken(cv::Mat&, bool);
	static cv::Mat createImageFromValues(int**, bool isBackgroundWhite = false);
};

