#pragma once

#include<opencv2/opencv.hpp> 
#include<vector>

class SudokuImageReader
{
public:
	SudokuImageReader();
	~SudokuImageReader();
	int** readSudokuFromImage(cv::String path);



private:
	int** initializeSudokuProblem();
	void drawLine(cv::Vec2f line, cv::Mat &img, cv::Scalar rgb = CV_RGB(0, 0, 255));
	cv::Mat** cutPuzzleImageIntoDigitsImages(cv::Mat imageToBeCut);
	void findCorners(std::vector<std::vector<cv::Point>> contours, std::vector<cv::Point>&);
};

