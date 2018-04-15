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
	//int** sudokuProblem = initializeSudokuProblem();

	// read from image


	// TODO: Add your control notification handler code here
	cv::Mat imgOriginal;        // input image
	cv::Mat imgGrayscale;       // grayscale of input image
	cv::Mat imgBlurred;         // intermediate blured image
								//cv::Mat imgCanny;           // Canny edge image
	cv::Mat imgTreshold;		// tresholded image
	cv::Mat imgBitwiseNot;		// bitwise not image
	cv::Mat imgDilate;			// dilated image
	cv::Mat imgBiggestBlob;		// biggest blob image
	cv::Mat imgEroded;			// eroded back image
	cv::Mat imgDetectedLines;	// detected lines image
	cv::Mat imgDetectedCorners; // detected corners of sudoku puzzle
	cv::Mat imgPerspective;	// detected lines image

	//cv::Mat imgCrossingPoints[16]; // crossing points images
	cv::Mat** puzzleSquareDigitsImages; // images of where the digits should be
	cv::Mat** imgThresholdDigitsImages;	// images of thresholded digits images
	cv::Mat	  imgThresholdPerspective; // 

	//cv::ml::TrainData::create()
	//imgGrayscale.
	imgOriginal = cv::imread(path);

	if (imgOriginal.empty()) {                                  // if unable to open image
																//TODO messege box
		std::cout << "error: image not read from file\n\n";     // show error message on command line
		_getch();                                               // may have to modify this line if not using Windows
		return NULL;                                            // and exit program
	}

	cv::cvtColor(imgOriginal, imgGrayscale, CV_BGR2GRAY);       // convert to grayscale

	cv::GaussianBlur(imgGrayscale,          // input image
		imgBlurred,                         // output image
		cv::Size(11, 11),					// smoothing window width and height in pixels
		0);                               // sigma value, determines how much the image will be blurred

	cv::adaptiveThreshold(imgBlurred, imgTreshold, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 5, 1.5);

	/*
	cv::Canny(imgBlurred,           // input image
	imgCanny,                   // output image
	82,                         // low threshold
	164);                       // high threshold
	*/

	cv::bitwise_not(imgTreshold, imgBitwiseNot);

	cv::Mat kernel = (cv::Mat_<uchar>(3, 3) << 0, 1, 0, 1, 1, 1, 0, 1, 0);
	cv::dilate(imgBitwiseNot, imgDilate, kernel);
	

	// _____

	imgBiggestBlob = imgDilate.clone();

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
	cv::dilate(imgBiggestBlob, imgBiggestBlob, kernel);
	cv::dilate(imgBiggestBlob, imgBiggestBlob, kernel);
	cv::dilate(imgBiggestBlob, imgBiggestBlob, kernel);
	//TODO przeskalowywać obrazek rozna rozdzielczosc = rozne dzialanie dilude i innych

	erode(imgBiggestBlob, imgEroded, kernel);

	//cutImageWithPoints(imgEroded, imgCrossingPoints);

	std::vector<std::vector<Point>> contours;

	findContours(imgEroded, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	std::vector<Point> corners;
	Point2f cornersf[4];
	findCorners(contours, corners);
	imgDetectedCorners = imgOriginal.clone();

	for (int circleIndex = 0; circleIndex < 4; circleIndex++)
	{
		circle(imgDetectedCorners, corners[circleIndex], 20, CV_RGB(255, 0, 0), -1);
		cornersf[circleIndex] = Point2f(corners[circleIndex]);
	}

	Point2f dst[] = { Point2f(0,0),Point2f(FLAT_IMAGE_WIDTH,0), Point2f(0,FLAT_IMAGE_HEIGHT),Point2f(FLAT_IMAGE_WIDTH,FLAT_IMAGE_HEIGHT) }; //1080/1080

	Mat M = getPerspectiveTransform(cornersf, dst);
	warpPerspective(imgOriginal, imgPerspective, M, Size(FLAT_IMAGE_WIDTH, FLAT_IMAGE_HEIGHT));


	puzzleSquareDigitsImages = cutPuzzleImageIntoDigitsImages(imgPerspective); // cuts puzzle into ditits images

	prepareDigitImagesForDetection(puzzleSquareDigitsImages);

	warpPerspective(imgTreshold, imgThresholdPerspective, M, Size(FLAT_IMAGE_WIDTH, FLAT_IMAGE_HEIGHT));

	imgThresholdDigitsImages = cutPuzzleImageIntoDigitsImages(imgThresholdPerspective);

	// TODO using each of the images and ditit recognizer for digit recognition

	//puzzleSquareDigitsImages[0][3] = digitRecognizer.preprocessImage(puzzleSquareDigitsImages[0][3]);

	//drawContours(imgOriginal, contours, -1, CV_RGB(255, 0, 0));

	//imgDetectedLines = imgEroded.clone();

	//std::vector<Vec2f> lines;
	//HoughLines(imgDetectedLines, lines, 1, CV_PI / 180, 200);

	/*
	for (int i = 0; i<lines.size(); i++)
	{
	drawLine(lines[i], imgDetectedLines, CV_RGB(0, 0, 128));
	}
	*/


	// _____
	// declare windows
	cv::namedWindow("imgOriginal", CV_WINDOW_NORMAL);     // note: you can use CV_WINDOW_NORMAL which allows resizing the window
														  //cv::namedWindow("imgCanny", CV_WINDOW_AUTOSIZE);        // or CV_WINDOW_AUTOSIZE for a fixed size window matching the resolution of the image
														  // CV_WINDOW_AUTOSIZE is the default
	//cv::namedWindow("imgBlurred", CV_WINDOW_NORMAL);
	//cv::namedWindow("ingTreshold", CV_WINDOW_NORMAL);
	//cv::namedWindow("imgBitwiseNot", CV_WINDOW_NORMAL);
	//cv::namedWindow("imgDilate", CV_WINDOW_NORMAL);
	//cv::namedWindow("imgBiggestBlob", CV_WINDOW_NORMAL);
	//cv::namedWindow("imgEroded", CV_WINDOW_NORMAL);
	//cv::namedWindow("imgDetectedLines", CV_WINDOW_NORMAL);
	cv::namedWindow("imgPerspective", CV_WINDOW_NORMAL);
	cv::namedWindow("imgDigitImage", CV_WINDOW_NORMAL);
	cv::namedWindow("imgThresholdPerspective", CV_WINDOW_NORMAL);
	cv::namedWindow("imgThresholdDigitsImages", CV_WINDOW_NORMAL);

	cv::imshow("imgOriginal", imgOriginal);
	//cv::imshow("imgBlurred", imgBlurred);     // show windows
	//cv::imshow("ingTreshold", imgTreshold);
	//cv::imshow("imgBitwiseNot", imgBitwiseNot);
	//cv::imshow("imgDilate", imgDilate);
	//cv::imshow("imgBiggestBlob", imgBiggestBlob);
	//cv::imshow("imgEroded", imgEroded);
	//cv::imshow("imgDetectedLines", imgDetectedLines);
	cv::imshow("imgPerspective", imgPerspective);
	cv::imshow("imgDigitImage", puzzleSquareDigitsImages[0][3]);
	cv::imshow("imgThresholdPerspective", imgThresholdPerspective);
	cv::imshow("imgThresholdDigitsImages", imgThresholdDigitsImages[0][3]);

	// ________

	return puzzleSquareDigitsImages;
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


void SudokuImageReader::drawLine(cv::Vec2f line, cv::Mat &img, cv::Scalar rgb)
{
	if (line[1] != 0)
	{
		float m = -1 / tan(line[1]);

		float c = line[0] / sin(line[1]);

		cv::line(img, Point(0, c), Point(img.size().width, m*img.size().width + c), rgb);
	}
	else
	{
		cv::line(img, Point(line[0], 0), Point(line[0], img.size().height), rgb);
	}

}

void SudokuImageReader::prepareDigitImagesForDetection(cv::Mat** puzzleSquareDigitImages)
{
	for (int row = 0; row < 9; row++)
		for (int col = 0; col < 9; col++)
		{
			cv::cvtColor(puzzleSquareDigitImages[row][col], puzzleSquareDigitImages[row][col], CV_BGR2GRAY);       // convert to grayscale

			cv::GaussianBlur(puzzleSquareDigitImages[row][col],         // input image
				puzzleSquareDigitImages[row][col],						// output image
				cv::Size(11, 11),										// smoothing window width and height in pixels
				0);														// sigma value, determines how much the image will be blurred


			cv::adaptiveThreshold(puzzleSquareDigitImages[row][col], puzzleSquareDigitImages[row][col],
				255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 5, 1.5);
			

			cv::Mat kernel = (cv::Mat_<uchar>(3, 3) << 0, 1, 0, 1, 1, 1, 0, 1, 0);

			cv::bitwise_not(puzzleSquareDigitImages[row][col], puzzleSquareDigitImages[row][col]);
			cv::dilate(puzzleSquareDigitImages[row][col], puzzleSquareDigitImages[row][col], kernel);
			cv::dilate(puzzleSquareDigitImages[row][col], puzzleSquareDigitImages[row][col], kernel);
			cv::dilate(puzzleSquareDigitImages[row][col], puzzleSquareDigitImages[row][col], kernel);
			cv::dilate(puzzleSquareDigitImages[row][col], puzzleSquareDigitImages[row][col], kernel); //TODO porawic to jakos
			removeBorderFromImage(puzzleSquareDigitImages[row][col]);

			std::vector<std::vector<Point>> contours;
			

			findContours(puzzleSquareDigitImages[row][col], contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
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
			}


			//convertColourImagesToGrayScale(puzzleSquareDigitImages[row][col]);
			//blurColourImagesToGrayScale(puzzleSquareDigitImages);
			//thresholdDigitImages(puzzleSquareDigitImages);
			//dilateDigitImages(puzzleSquareDigitImages);
			//erodeDigitImages(puzzleSquareDigitImages);
			//removeBorders(puzzleSquareDigitImages);
		}

}

/*
void SudokuImageReader::convertColourImagesToGrayScale(cv::Mat** puzzleSquareDigitImages)
{

			cv::cvtColor(puzzleSquareDigitImages[row][col], puzzleSquareDigitImages[row][col], CV_BGR2GRAY);       // convert to grayscale
}

void SudokuImageReader::blurColourImagesToGrayScale(cv::Mat** puzzleSquareDigitImages)
{
	for (int row = 0; row < 9; row++)
		for (int col = 0; col < 9; col++)
			cv::GaussianBlur(puzzleSquareDigitImages[row][col],         // input image
				puzzleSquareDigitImages[row][col],						// output image
				cv::Size(11, 11),										// smoothing window width and height in pixels
				0);														// sigma value, determines how much the image will be blurred
}

void SudokuImageReader::thresholdDigitImages(cv::Mat** puzzleSquareDigitImages)
{
	cv::Mat temp;
	for (int row = 0; row < 9; row++)
		for (int col = 0; col < 9; col++)
		{
			cv::adaptiveThreshold(puzzleSquareDigitImages[row][col], temp,
				255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 5, 1.5);
			puzzleSquareDigitImages[row][col] = temp;
		}

}

void SudokuImageReader::dilateDigitImages(cv::Mat** puzzleSquareDigitImages)
{
	cv::Mat kernel = (cv::Mat_<uchar>(3, 3) << 0, 1, 0, 1, 1, 1, 0, 1, 0);
	for (int row = 0; row < 9; row++)
		for (int col = 0; col < 9; col++)
		{
			cv::bitwise_not(puzzleSquareDigitImages[row][col], puzzleSquareDigitImages[row][col]);
			cv::dilate(puzzleSquareDigitImages[row][col], puzzleSquareDigitImages[row][col], kernel);
		}
}

void SudokuImageReader::erodeDigitImages(cv::Mat** puzzleSquareDigitImages)
{
	cv::Mat kernel = (cv::Mat_<uchar>(3, 3) << 0, 1, 0, 1, 1, 1, 0, 1, 0);
	for (int row = 0; row < 9; row++)
		for (int col = 0; col < 9; col++)
			erode(puzzleSquareDigitImages[row][col], puzzleSquareDigitImages[row][col], kernel);
}

void SudokuImageReader::removeBorders(cv::Mat** puzzleSquareDigitImages)
{
	for (int row = 0; row < 9; row++)
		for (int col = 0; col < 9; col++)
			removeBorderFromImage(puzzleSquareDigitImages[row][col]);
}
*/
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


Mat** SudokuImageReader::cutPuzzleImageIntoDigitsImages(cv::Mat imageToBeCut)
{
	cv::Mat** digitsImages = new cv::Mat*[9];
	//cv::Mat digitsImages[9][9];
	for (int row = 0; row < 9; row++)
		digitsImages[row] = new cv::Mat[9]();

	int yDimSize = imageToBeCut.size().height;
	int xDimSize = imageToBeCut.size().width;
	int yDimIncrease = yDimSize / 9;
	int xDimIncrease = xDimSize / 9;

	int upperIndentation, leftIndentation, rightIndentation, bottomIndentation;

	for (int row = 0; row < 9; row++)
	{
		for (int col = 0, 
			upperIndentation = 20,
			leftIndentation = 20,
			rightIndentation = 20,
			bottomIndentation = 20;
			col < 9; col++, 
			upperIndentation = 20, leftIndentation = 20, rightIndentation = 20, bottomIndentation = 20)
		{
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