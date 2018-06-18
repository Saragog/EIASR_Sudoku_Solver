/*
	Creators Andrzej Dackiewicz, Mateusz Jarzemski
	This file is a part of a university project Sudoku Solver
	that was developed as a EIASR project.
	The aim is to create a software program, that is capable of recognizing sudoku problems
	read from photographs. The program is using OpenCV library.

	This file describes a class that is used for the whole process of finding the sudoku solution.
	So far we are capable of recognizing the digits drawn on most of the pictures.
	The code responsible for Sudoku solution has yet to be created.
*/

#include "stdafx.h"
#include "SudokuSolutionFinder.h"
#include "ImageWindowCreator.h"
using namespace cv;

// The constructor prepares both recognizers
// The main recognizer and the secondary recognizer for detecting ones
SudokuSolutionFinder::SudokuSolutionFinder()
{
	digitRecognizer.train("train_data");
	redundantOnesRecognizer.train("train_data2");
}


SudokuSolutionFinder::~SudokuSolutionFinder()
{
}


// This function reads the image from a file in specified path and runs classificators
void SudokuSolutionFinder::solveProblem(String problemPath)
{
	Mat** sudokuParts = sudokuImageReader.readSudokuFromImage(problemPath);
	if (sudokuParts == NULL) return;
	int** sudokuValues = digitRecognizer.classifyAll(sudokuParts);
	redundantOnesRecognizer.classifyOnes(sudokuParts, sudokuValues);
	// Detected numbers in original method

	ImageWindowCreator::showDetectedSudoku("Sudoku problem detected", sudokuValues);

	// checking if the detected numbers make sense
	if (sudokuProblemSolver.checkIfSudokuProblemIsAppropriate(sudokuValues)) {  // Appropriate values
		ImageWindowCreator::showImage("Classification Result Image1", sudokuValues, true);
		sudokuProblemSolver.solveSudokuProblem(sudokuValues);
	}
	else { // Inappropriate values detected
		ImageWindowCreator::showImage("Classification Result Image1", sudokuValues, false);
	}

	return;
}