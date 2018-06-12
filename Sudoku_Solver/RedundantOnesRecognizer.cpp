#include "stdafx.h"
#include "RedundantOnesRecognizer.h"

#define RESIZED_IMAGE_WIDTH 128
#define RESIZED_IMAGE_HEIGHT 128

using namespace cv;

RedundantOnesRecognizer::RedundantOnesRecognizer()
{
}


RedundantOnesRecognizer::~RedundantOnesRecognizer()
{
}

void RedundantOnesRecognizer::classifyOnes(cv::Mat ** images, int ** sudokuValues)
{
	for (int row = 0; row < 9; row++)
	{
		
		for (int col = 0; col < 9; col++)
		{
			if(sudokuValues[row][col] == 1)
				sudokuValues[row][col] = classify(images[row][col]);
		}
	}
}

cv::Mat RedundantOnesRecognizer::preprocessSudokuDigitImage(cv::Mat img)
{
	Mat imgResized;

	resize(img, imgResized, cv::Size(RESIZED_IMAGE_WIDTH, RESIZED_IMAGE_HEIGHT));

	HOGDescriptor hog(
		Size(RESIZED_IMAGE_WIDTH, RESIZED_IMAGE_HEIGHT), //winSize
		Size(32, 32),	//blocksize
		Size(16, 16),	//blockStride,
		Size(16, 16),	//cellSize,
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

void RedundantOnesRecognizer::prepareTraining(cv::Mat * trainingImages, cv::Mat * classificationInts, cv::String path)
{

	prepareOneFolder(trainingImages, classificationInts, path, 0);
	prepareOneFolder(trainingImages, classificationInts, path, 1);
	prepareOneFolder(trainingImages, classificationInts, path, 7);


}

void RedundantOnesRecognizer::prepareOneFolder(cv::Mat * trainingImages, cv::Mat * classificationInts, cv::String path, int num)
{

	char folder = num + '0';
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
		classificationInts->push_back(num);
	}


}
