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
	static cv::Mat joinImagesIntoOne(cv::Mat** images);
	static cv::Mat** initializeFullImage();
private:
	static void drawCorners(cv::Mat&);
	static void drawCorrectnessToken(cv::Mat&, bool);
};

