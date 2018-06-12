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
	static void drawLines(cv::Mat&, bool isBackgroundWhite = false);
	static void drawCorrectnessToken(cv::Mat&, bool);
	static cv::Mat createImageFromValues(int**, bool isBackgroundWhite = false);
};

