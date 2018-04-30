#include "stdafx.h"
#include "SudokuSolver.h"

#include "ImageWindowCreator.h"

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
	// TODO implement other methods for detecting numbers and run checkIfSudokuProblemIsAppropriate(int**) function
	// for checking if the problem is appropriate
	// If all attempts fail (all the combinations of detected numbers are inappropriate) then we inform the user about it

	Mat** sudokuParts = sudokuImageReader.readSudokuFromImage(problemPath);
	if (sudokuParts == NULL) return;
	int** sudokuValues = digitRecognizer.classifyAll(sudokuParts);

	// Detected numbers in original method
	ImageWindowCreator::showImage("Classification Result Image1", sudokuValues);

	// checking if the detected numbers make sense
	if (sudokuProblemSolver.checkIfSudokuProblemIsAppropriate(sudokuValues)) {  // Appropriate values
		// TODO
	}
	else if (sudokuProblemSolver.checkIfSudokuProblemIsAppropriate(sudokuValues)){ // Inappropriate values detected
		// TODO
	}
	return;
}

/*
hierarchy classification todo
dodac typy obrazu do dokumentacji ok 20 obrazow testowych
za malo danych do rozpoznawania do nauczenia
zamienic na angielski i doslac
*/