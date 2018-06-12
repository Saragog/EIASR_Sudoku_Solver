#include "stdafx.h"
#include "DigitRecognizer.h"
#include "SudokuImageReader.h"

using namespace cv;

#define RESIZED_IMAGE_WIDTH 128
#define RESIZED_IMAGE_HEIGHT 128

DigitRecognizer::DigitRecognizer()
{
	//mozna zapisywac do pliku write ,read
	classifier = ml::KNearest::create();
	svm = ml::SVM::create();

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
	//return true;


	// Set up SVM for OpenCV 3
	// svm = ml::SVM::create();
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
	svm->train(td);

	//svm->trainAuto(td);

	return true;

	// Save trained model 
	//svm->save("digits_svm_model.yml");

	// Test on a held out test set
	//svm->predict(testMat, testResponse);
}

int DigitRecognizer::classify(Mat img)
{
	Mat matImageFlattened = preprocessSudokuDigitImage(img);
	Mat matResult(0, 0, CV_32F);

	classifier->findNearest(matImageFlattened, 3, matResult);
	//float result = matResult.at<float>(0, 0);



	return (int)(svm->predict(matImageFlattened.clone()));



	//return int(result);
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

cv::Mat DigitRecognizer::preprocessSudokuDigitImage(cv::Mat img) // TODO zmienić tak, aby dzialalo lepiej :)
{
	Mat imgResized;
	
	resize(img, imgResized, cv::Size(RESIZED_IMAGE_WIDTH, RESIZED_IMAGE_HEIGHT));

	HOGDescriptor hog(
		Size(RESIZED_IMAGE_WIDTH, RESIZED_IMAGE_HEIGHT),//120, 120), //winSize
		Size(64, 64),//10, 10),	//blocksize
		Size(32, 32),//5, 5),		//blockStride,
		Size(32, 32),//10, 10),	//cellSize,
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



	/*

	Moments m = moments(imgResized, true);

	//sprobowac z normalizacja rozmiaru i cala bitmapa do knn
	Mat matHuMoments;
	HuMoments(m, matHuMoments);
	
	*/

	Mat descriptorMat(1, 324, CV_32FC1, descriptor.data());

	//Mat matImageFloat;
	//matHuMoments.convertTo(matImageFloat, CV_32FC1);
	//Mat matImageFlattenedFloat = matImageFloat.reshape(1, 1);
	return descriptorMat.clone();// matImageFlattenedFloat;
}

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

	/*
	Moments m = moments(matThresh, true);
	Mat matHuMoments;
	HuMoments(m, matHuMoments);
	double huhu[9];
	HuMoments(m, huhu);
	
	
	Mat matImageFloat;
	matHuMoments.convertTo(matImageFloat, CV_32FC1);
	Mat matImageFlattenedFloat = matImageFloat.reshape(1, 1);
	return matImageFlattenedFloat;

	*/
	//return matHuMoments;
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

