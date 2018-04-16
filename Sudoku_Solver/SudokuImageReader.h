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

	static cv::Mat joinImagesIntoOne(cv::Mat** images); // testing method for showing progress in whole image

private:

	int** initializeSudokuProblem();
	//void drawLine(cv::Vec2f line, cv::Mat &img, cv::Scalar rgb = CV_RGB(0, 0, 255));
	cv::Mat findAndRemovePerspective(cv::Mat input);
	cv::Mat findBiggestBlob(cv::Mat input);
	cv::Mat** cutPuzzleImageIntoDigitsImages(cv::Mat imageToBeCut);
	void findCorners(std::vector<std::vector<cv::Point>> contours, std::vector<cv::Point>&);
	void SudokuImageReader::prepareDigitImagesForDetection(cv::Mat** puzzleSquareDigitImages);
	void SudokuImageReader::removeBorderFromImage(cv::Mat puzzleSquareDigitImage);

};

