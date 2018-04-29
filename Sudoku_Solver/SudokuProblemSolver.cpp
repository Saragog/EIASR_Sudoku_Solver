#include "stdafx.h"
#include "SudokuProblemSolver.h"


SudokuProblemSolver::SudokuProblemSolver()
{
}


SudokuProblemSolver::~SudokuProblemSolver()
{
}

void SudokuProblemSolver::solveSudokuProblem(int** sudokuProblem)
{

}

bool SudokuProblemSolver::checkIfSudokuProblemIsAppropriate(int** sudokuProblem)
{
	sudokuProblemValues = sudokuProblem;
	if (checkRows() && checkColumns() && checkGrids()) return true;
	else return false;
}

bool SudokuProblemSolver::checkRows()
{
	for (int rowIndex = 0; rowIndex < 9; rowIndex++)
		if (!checkRow(rowIndex)) return false;
	return true;
}

bool SudokuProblemSolver::checkColumns()
{
	for (int columnIndex = 0; columnIndex < 9; columnIndex++)
		if (!checkRow(columnIndex)) return false;
	return true;
}

bool SudokuProblemSolver::checkColumn(int columnIndex)
{
	bool columnUsedValues[9];
	int value;
	for (int value = 0; value < 9; value++)
		columnUsedValues[value] = false;
	for (int rowIndex = 0; rowIndex < 9; rowIndex++)
	{
		value = sudokuProblemValues[rowIndex][columnIndex];
		if (columnUsedValues[value - 1]) return false;
	}
	return true;
}

bool SudokuProblemSolver::checkRow(int rowIndex)
{
	bool rowUsedValues[9];
	int value;
	for (int valueIndex = 0; valueIndex < 9; valueIndex++)
		rowUsedValues[valueIndex] = false;
	for (int columnIndex = 0; columnIndex < 9; columnIndex++)
	{
		value = sudokuProblemValues[rowIndex][columnIndex];
		if (rowUsedValues[value - 1]) return false;
		else rowUsedValues[value - 1] = true;
	}
	return true;
}

bool SudokuProblemSolver::checkGrids()
{
	for (int gridIndex = 0; gridIndex < 9; gridIndex++)
		if (!checkGrid(gridIndex)) return false;
	return true;
}

bool SudokuProblemSolver::checkGrid(int gridIndex)
{
	int startingRowIndex = gridIndex / 3, startingColumnIndex = gridIndex % 3;
	int finishingRowIndex = startingRowIndex + 3, finishingColumnIndex = startingColumnIndex + 3;
	bool gridUsedValues[9];
	for (int valueIndex = 0; valueIndex < 9; valueIndex++)
		gridUsedValues[valueIndex] = false;
	for (int rowIndex = startingRowIndex; rowIndex < finishingRowIndex; rowIndex++)
		for (int colIndex = startingColumnIndex; colIndex < finishingColumnIndex; colIndex++)
			if (gridUsedValues[sudokuProblemValues[rowIndex][colIndex] - 1]) return false;
			else gridUsedValues[sudokuProblemValues[rowIndex][colIndex] - 1] = true;
	return true;
}