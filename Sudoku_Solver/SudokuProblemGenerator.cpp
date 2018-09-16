#include "stdafx.h"
#include "SudokuProblemGenerator.h"


SudokuProblemGenerator::SudokuProblemGenerator()
{
}

SudokuProblemGenerator::~SudokuProblemGenerator()
{
}



int** SudokuProblemGenerator::generateSudokuProblem()
{
	int generatedPuzzle[9][9] = { { 0, 3, 6, 4, 9, 2, 5, 8, 7},
								{ 0, 9, 4, 7, 6, 8, 1, 3, 2},
								{ 0, 2, 8, 3, 1, 5, 9, 6, 4},
								{ 0, 6, 1, 5, 7, 4, 2, 9, 3},
								{ 0, 7, 5, 9, 3, 1, 6, 4, 8},
								{ 0, 4, 9, 2, 8, 6, 7, 1, 5},
								{ 0, 5, 7, 1, 4, 3, 8, 2, 9},
								{ 0, 8, 2, 6, 5, 9, 3, 7, 1},
								{ 0, 1, 3, 8, 2, 7, 4, 5, 6}};

	int** sudokuProblem = new int*[9];
	for (int row = 0; row < 9; row++)
	{
		sudokuProblem[row] = new int[9];
		for (int column = 0; column < 9; column++)
		{
			sudokuProblem[row][column] = generatedPuzzle[row][column];
		}
	}
	return sudokuProblem;
}