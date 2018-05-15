#include "stdafx.h"
#include "ImageWindowCreator.h"

using namespace cv;

ImageWindowCreator::ImageWindowCreator()
{
}

ImageWindowCreator::~ImageWindowCreator()
{
}

void ImageWindowCreator::showImage(String windowName, cv::Mat image)
{
	cv::namedWindow(windowName, CV_WINDOW_NORMAL);
	cv::imshow(windowName, image);
}

void ImageWindowCreator::showImage(cv::String windowName, Mat** partialImages)
{
	cv::Mat fullImage = joinImagesIntoOne(partialImages);
	delete partialImages;
	showImage(windowName, fullImage);
}

void ImageWindowCreator::showImage(cv::String windowName, int** values, bool isImageAppropriate)
{
	cv::Mat fullImage;
	cv::Mat** resultImageParts = initializeFullImage();
	for (int row = 0; row < 9; row++)
		for (int col = 0; col < 9; col++)
			putText(resultImageParts[row][col], std::to_string(values[row][col]), Point(10, 40), cv::FONT_HERSHEY_COMPLEX, 1, CV_RGB(0, 0, 255));
	fullImage = joinImagesIntoOne(resultImageParts);
	delete resultImageParts;

	drawCorners(fullImage);
	drawCorrectnessToken(fullImage, isImageAppropriate);
	// ______
	showImage(windowName, fullImage);
}

cv::Mat** ImageWindowCreator::initializeFullImage()
{
	cv::Mat** imageParts = new cv::Mat*[9];
	for (int r = 0; r < 9; r++)
	{
		imageParts[r] = new Mat[9];
		for (int c = 0; c < 9; c++)
		{
			imageParts[r][c] = Mat(100, 100, CV_8UC1, Scalar(0, 0, 0));
		}
	}
	return imageParts;
}

Mat ImageWindowCreator::joinImagesIntoOne(cv::Mat** images) // testing function for showing progress in whole image
{
	Mat wholeImage;

	std::vector<Mat> rows;
	std::vector<Mat> cols;
	if (images != NULL)
	{
		for (int row = 0; row < 9; row++)
		{
			cols.clear();
			for (int col = 0; col < 9; col++)
				cols.push_back(images[row][col]);
			rows.push_back(Mat());
			hconcat(cols, rows[row]);
		}
		vconcat(rows, wholeImage);

	}

	return wholeImage;

}

/*
TODO Zrobic gdzies zmienna przechowujaca wszystkie wielkości obrazu
Narazie zrobione na sztywno po 100 pikseli natomiast potem trzeba to gdzies static consta
*/

/*
	This function draws sudoku edges on the combined image
*/
void ImageWindowCreator::drawCorners(cv::Mat& image)
{
	for (int row = 0; row < 10; row++)
		line(image, Point(0, row * 100), Point(1000, row * 100), CV_RGB(0,0,255), 2/*, int lineType = 8, int shift = 0*/);
	for (int column = 0; column < 10; column++)
		line(image, Point(column * 100, 0), Point(column * 100, 1000), CV_RGB(0, 0, 255), 2);
	return;
}

/*
	This function draws a letter on the upper right corner of the combined image
	showing whether the sudoku problem that was read from the image is a proper sudoku problem
*/
void ImageWindowCreator::drawCorrectnessToken(cv::Mat& fullImage, bool isImageAppropriate)
{
	putText(fullImage, isImageAppropriate == true ? "T" : "F", Point(850, 50), cv::FONT_HERSHEY_COMPLEX, 2, CV_RGB(0, 0, 180));
}