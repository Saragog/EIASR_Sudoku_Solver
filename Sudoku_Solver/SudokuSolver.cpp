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
	if (sudokuParts == NULL) return;
	Mat recognitionResult = digitRecognizer.classifyAll(sudokuParts);
	cv::namedWindow("classificationResultImage", CV_WINDOW_NORMAL);
	cv::imshow("classificationResultImage", recognitionResult);
	return;
}
