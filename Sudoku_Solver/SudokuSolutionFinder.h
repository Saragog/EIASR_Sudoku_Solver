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

#pragma once

#include "SudokuImageReader.h"
#include "SudokuProblemSolver.h"
#include "DigitRecognizer.h"
#include <opencv2/opencv.hpp> 
#include "RedundantOnesRecognizer.h"

class SudokuSolutionFinder
{
public:
	SudokuSolutionFinder();
	~SudokuSolutionFinder();
	void solveProblem(cv::String problemPath);


private:
	SudokuImageReader sudokuImageReader;		// reads sudoku problem from an image
	SudokuProblemSolver sudokuProblemSolver;	// solves a given sudoku problem
	DigitRecognizer digitRecognizer;			// a clasifier that can read digits from images
	RedundantOnesRecognizer redundantOnesRecognizer;

};

