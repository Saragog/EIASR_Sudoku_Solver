/*
	Creators Andrzej Dackiewicz, Mateusz Jarzemski
	This file is a part of a university project Sudoku Solver
	that was developed as a EIASR project.
	The aim is to create a software program, that is capable of recognizing sudoku problems
	read from photographs. The program is using OpenCV library.

	This file describes a class that is used for checking if the recognized placement of digits
	is appropriate to be a representative of a sudoku problem. This class will be later on improved with
	the power to solve given sudoku problems.
*/

#include "stdafx.h"
#include "SudokuProblemSolver.h"


SudokuProblemSolver::SudokuProblemSolver()
{

}


SudokuProblemSolver::~SudokuProblemSolver()
{

}

// Function that could be implemented in the future
// It would solve the sudoku problem that was read from an image file
void SudokuProblemSolver::solveSudokuProblem(int** sudokuProblem)
{
	initializeProbabilities();
	crossHatch();
	bruteForceTheRest();

}

void SudokuProblemSolver::initializeProbabilities()
{
	for (int row = 0; row < 9; row++)
		for (int column = 0; column < 9; column++)
			possibilitiesForFields[row][column] = PossibilitiesForAField();
}

void SudokuProblemSolver::findPossibilitiesForOneField(int row, int column)
{
	PossibilitiesForAField possibilities = PossibilitiesForAField();
	// TODO sprawdzanie po rzedzie
	// TODO sprawdzanie po kolumnie
	// TODO sprawdzanie po gridzie
	//PossibilitiesForAField possibilitiesInRow = ...
	
}

PossibilitiesForAField SudokuProblemSolver::sumPossibilitiesForAField(PossibilitiesForAField possibilitiesInRow,
	PossibilitiesForAField possibilitiesInColumn,
	PossibilitiesForAField possibilitiesInGrid)
{
	return PossibilitiesForAField();
}

PossibilitiesForAField SudokuProblemSolver::findPossibilitiesForFieldInRow(int row)
{
	// for (int)
	// TODO
	return PossibilitiesForAField();
}

PossibilitiesForAField SudokuProblemSolver::findPossibilitiesForFieldInColumn(int column)
{
	// TODO
	return PossibilitiesForAField();
}

PossibilitiesForAField SudokuProblemSolver::findPossibilitiesForFieldInGrid(int row, int column)
{
	// TODO
	return PossibilitiesForAField();
}

void SudokuProblemSolver::findPossibilitiesForAllFields()
{

}

void SudokuProblemSolver::crossHatch()
{

}

void SudokuProblemSolver::bruteForceTheRest()
{

}



// Function that checks if the sudoku problem is appropriate
bool SudokuProblemSolver::checkIfSudokuProblemIsAppropriate(int** sudokuProblem)
{
	
	sudokuProblemValues = sudokuProblem;
	if (checkRows() && checkColumns() && checkGrids()) return true;
	else return false;
}

// Function that checks rows for digit duplications
bool SudokuProblemSolver::checkRows()
{
	for (int rowIndex = 0; rowIndex < 9; rowIndex++)
		if (!checkRow(rowIndex)) return false;
	return true;
}

// Function that checks columns for digit duplications
bool SudokuProblemSolver::checkColumns()
{
	for (int columnIndex = 0; columnIndex < 9; columnIndex++)
		if (!checkColumn(columnIndex)) return false;
	return true;
}

// Function that checks one row for digit duplications
bool SudokuProblemSolver::checkColumn(int columnIndex)
{
	bool columnUsedValues[9];
	int value;
	for (int valueIndex = 0; valueIndex < 9; valueIndex++)
		columnUsedValues[valueIndex] = false;
	for (int rowIndex = 0; rowIndex < 9; rowIndex++)
	{
		value = sudokuProblemValues[rowIndex][columnIndex];
		if (value != 0)
			if (columnUsedValues[value - 1]) return false;
			else columnUsedValues[value - 1] = true;
	}
	return true;
}

// Function that checks one column for digit duplications
bool SudokuProblemSolver::checkRow(int rowIndex)
{
	bool rowUsedValues[9];
	int value;
	for (int valueIndex = 0; valueIndex < 9; valueIndex++)
		rowUsedValues[valueIndex] = false;
	for (int columnIndex = 0; columnIndex < 9; columnIndex++)
	{
		value = sudokuProblemValues[rowIndex][columnIndex];
		if (value != 0)
			if (rowUsedValues[value - 1]) return false;
			else rowUsedValues[value - 1] = true;

	}
	return true;
}

// Function that checks all grids for digit duplications
bool SudokuProblemSolver::checkGrids()
{
	for (int gridIndex = 0; gridIndex < 9; gridIndex++)
		if (!checkGrid(gridIndex)) return false;
	return true;
}

// Function that checks one grid for digit duplications
bool SudokuProblemSolver::checkGrid(int gridIndex)
{
	int startingRowIndex = gridIndex / 3, startingColumnIndex = gridIndex % 3;
	int finishingRowIndex = startingRowIndex + 3, finishingColumnIndex = startingColumnIndex + 3;
	bool gridUsedValues[9];
	int value;
	for (int valueIndex = 0; valueIndex < 9; valueIndex++)
		gridUsedValues[valueIndex] = false;
	for (int rowIndex = startingRowIndex; rowIndex < finishingRowIndex; rowIndex++)
		for (int colIndex = startingColumnIndex; colIndex < finishingColumnIndex; colIndex++)
		{
			value = sudokuProblemValues[rowIndex][colIndex];
			if (value != 0)
				if (gridUsedValues[value - 1]) return false;
				else gridUsedValues[value - 1] = true;
		}
	return true;
}