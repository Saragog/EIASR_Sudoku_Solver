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

#pragma once

#include "PossibilitiesForAField.h"
#include "Field.h"

class SudokuProblemSolver
{

// TODO later on work on the public / private keywords
public:
	static const int ROWS = 9;
	static const int COLUMNS = 9;
	static const int GRID_ROWS = 3;
	static const int GRID_COLUMNS = 3;
	int** sudokuProblemValues;
	Field*** fields;
	//PossibilitiesForAField possibilitiesForFields[9][9];

	SudokuProblemSolver();
	~SudokuProblemSolver();
	int** solveSudokuProblem(int** sudokuProblem);					// Solves the sudoku problem, the data of which has already been read from the image
	void initializeProblem(int** sudokuProblem);
	void initiallyRemovePossibilities();
	bool checkIfSudokuProblemIsAppropriate(int** sudokuProblem);	// Performs simple calculations for checking if given sudoku data is appropriate to be
																	// a sudoku problem
private:
	bool checkRows();												// Checks if there are any duplications of digits in any row
	bool checkRow(int rowIndex);									// Checks if there are any duplications of digits in one row
	bool checkColumns();											// Checks if there are any duplications of digits in any column
	bool checkColumn(int columnIndex);								// Checks if there are any duplications of digits in one column
	bool checkGrids();												// Checks if there are any duplications of digits in any of the grids (3x3)
	bool checkGrid(int gridIndex);	// index 0 - upper left grid ... index 8 - bottom right grid
									// from up to down from left to right

	//void initializeProbabilities();
	void crossHatch();
	void removePossibilityForRow(int, int);
	void removePossibilityForColumn(int, int);
	void removePossibilityForGrid(int, int, int);
	void bruteForceTheRest();
};

