#include "stdafx.h"
#include "DigitRecognizer.h"
#include "SudokuImageReader.h"

using namespace cv;

#define RESIZED_IMAGE_WIDTH 120
#define RESIZED_IMAGE_HEIGHT 120

DigitRecognizer::DigitRecognizer()
{
	//mozna zapisywac do pliku write ,read
	classifier = ml::KNearest::create();
	

}

DigitRecognizer::~DigitRecognizer()
{
	classifier.release();
}

bool DigitRecognizer::train(cv::String path)
{
	Mat matTrainingImagesFlattened;
	Mat matClassificationInts;

	prepareTraining(&matTrainingImagesFlattened, &matClassificationInts, path);

	classifier->train(matTrainingImagesFlattened, ml::ROW_SAMPLE, matClassificationInts);
	return true;
}

int DigitRecognizer::classify(Mat img)
{
	Mat matImageFlattened = preprocessClassImage(img);
	Mat matResult(0, 0, CV_32F);
	classifier->findNearest(matImageFlattened, 1, matResult);
	float result = matResult.at<float>(0, 0);
	return int(result);
}

int** DigitRecognizer::classifyAll(cv::Mat** images) // testing function for classification of 2d arrays of images
{
	int** classificationResults = new int*[9];
	for (int row = 0; row < 9; row++)
	{
		classificationResults[row] = new int[9];
		for (int col = 0; col < 9; col++)
		{
			classificationResults[row][col] = classify(images[row][col]);
		}
	}
	delete images;
	return classificationResults;
}

cv::Mat DigitRecognizer::createClassificationResultImage(int** classificationResults)
{
	cv::Mat classificationResult;
	cv::Mat** resultImageParts = initializeResultImageParts();
	for (int row = 0; row < 9; row++)
		for (int col = 0; col < 9; col++)
			putText(resultImageParts[row][col], std::to_string(classificationResults[row][col]), Point(10, 40), cv::FONT_HERSHEY_COMPLEX, 1, CV_RGB(0, 0, 255));
	classificationResult = SudokuImageReader::joinImagesIntoOne(resultImageParts);
	
	delete resultImageParts;

	return classificationResult;
}

cv::Mat** DigitRecognizer::initializeResultImageParts()
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

cv::Mat DigitRecognizer::preprocessClassImage(cv::Mat img)
{
	Mat imgResized;
	
	resize(img, imgResized, cv::Size(RESIZED_IMAGE_WIDTH, RESIZED_IMAGE_HEIGHT));
	cv::bitwise_not(imgResized, imgResized);
	Mat matImageFloat;
	imgResized.convertTo(matImageFloat, CV_32FC1);
	Mat matImageFlattenedFloat = matImageFloat.reshape(1, 1);

	return matImageFlattenedFloat;
}

Mat DigitRecognizer::preprocessImage(Mat img)
{
	Mat matGrayscale;           
	Mat matBlurred;            
	Mat matThresh; 

	cvtColor(img, matGrayscale, CV_BGR2GRAY);
	GaussianBlur(matGrayscale, matBlurred, cv::Size(5, 5), 0);    
	adaptiveThreshold(matBlurred, matThresh, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 11, 2);

	Mat matImageFloat;
	matThresh.convertTo(matImageFloat, CV_32FC1);
	Mat matImageFlattenedFloat = matImageFloat.reshape(1, 1);

	return matImageFlattenedFloat;
}

void DigitRecognizer::addTrainingImage(cv::Mat * trainingImages, cv::Mat img)
{
	
	Mat imgResized;
	resize(img, imgResized, cv::Size(RESIZED_IMAGE_WIDTH, RESIZED_IMAGE_HEIGHT)); //ewentualnie zakomentowac
	
	Mat matImageFlattenedFloat = preprocessImage(imgResized);
	trainingImages->push_back(matImageFlattenedFloat);
}

void DigitRecognizer::prepareTraining(cv::Mat * trainingImages, cv::Mat * classificationInts, cv::String path)
{
	//pliki

	for (int i = 0; i < 10; i++) //cyfry
	{
		char folder = i + '0';
		String tempPath = path + "\\" + folder + "\\*.png"; // TODO poprawic jak bedzie wiadomo jakie pliki
		std::vector<String> filenames;

		glob(tempPath, filenames); 

		for (int a = 0; a < filenames.size(); ++a)
		{
			Mat src = imread(filenames[a]);
			if (src.empty())
			{
				std::cout << "error: image not read from file\n\n";
				continue;
			}
			addTrainingImage(trainingImages, src);
			classificationInts->push_back(i);


		}

	}
}

