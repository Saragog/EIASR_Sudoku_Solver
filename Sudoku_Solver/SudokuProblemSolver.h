#pragma once
class SudokuProblemSolver
{
public:

	int** sudokuProblemValues;

	SudokuProblemSolver();
	~SudokuProblemSolver();
	void solveSudokuProblem(int** sudokuProblem);
	bool checkIfSudokuProblemIsAppropriate(int** sudokuProblem);
private:
	bool checkRows();
	bool checkRow(int rowIndex);
	bool checkColumns();
	bool checkColumn(int columnIndex);
	bool checkGrids();
	bool checkGrid(int gridIndex);	// index 0 - upper left grid ... index 8 - bottom right grid
									// from up to down from left to right
};

