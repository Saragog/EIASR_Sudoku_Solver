/*
	Creators Andrzej Dackiewicz, Mateusz Jarzemski
	This file is a part of a university project Sudoku Solver
	that was developed as a EIASR project.
	The aim is to create a software program, that is capable of recognizing sudoku problems
	read from photographs. The program is using OpenCV library.

	This file describes a class that is used for reading image from file, performing operations that
	are used for extracting images of single digits representing the sudoku problem. Later on these images
	give us information about what digits are placed on the sudoku problem image.
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
	cv::Mat** readSudokuFromImage(cv::String path);												// Reads sudoku problem image specified by path

private:
	cv::Mat findAndRemovePerspective(cv::Mat input);											// Removes perspective of the sudoku problem
	cv::Mat findBiggestBlob(cv::Mat input);														// Finds the biggest blob of single colour on the image
	cv::Mat** cutPuzzleImageIntoDigitsImages(cv::Mat imageToBeCut);								// Cuts sudoku problem into single digit blocks
	void findCorners(std::vector<std::vector<cv::Point>> contours, std::vector<cv::Point>&);	// Finds corners of the sudoku problem
	void SudokuImageReader::prepareDigitImagesForDetection(cv::Mat** puzzleSquareDigitImages);	// Prepares single digit images for digit extraction
	void SudokuImageReader::removeBorderFromImage(cv::Mat puzzleSquareDigitImage);				// Removes the border from an image

};

