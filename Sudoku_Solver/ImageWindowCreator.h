/*
	Creators Andrzej Dackiewicz, Mateusz Jarzemski
	This file is a part of a university project Sudoku Solver
	that was developed as a EIASR project.
	The aim is to create a software program, that is capable of recognizing sudoku problems
	read from photographs. The program is using OpenCV library.

	This file describes a class that is used for showing results and partial results of
	image computation.
*/

#pragma once

#include<opencv2/opencv.hpp> 

class ImageWindowCreator
{
public:
	ImageWindowCreator();
	~ImageWindowCreator();

	// Functions that are used for showing results of data extraction
	static void showImage(cv::String, cv::Mat);
	static void showImage(cv::String, int**, bool);						
	static void showImage(cv::String, cv::Mat**);

	static cv::Mat joinImagesIntoOne(cv::Mat**);									// Joins smaller images into one bigger image
																					// Used for joining single digit images into whole problem

	static cv::Mat** initializeFullImage(bool);										// Creates an image that is the base for showing program results
	static void showDetectedSudoku(cv::String, int**);								// Shows a window with detected images on the sudoku problem
private:
	static void drawLines(cv::Mat&, bool isBackgroundWhite = false);				// Draws lines for the sudoku problem
	static void drawCorrectnessToken(cv::Mat&, bool);								// Draws a token in upper right corner of the image that says
																					// whether the detected problem is an appropriate sudoku problem
	static cv::Mat createImageFromValues(int**, bool isBackgroundWhite = false);	// Creates a single image with printed given digits
};

