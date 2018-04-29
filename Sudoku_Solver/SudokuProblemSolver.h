#pragma once
class SudokuProblemSolver
{
public:
	SudokuProblemSolver();
	~SudokuProblemSolver();
	void solveSudokuProblem(int** sudokuProblem);
	bool checkIfSudokuProblemIsAppropriate(int** sudokuProblem);
private:
};

