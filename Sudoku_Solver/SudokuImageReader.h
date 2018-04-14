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



private:

	
	int** initializeSudokuProblem();
	void drawLine(cv::Vec2f line, cv::Mat &img, cv::Scalar rgb = CV_RGB(0, 0, 255));
	cv::Mat** cutPuzzleImageIntoDigitsImages(cv::Mat imageToBeCut);
	void findCorners(std::vector<std::vector<cv::Point>> contours, std::vector<cv::Point>&);
	void SudokuImageReader::prepareDigitImagesForDetection(cv::Mat** puzzleSquareDigitImages);
	void SudokuImageReader::convertColourImagesToGrayScale(cv::Mat** puzzleSquareDigitImages);
	void SudokuImageReader::blurColourImagesToGrayScale(cv::Mat** puzzleSquareDigitImages);
	void SudokuImageReader::thresholdDigitImages(cv::Mat** puzzleSquareDigitImages);
	void SudokuImageReader::dilateDigitImages(cv::Mat** puzzleSquareDigitImages);
	void SudokuImageReader::erodeDigitImages(cv::Mat** puzzleSquareDigitImages);
	void SudokuImageReader::removeBorders(cv::Mat** puzzleSquareDigitImages);
	void SudokuImageReader::removeBorderFromImage(cv::Mat puzzleSquareDigitImage);
};

