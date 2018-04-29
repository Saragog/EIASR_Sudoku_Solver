#pragma once

#include<opencv2/opencv.hpp> 

class ImageWindowCreator
{
public:
	ImageWindowCreator();
	~ImageWindowCreator();

	static void showImage(int**);
	static void showImage(cv::Mat**);
};

