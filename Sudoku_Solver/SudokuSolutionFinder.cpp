#include "stdafx.h"
#include "SudokuSolutionFinder.h"

#include "ImageWindowCreator.h"

using namespace cv;
SudokuSolutionFinder::SudokuSolutionFinder()
{
	digitRecognizer.train("train_data");
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

	// Detected numbers in original method

	// checking if the detected numbers make sense
	if (sudokuProblemSolver.checkIfSudokuProblemIsAppropriate(sudokuValues)) {  // Appropriate values
		ImageWindowCreator::showImage("Classification Result Image1", sudokuValues, true);
	}
	else { // Inappropriate values detected
		ImageWindowCreator::showImage("Classification Result Image1", sudokuValues, false);
	}
	ImageWindowCreator::showDetectedSudoku("Sudoku problem detected", sudokuValues);

	return;
}

/*
hierarchy classification todo
dodac typy obrazu do dokumentacji ok 20 obrazow testowych
za malo danych do rozpoznawania do nauczenia
zamienic na angielski i doslac
*/