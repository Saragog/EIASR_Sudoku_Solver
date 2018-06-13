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
SudokuSolutionFinder::SudokuSolutionFinder()
{
	digitRecognizer.train("train_data");
	redundantOnesRecognizer.train("train_data2");
}


SudokuSolutionFinder::~SudokuSolutionFinder()
{
}

void SudokuSolutionFinder::solveProblem(String problemPath)
{
	// TODO implement other methods for detecting numbers and run checkIfSudokuProblemIsAppropriate(int**) function
	// for checking if the problem is appropriate
	// If all attempts fail (all the combinations of detected numbers are inappropriate) then we inform the user about it

	Mat** sudokuParts = sudokuImageReader.readSudokuFromImage(problemPath);
	if (sudokuParts == NULL) return;
	int** sudokuValues = digitRecognizer.classifyAll(sudokuParts);
	redundantOnesRecognizer.classifyOnes(sudokuParts, sudokuValues);
	// Detected numbers in original method

	// checking if the detected numbers make sense
	//if (sudokuProblemSolver.checkIfSudokuProblemIsAppropriate(sudokuValues)) {  // Appropriate values
	//	ImageWindowCreator::showImage("Classification Result Image1", sudokuValues, true);
	//}
	//else { // Inappropriate values detected
	//	ImageWindowCreator::showImage("Classification Result Image1", sudokuValues, false);
	//}
	ImageWindowCreator::showDetectedSudoku("Sudoku problem detected", sudokuValues);

	return;
}



/*
hierarchy classification todo
dodac typy obrazu do dokumentacji ok 20 obrazow testowych
za malo danych do rozpoznawania do nauczenia
zamienic na angielski i doslac
*/