#include "stdafx.h"
#include "DigitRecognizer.h"

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

void DigitRecognizer::classifyAll(cv::Mat** images) // testing function for classification of 2d arrays of images
{
	for (int row = 0; row < 9; row++)
		for (int col = 0; col < 9; col++)
		{
			int a = classify(images[row][col]);
			putText(images[row][col], std::to_string(a), Point(10, 40), cv::FONT_HERSHEY_COMPLEX, 1, CV_RGB(0, 0, 64));
		}

	return;
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

