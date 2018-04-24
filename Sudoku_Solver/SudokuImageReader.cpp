#include "stdafx.h"
#include "SudokuImageReader.h"
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
																//TODO messege box
		std::cout << "error: image not read from file\n\n";     // show error message on command line
		_getch();                                               // may have to modify this line if not using Windows
		return NULL;                                            // and exit program
	}

	cv::namedWindow("imgOriginal", CV_WINDOW_NORMAL);
	cv::imshow("imgOriginal", imgOriginal);

	imgPerspective = findAndRemovePerspective(imgOriginal);


	puzzleSquareDigitsImages = cutPuzzleImageIntoDigitsImages(imgPerspective); // cuts puzzle into ditits images

	prepareDigitImagesForDetection(puzzleSquareDigitsImages);

	
	// _____
	// declare windows
	cv::namedWindow("imgOriginal", CV_WINDOW_NORMAL);     
	cv::imshow("imgOriginal", imgOriginal);													  
														  
	
	// ________

	return puzzleSquareDigitsImages;
}

Mat SudokuImageReader::joinImagesIntoOne(cv::Mat** images) // testing method for showing progress in whole image
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
	// TODO show sudoku solution
	// TODO clean the sudoku problem 2d array somewhere (delete)
	return wholeImage;

}

int** SudokuImageReader::initializeSudokuProblem()
{
	int** sudokuProblem = new int*[9];
	for (int row = 0; row < 9; ++row)
	{
		sudokuProblem[row] = new int[9];
		for (int col = 0; col < 9; ++col)
			sudokuProblem[row][col] = 0;
	}
	return sudokuProblem;
}


//void SudokuImageReader::drawLine(cv::Vec2f line, cv::Mat &img, cv::Scalar rgb)
//{
//	if (line[1] != 0)
//	{
//		float m = -1 / tan(line[1]);
//
//		float c = line[0] / sin(line[1]);
//
//		cv::line(img, Point(0, c), Point(img.size().width, m*img.size().width + c), rgb);
//	}
//	else
//	{
//		cv::line(img, Point(line[0], 0), Point(line[0], img.size().height), rgb);
//	}
//
//}

void SudokuImageReader::prepareDigitImagesForDetection(cv::Mat** puzzleSquareDigitImages)
{
	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 9; col++)
		{
			cv::cvtColor(puzzleSquareDigitImages[row][col], puzzleSquareDigitImages[row][col], CV_BGR2GRAY);       // convert to grayscale

			cv::GaussianBlur(puzzleSquareDigitImages[row][col],         // input image
				puzzleSquareDigitImages[row][col],						// output image
				cv::Size(5, 5),										// smoothing window width and height in pixels
				0);														// sigma value, determines how much the image will be blurred
			

			cv::adaptiveThreshold(puzzleSquareDigitImages[row][col], puzzleSquareDigitImages[row][col],
				255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 19, 2.5);
			
		}
	}

	Mat combinedParts = joinImagesIntoOne(puzzleSquareDigitImages);
	cv::namedWindow("combinedParts Threshold", CV_WINDOW_NORMAL);
	cv::imshow("combinedParts Threshold", combinedParts);
	
	//cv::Mat kernel = (cv::Mat_<uchar>(5, 5) << 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0);
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

	combinedParts = joinImagesIntoOne(puzzleSquareDigitImages);
	cv::namedWindow("combinedParts removeBorder", CV_WINDOW_NORMAL);
	cv::imshow("combinedParts removeBorder", combinedParts);

	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 9; col++)
		{

			

			
			//erode(puzzleSquareDigitImages[row][col], puzzleSquareDigitImages[row][col], kernel);

			

			//cv::imshow("imgPerspective", imgPerspective);
			//cv::imshow("imgDigitImage", puzzleSquareDigitsImages[0][3]);
			//cv::imshow("imgThresholdPerspective", imgThresholdPerspective);
			//cv::imshow("imgThresholdDigitsImages", imgThresholdDigitsImages[0][3]);

			std::vector<std::vector<Point>> contours;


			cv::findContours(puzzleSquareDigitImages[row][col], contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
			if (!contours.empty())
			{
				cv::Moments m = moments(contours[0], true);

				int middleX = (int)(m.m10 / m.m00), middleY = (int)(m.m01 / m.m00);

				int halfOfHeight = puzzleSquareDigitImages[row][col].size().height / 2;
				int halfOfWidth = puzzleSquareDigitImages[row][col].size().width / 2;

				int deltaX = halfOfWidth - middleX;
				int deltaY = halfOfHeight - middleY;

				Mat translationMatrix = (Mat_<double>(2, 3) << 1, 0, deltaX, 0, 1, deltaY);
				warpAffine(puzzleSquareDigitImages[row][col], puzzleSquareDigitImages[row][col], translationMatrix, puzzleSquareDigitImages[row][col].size());
				//TODO sprawdzic skalowanie do wielkosci
				
				erode(puzzleSquareDigitImages[row][col], puzzleSquareDigitImages[row][col], kernel);
			}
		}
	}

}



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


cv::Mat SudokuImageReader::findAndRemovePerspective(cv::Mat input)
{
	cv::Mat imgBlurred;         // intermediate blured image
	cv::Mat imgTreshold;		// tresholded image
	cv::Mat imgBiggestBlob;

	resize(input, imgBlurred, cv::Size((input.size().width*1200)/input.size().height, 1200)); // przeskalowanie z zachowaniem proporcji

	cv::cvtColor(imgBlurred, imgBlurred, CV_BGR2GRAY);       // convert to grayscale

	cv::GaussianBlur(imgBlurred,          // input image
		imgBlurred,                         // output image
		cv::Size(11, 11),					// smoothing window width and height in pixels
		0);                               // sigma value, determines how much the image will be blurred

	cv::namedWindow("imgBlurred", CV_WINDOW_NORMAL);
	cv::imshow("imgBlurred", imgBlurred);

	cv::adaptiveThreshold(imgBlurred, imgTreshold, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY_INV, 5, 1.5);

	cv::namedWindow("imgTreshold", CV_WINDOW_NORMAL);
	cv::imshow("imgTreshold", imgTreshold);

	//cv::bitwise_not(imgTreshold, imgBitwiseNot);

	cv::Mat kernel = (cv::Mat_<uchar>(3, 3) << 0, 1, 0, 1, 1, 1, 0, 1, 0);

	cv::dilate(imgTreshold, imgTreshold, kernel);

	cv::namedWindow("imgTresholdDilate", CV_WINDOW_NORMAL);
	cv::imshow("imgTresholdDilate", imgTreshold);
	
	imgBiggestBlob = findBiggestBlob(imgTreshold);

	cv::namedWindow("imgBiggestBlob", CV_WINDOW_NORMAL);
	cv::imshow("imgBiggestBlob", imgBiggestBlob);

	

	
	//cv::dilate(imgBiggestBlob, imgBiggestBlob, kernel);
	//cv::dilate(imgBiggestBlob, imgBiggestBlob, kernel);
	//cv::dilate(imgBiggestBlob, imgBiggestBlob, kernel);
	

	//erode(imgBiggestBlob, imgBiggestBlob, kernel);

	

	std::vector<std::vector<Point>> contours;

	findContours(imgBiggestBlob, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	std::vector<Point> corners;
	Point2f cornersf[4];

	findCorners(contours, corners);
	//TODO sprawdzenie czy punkty sa dobre

	Mat imgDetectedCorners = imgBlurred.clone();
	cv::cvtColor(imgDetectedCorners, imgDetectedCorners, CV_GRAY2BGR);

	float resizeRatio = (float)input.size().height / 1200;
	for (int circleIndex = 0; circleIndex < 4; circleIndex++)
	{
		circle(imgDetectedCorners, corners[circleIndex], 5, CV_RGB(255, 0, 0), -1);
		cornersf[circleIndex] = Point2f(corners[circleIndex].x *resizeRatio, corners[circleIndex].y*resizeRatio);
	}

	cv::namedWindow("imgDetectedCorners", CV_WINDOW_NORMAL);
	cv::imshow("imgDetectedCorners", imgDetectedCorners);

	Point2f dst[] = { Point2f(0,0),Point2f(FLAT_IMAGE_WIDTH,0), Point2f(0,FLAT_IMAGE_HEIGHT),Point2f(FLAT_IMAGE_WIDTH,FLAT_IMAGE_HEIGHT) }; //1080/1080

	Mat imgPerspective;
	Mat M = getPerspectiveTransform(cornersf, dst);
	warpPerspective(input, imgPerspective, M, Size(FLAT_IMAGE_WIDTH, FLAT_IMAGE_HEIGHT));

	cv::namedWindow("imgPerspective", CV_WINDOW_NORMAL);
	cv::imshow("imgPerspective", imgPerspective);

	return imgPerspective;
}

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

	// TODO remember to clean that digitsImages array !!!

	return digitsImages;
}

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