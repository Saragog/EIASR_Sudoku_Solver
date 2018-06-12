/*
	Creators Andrzej Dackiewicz, Mateusz Jarzemski
	This file is a part of a university project Sudoku Solver
	that was developed as a EIASR project.
	The aim is to create a software program, that is capable of recognizing sudoku problems
	read from photographs. The program is using OpenCV library.

	This file describes a class that is used for reading image from file, performing operations.
*/

#pragma once

#include<opencv2/opencv.hpp> 
#include<vector>
#include"DigitRecognizer.h"

class SudokuImageReader
{
public:
	SudokuImageReader();
	~SudokuImageReader();
	cv::Mat** readSudokuFromImage(cv::String path);

	static cv::Mat joinImagesIntoOne(cv::Mat** images);

private:
	cv::Mat findAndRemovePerspective(cv::Mat input);
	cv::Mat findBiggestBlob(cv::Mat input);
	cv::Mat** cutPuzzleImageIntoDigitsImages(cv::Mat imageToBeCut);
	void findCorners(std::vector<std::vector<cv::Point>> contours, std::vector<cv::Point>&);
	void SudokuImageReader::prepareDigitImagesForDetection(cv::Mat** puzzleSquareDigitImages);
	void SudokuImageReader::removeBorderFromImage(cv::Mat puzzleSquareDigitImage);

};

