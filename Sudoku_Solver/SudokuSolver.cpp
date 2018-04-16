#include "stdafx.h"
#include "SudokuSolver.h"

using namespace cv;
SudokuSolver::SudokuSolver()
{
	digitRecognizer.train("train_data");
}


SudokuSolver::~SudokuSolver()
{
}

void SudokuSolver::solveProblem(String problemPath)
{
	Mat** sudokuParts = sudokuImageReader.readSudokuFromImage(problemPath);
	digitRecognizer.classifyAll(sudokuParts);
	Mat classificationResultImage = SudokuImageReader::joinImagesIntoOne(sudokuParts);
	cv::imshow("imgThresholdDigitsImages", classificationResultImage);
	return;
}
