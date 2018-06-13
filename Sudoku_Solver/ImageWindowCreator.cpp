/*
	Creators Andrzej Dackiewicz, Mateusz Jarzemski
	This file is a part of a university project Sudoku Solver
	that was developed as a EIASR project.
	The aim is to create a software program, that is capable of recognizing sudoku problems
	read from photographs. The program is using OpenCV library.

	This file describes a class that is used for showing results and partial results of
	image computation.
*/

#include "stdafx.h"
#include "ImageWindowCreator.h"

using namespace cv;

ImageWindowCreator::ImageWindowCreator()
{
}

ImageWindowCreator::~ImageWindowCreator()
{
}

// Shows an image
void ImageWindowCreator::showImage(String windowName, cv::Mat image)
{
	cv::namedWindow(windowName, CV_WINDOW_NORMAL);
	cv::imshow(windowName, image);
}

// Joins smaller images into one and shows it
void ImageWindowCreator::showImage(cv::String windowName, Mat** partialImages)
{
	cv::Mat fullImage = joinImagesIntoOne(partialImages);
	delete partialImages;
	showImage(windowName, fullImage);
}

// Creates an image of a sudoku problem, checks if it is appropriate and shows the image
void ImageWindowCreator::showImage(cv::String windowName, int** values, bool isImageAppropriate)
{
	cv::Mat image = createImageFromValues(values);
	drawCorrectnessToken(image, isImageAppropriate);
	showImage(windowName, image);
}

// Shows the final result of digit recognition
cv::Mat ImageWindowCreator::createImageFromValues(int** values, bool isBackgroundWhite)
{
	cv::Mat fullImage;
	Scalar digitsColour;
	cv::Mat** resultImageParts = initializeFullImage(isBackgroundWhite);
	if (isBackgroundWhite == true) digitsColour = CV_RGB(0, 0, 0);
	else digitsColour = CV_RGB(0, 0, 255);
	for (int row = 0; row < 9; row++)
		for (int col = 0; col < 9; col++)
			if(values[row][col] != 0)
				putText(resultImageParts[row][col], std::to_string(values[row][col]), Point(20, 75), cv::FONT_HERSHEY_DUPLEX, 2.5, digitsColour,2);
	fullImage = joinImagesIntoOne(resultImageParts);
	delete resultImageParts;
	drawLines(fullImage, isBackgroundWhite);
	return fullImage;
}

// Shows the final image with detected digits
void ImageWindowCreator::showDetectedSudoku(cv::String windowName, int** values)
{
	cv::Mat image = createImageFromValues(values, true);
	showImage(windowName, image);
}

// Initializes the Mat object for showing the partial / final result of the algorythm
cv::Mat** ImageWindowCreator::initializeFullImage(bool isBackgroundWhite)
{
	cv::Mat** imageParts = new cv::Mat*[9];
	for (int r = 0; r < 9; r++)
	{
		imageParts[r] = new Mat[9];
		for (int c = 0; c < 9; c++)
		{
			if (isBackgroundWhite == true) imageParts[r][c] = Mat(100, 100, CV_8UC1, CV_RGB(0,0,255));
			else imageParts[r][c] = Mat(100, 100, CV_8UC1, CV_RGB(0,0,0));
		}
	}
	return imageParts;
}

// Joins many smaller images into one
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
	This function draws sudoku edges on the combined image
*/
void ImageWindowCreator::drawLines(cv::Mat& image, bool isBackgroundWhite)
{
	Scalar lineColour;
	if (isBackgroundWhite == true) lineColour = CV_RGB(0, 0, 0);
	else lineColour = CV_RGB(0, 0, 255);
	int width = 2;
	for (int row = 0; row < 10; row++)
	{
		if (row == 0 || row == 3 || row == 6 || row == 9)
			width = 9;
		else
			width = 2;
		line(image, Point(0, row * 100), Point(1000, row * 100), lineColour, width);
	}
	for (int column = 0; column < 10; column++)
	{
		if (column == 0 || column == 3 || column == 6 || column == 9)
			width = 9;
		else
			width = 2;
		line(image, Point(column * 100, 0), Point(column * 100, 1000), lineColour, width);
	}
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