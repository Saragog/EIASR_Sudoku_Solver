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

#include "stdafx.h"
#include "SudokuImageReader.h"
#include "ImageWindowCreator.h"
#include<conio.h>

using namespace cv;

#define FLAT_IMAGE_WIDTH 120*9
#define FLAT_IMAGE_HEIGHT 120*9

SudokuImageReader::SudokuImageReader()
{

}

SudokuImageReader::~SudokuImageReader()
{
}

/*
Reads the sudoku problem from an image under given path
*/
Mat** SudokuImageReader::readSudokuFromImage(String path)
{
	
	// read from image
		
	cv::Mat imgOriginal;			// input image
	cv::Mat imgPerspective;			// detected lines image
	
	cv::Mat** puzzleSquareDigitsImages; // images of where the digits should be
	
	

	
	imgOriginal = cv::imread(path);

	if (imgOriginal.empty()) {                                  // if unable to open image
		std::cout << "error: image not read from file\n\n";     // show error message on command line
		_getch();                                               // may have to modify this line if not using Windows
		return NULL;                                            // and exit program
	}

	ImageWindowCreator::showImage("Original Image", imgOriginal);

	imgPerspective = findAndRemovePerspective(imgOriginal);

	puzzleSquareDigitsImages = cutPuzzleImageIntoDigitsImages(imgPerspective); // cuts puzzle into ditits images

	prepareDigitImagesForDetection(puzzleSquareDigitsImages);

	ImageWindowCreator::showImage("Perspective removed", imgPerspective);												  

	return puzzleSquareDigitsImages;
}

// Prepares the parts of the image with digits for digit classification
void SudokuImageReader::prepareDigitImagesForDetection(cv::Mat** puzzleSquareDigitImages)
{
	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 9; col++)
		{
			cv::cvtColor(puzzleSquareDigitImages[row][col], puzzleSquareDigitImages[row][col], CV_BGR2GRAY);       // convert to grayscale

			cv::GaussianBlur(puzzleSquareDigitImages[row][col],         // input image
				puzzleSquareDigitImages[row][col],						// output image
				cv::Size(5, 5),											// smoothing window width and height in pixels
				0);														// sigma value, determines how much the image will be blurred
			

			cv::adaptiveThreshold(puzzleSquareDigitImages[row][col], puzzleSquareDigitImages[row][col],
				255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 19, 2.5);
			
		}
	}

	Mat combinedParts = ImageWindowCreator::joinImagesIntoOne(puzzleSquareDigitImages);

	//ImageWindowCreator::showImage("combinedParts Threshold", combinedParts);

	cv::Mat kernel = (cv::Mat_<uchar>(3, 3) << 0, 1, 0, 1, 1, 1, 0, 1, 0);
	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 9; col++)
		{
			cv::dilate(puzzleSquareDigitImages[row][col], puzzleSquareDigitImages[row][col], kernel); 
			removeBorderFromImage(puzzleSquareDigitImages[row][col]);
			findBiggestBlob(puzzleSquareDigitImages[row][col]);
		}
	}

	combinedParts = ImageWindowCreator::joinImagesIntoOne(puzzleSquareDigitImages);
	ImageWindowCreator::showImage("combinedParts removedBorder", combinedParts);

	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 9; col++)
		{
			cv::Moments m = moments(puzzleSquareDigitImages[row][col], true);
			int middleX = (int)(m.m10 / m.m00), middleY = (int)(m.m01 / m.m00);

			int halfOfHeight = puzzleSquareDigitImages[row][col].size().height / 2;
			int halfOfWidth = puzzleSquareDigitImages[row][col].size().width / 2;

			int deltaX = halfOfWidth - middleX;
			int deltaY = halfOfHeight - middleY;

			Mat translationMatrix = (Mat_<double>(2, 3) << 1, 0, deltaX, 0, 1, deltaY);
			warpAffine(puzzleSquareDigitImages[row][col], puzzleSquareDigitImages[row][col], translationMatrix, puzzleSquareDigitImages[row][col].size());

			erode(puzzleSquareDigitImages[row][col], puzzleSquareDigitImages[row][col], kernel);
			
		}
	}

}


// Removes border from an image
void SudokuImageReader::removeBorderFromImage(cv::Mat puzzleSquareDigitImage)
{
	int xDimSize = puzzleSquareDigitImage.size().width;
	int yDimSize = puzzleSquareDigitImage.size().height;

	for (int y = 0; y < yDimSize; y++)
	{
		uchar *row = puzzleSquareDigitImage.ptr(y);
		for (int x = 0; x < xDimSize; x++)
			if (x == 10 && y >= 10 && y <= yDimSize - 10) x = xDimSize - 10;
			else if (row[x] == 255)
				floodFill(puzzleSquareDigitImage, Point(x, y), CV_RGB(0, 0, 0));
	}
}

// Finds and removes perspective from a problem image
cv::Mat SudokuImageReader::findAndRemovePerspective(cv::Mat input)
{
	cv::Mat imgBlurred;         // intermediate blured image
	cv::Mat imgThreshold;		// tresholded image
	cv::Mat imgBiggestBlob;		// biggest blob image

	resize(input, imgBlurred, cv::Size((input.size().width*1200)/input.size().height, 1200)); // przeskalowanie z zachowaniem proporcji

	cv::cvtColor(imgBlurred, imgBlurred, CV_BGR2GRAY);       // convert to grayscale

	cv::GaussianBlur(imgBlurred,          // input image
		imgBlurred,                         // output image
		cv::Size(11, 11),					// smoothing window width and height in pixels
		0);                               // sigma value, determines how much the image will be blurred

	//ImageWindowCreator::showImage("imgBlurred", imgBlurred);

	cv::adaptiveThreshold(imgBlurred, imgThreshold, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY_INV, 5, 1.5);

	//ImageWindowCreator::showImage("Img threshold", imgThreshold);

	cv::Mat kernel = (cv::Mat_<uchar>(3, 3) << 0, 1, 0, 1, 1, 1, 0, 1, 0);

	cv::dilate(imgThreshold, imgThreshold, kernel);

	//ImageWindowCreator::showImage("Img Threshold Dilate", imgThreshold);
	
	imgBiggestBlob = findBiggestBlob(imgThreshold);

	//ImageWindowCreator::showImage("imgBiggestBlob", imgBiggestBlob);

	std::vector<std::vector<Point>> contours;

	findContours(imgBiggestBlob, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	std::vector<Point> corners;
	Point2f cornersf[4];

	findCorners(contours, corners);   // Finding corners of a sudoku problem

	Mat imgDetectedCorners = imgBlurred.clone();
	cv::cvtColor(imgDetectedCorners, imgDetectedCorners, CV_GRAY2BGR);

	// Preparing an image with red dots showing where the corners of a sudoku problem were found
	float resizeRatio = (float)input.size().height / 1200;
	for (int circleIndex = 0; circleIndex < 4; circleIndex++)
	{
		circle(imgDetectedCorners, corners[circleIndex], 5, CV_RGB(255, 0, 0), -1);
		cornersf[circleIndex] = Point2f(corners[circleIndex].x *resizeRatio, corners[circleIndex].y*resizeRatio);
	}

	//ImageWindowCreator::showImage("Img Detected Corners", imgDetectedCorners);

	Point2f dst[] = { Point2f(0,0),Point2f(FLAT_IMAGE_WIDTH,0), Point2f(0,FLAT_IMAGE_HEIGHT),Point2f(FLAT_IMAGE_WIDTH,FLAT_IMAGE_HEIGHT) };

	// Removing the perspective from an image
	Mat imgPerspective;
	Mat M = getPerspectiveTransform(cornersf, dst);
	warpPerspective(input, imgPerspective, M, Size(FLAT_IMAGE_WIDTH, FLAT_IMAGE_HEIGHT));

	//ImageWindowCreator::showImage("Img Perspective", imgPerspective);

	return imgPerspective;
}

// Finding the biggest blob - the border of our sudoku problem
cv::Mat SudokuImageReader::findBiggestBlob(cv::Mat input)
{
	Mat imgBiggestBlob(input);

	int count = 0;
	int max = -1;

	Point maxPt;

	for (int y = 0; y<imgBiggestBlob.size().height; y++)
	{
		uchar *row = imgBiggestBlob.ptr(y);
		for (int x = 0; x<imgBiggestBlob.size().width; x++)
		{
			if (row[x] >= 128)
			{

				int area = floodFill(imgBiggestBlob, Point(x, y), CV_RGB(0, 0, 64));

				if (area>max)
				{
					maxPt = Point(x, y);
					max = area;
				}
			}
		}

	}
	if (max < 200)
	{
		imgBiggestBlob = Mat::zeros(imgBiggestBlob.size(), imgBiggestBlob.type());
		return imgBiggestBlob;	
	}
 
	floodFill(imgBiggestBlob, maxPt, CV_RGB(255, 255, 255));


	for (int y = 0; y<imgBiggestBlob.size().height; y++)
	{
		uchar *row = imgBiggestBlob.ptr(y);
		for (int x = 0; x<imgBiggestBlob.size().width; x++)
		{
			if (row[x] == 64 && x != maxPt.x && y != maxPt.y)
			{
				int area = floodFill(imgBiggestBlob, Point(x, y), CV_RGB(0, 0, 0));
			}
		}
	}
	return imgBiggestBlob;
}

// Cutting a sudoku problem into small images with single digits printed on them
Mat** SudokuImageReader::cutPuzzleImageIntoDigitsImages(cv::Mat imageToBeCut)
{
	cv::Mat** digitsImages = new cv::Mat*[9];
	for (int row = 0; row < 9; row++)
		digitsImages[row] = new cv::Mat[9]();

	int yDimSize = imageToBeCut.size().height;
	int xDimSize = imageToBeCut.size().width;
	int yDimIncrease = yDimSize / 9;
	int xDimIncrease = xDimSize / 9;

	int upperIndentation, leftIndentation, rightIndentation, bottomIndentation;

	for (int row = 0; row < 9; row++)
	{
		for (int col = 0;col < 9; col++)
		{
			upperIndentation = 30; leftIndentation = 30; rightIndentation = 30; bottomIndentation = 30;
			if (row == 0) upperIndentation = 0;
			if (col == 0) leftIndentation = 0;
			if (col == 8) rightIndentation = 0;
			if (row == 8) bottomIndentation = 0;
			
			cv::Rect rect = cv::Rect(0 + col * xDimIncrease - leftIndentation,
									 0 + row * yDimIncrease - upperIndentation,
									 xDimIncrease + leftIndentation + rightIndentation,
									 yDimIncrease + upperIndentation + bottomIndentation);
			digitsImages[row][col] = cv::Mat(imageToBeCut, rect).clone();
		}
	}
	return digitsImages;
}

// Finding Corners of a sudoku problem
void SudokuImageReader::findCorners(std::vector<std::vector<Point>> contours, std::vector<Point> &corners)
{
	corners.push_back(Point(contours[0][0]));							// upper left corner
	for (int step = 0; step < 3; step++) corners.push_back(Point(contours[0][0]));	// upper right / down left / down right corners

	int size = contours[0].size();
	for (int i = 0; i < size; i++)
	{
		if (contours[0][i].x + contours[0][i].y < corners[0].x + corners[0].y)
			corners[0] = Point(contours[0][i]);
		else if (contours[0][i].x - contours[0][i].y > corners[1].x - corners[1].y)
			corners[1] = Point(contours[0][i]);
		else if (contours[0][i].y - contours[0][i].x > corners[2].y - corners[2].x)
			corners[2] = Point(contours[0][i]);
		else if (contours[0][i].x + contours[0][i].y > corners[3].x + corners[3].y)
			corners[3] = Point(contours[0][i]);
	}

}