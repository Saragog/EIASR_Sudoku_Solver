﻿/*
Creators Andrzej Dackiewicz, Mateusz Jarzemski
This file is a part of a university project Sudoku Solver
that was developed as a EIASR project.
The aim is to create a software program, that is capable of recognizing sudoku problems
read from photographs. The program is using OpenCV library.

This file describes a class that is used for digit recognition.
*/

#include "stdafx.h"
#include "DigitRecognizer.h"
#include "SudokuImageReader.h"

using namespace cv;

#define RESIZED_IMAGE_WIDTH 128
#define RESIZED_IMAGE_HEIGHT 128

DigitRecognizer::DigitRecognizer()
{
	
	svm = ml::SVM::create();

}

DigitRecognizer::~DigitRecognizer()
{
	svm.release();
}

// Training of model
bool DigitRecognizer::train(cv::String path)
{
	Mat matTrainingImagesFlattened;
	Mat matClassificationInts;

	prepareTraining(&matTrainingImagesFlattened, &matClassificationInts, path);

	// Set SVM type
	svm->setType(ml::SVM::C_SVC);
	// Set SVM Kernel to Radial Basis Function (RBF) 
	svm->setKernel(ml::SVM::RBF);
	// Set parameter C
	svm->setC(50.5);
	// Set parameter Gamma
	svm->setGamma(0.50625);

	// Train SVM on training data 
	Ptr<ml::TrainData> td = ml::TrainData::create(matTrainingImagesFlattened, ml::ROW_SAMPLE, matClassificationInts);
	//svm->train(td);
	svm->trainAuto(td);

	return true;	
}

// Classification of a single digit image
int DigitRecognizer::classify(Mat img)
{
	Mat matImageFlattened = preprocessSudokuDigitImage(img);
	return (int)(svm->predict(matImageFlattened));
}

// Classifies all the single digit images
// Returns an array of detected digits (0 for empty fields)
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
	return classificationResults;
}

// Preprocesses single image digits
cv::Mat DigitRecognizer::preprocessSudokuDigitImage(cv::Mat img) 
{
	Mat imgResized;
	
	resize(img, imgResized, cv::Size(RESIZED_IMAGE_WIDTH, RESIZED_IMAGE_HEIGHT));

	HOGDescriptor hog(
		Size(RESIZED_IMAGE_WIDTH, RESIZED_IMAGE_HEIGHT), //winSize
		Size(64, 64),	//blocksize
		Size(32, 32),	//blockStride,
		Size(32, 32),	//cellSize,
		9, //nbins,
		1, //derivAper,
		-1, //winSigma,
		0, //histogramNormType,
		0.2, //L2HysThresh,
		1,//gammal correction,
		64,//nlevels=64
		1);//Use signed gradients 

	std::vector<float> descriptor;
	hog.compute(imgResized, descriptor);	

	Mat descriptorMat(1, descriptor.size(), CV_32FC1, descriptor.data());
	
	return descriptorMat.clone();
}

// Preprocesses images
Mat DigitRecognizer::preprocessImage(Mat img)
{
	Mat matGrayscale;           
	Mat matBlurred;            
	Mat matThresh; 

	cvtColor(img, matGrayscale, CV_BGR2GRAY);
	GaussianBlur(matGrayscale, matBlurred, cv::Size(5, 5), 0);    
	adaptiveThreshold(matBlurred, matThresh, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 11, 2);
	
	Mat test = preprocessSudokuDigitImage(matThresh);
	return test;
		
}

// Adds training images
void DigitRecognizer::addTrainingImage(cv::Mat * trainingImages, cv::Mat img)
{
	
	Mat imgResized;
	resize(img, imgResized, cv::Size(RESIZED_IMAGE_WIDTH, RESIZED_IMAGE_HEIGHT)); //ewentualnie zakomentowac
	
	Mat matImageFlattenedFloat = preprocessImage(imgResized);

	trainingImages->push_back(matImageFlattenedFloat);
}

// adds files for learning algorythm
void DigitRecognizer::prepareTraining(cv::Mat * trainingImages, cv::Mat * classificationInts, cv::String path)
{
	for (int i = 0; i < 10; i++) //digits
	{
		char folder = i + '0';
		String tempPath = path + "\\" + folder + "\\*.png";
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

