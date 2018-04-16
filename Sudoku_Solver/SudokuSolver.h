#pragma once

#include "SudokuImageReader.h"
#include "SudokuProblemSolver.h"
#include "DigitRecognizer.h"
#include<opencv2/opencv.hpp> 

class SudokuSolver
{
public:
	SudokuSolver();
	~SudokuSolver();
	void solveProblem(cv::String problemPath);

private:
	SudokuImageReader sudokuImageReader;		// reads sudoku problem from an image
	SudokuProblemSolver sudokuProblemSolver;	// solves a given sudoku problem
	DigitRecognizer digitRecognizer;			// a machine learning model that can read digits from images
};

