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

#include <iostream>

SudokuProblemSolver::SudokuProblemSolver()
{

}


SudokuProblemSolver::~SudokuProblemSolver()
{

}

// Function that could be implemented in the future
// It would solve the sudoku problem that was read from an image file
int** SudokuProblemSolver::solveSudokuProblem(int** sudokuProblem)
{
	initializeProblem(sudokuProblem);
	initiallyRemovePossibilities();
	crossHatch();
	bruteForceTheRest();

	int** results = new int*[ROWS];
	for (int row = 0; row < ROWS; row++)
	{
		results[row] = new int[COLUMNS];
		for (int col = 0; col < COLUMNS; col++)
		{
			results[row][col] = fields[row][col]->getValue();
		}
	}
	
	return results;
}

void SudokuProblemSolver::initializeProblem(int** sudokuProblem)
{
	fields = new Field**[ROWS];
	for (int row = 0; row < ROWS; row++)
	{
		fields[row] = new Field*[COLUMNS];
		for (int column = 0; column < COLUMNS; column++)
		{
			if (sudokuProblem[row][column] != 0)
			{
				fields[row][column] = new Field(sudokuProblem[row][column]);

			}
			else
				fields[row][column] = new Field();
		}
	}
}

void SudokuProblemSolver::initiallyRemovePossibilities()
{
	int fieldValue;
	for (int row = 0; row < ROWS; row++)
	{
		for (int column = 0; column < COLUMNS; column++)
		{
			fieldValue = fields[row][column]->getValue();
			if (fields[row][column]->getValue() != -1)
			{
				removePossibilityForRow(fieldValue, row);
				removePossibilityForColumn(fieldValue, column);
				removePossibilityForGrid(fieldValue, row, column);
			}
		}
	}
}

void SudokuProblemSolver::crossHatch()
{
	for (int column = 0; column < COLUMNS; column++)
	{
		for (int row = 0; row < ROWS; row++)
		{
			if (fields[row][column]->isThereOnePossibility() == true)
			{
				fields[row][column]->setTheOnlyPossibility();
				removePossibilityForRow(fields[row][column]->getValue(), row);
				removePossibilityForColumn(fields[row][column]->getValue(), column);
				removePossibilityForGrid(fields[row][column]->getValue(), row, column);
				column = 0;
				row = 0;
			}
		}
	}
}

/*
	Removes possibility that was used in the Row
*/
void SudokuProblemSolver::removePossibilityForRow(int possibilityToBeRemoved, int rowIndex)
{
	for (int columnIndex = 0; columnIndex < COLUMNS; columnIndex++)
	{
		fields[rowIndex][columnIndex]->removePossibility(possibilityToBeRemoved);
	}
}

/*
	Removes possibility that was used in the Column
*/
void SudokuProblemSolver::removePossibilityForColumn(int possibilityToBeRemoved, int columnIndex)
{
	for (int rowIndex = 0; rowIndex < ROWS; rowIndex++)
	{
		fields[rowIndex][columnIndex]->removePossibility(possibilityToBeRemoved);
	}
}

/*
	Removes a possibility that was used in the grid.
*/
void SudokuProblemSolver::removePossibilityForGrid(int possibilityToBeRemoved, int rowIndex, int columnIndex)
{
	int startingRow = rowIndex / GRID_ROWS * 3;
	int startingColumn = columnIndex / GRID_COLUMNS * 3;
	int endingRow = startingRow + GRID_ROWS;
	int endingColumn = startingColumn + GRID_COLUMNS;
	for (int row = startingRow; row < endingRow; row++)
	{
		for (int column = startingColumn; column < endingColumn; column++)
		{
			fields[row][column]->removePossibility(possibilityToBeRemoved);
		}
	}
}

// TODO rename and perform the backtracking algorithm
void SudokuProblemSolver::bruteForceTheRest()
{

}

// __________

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