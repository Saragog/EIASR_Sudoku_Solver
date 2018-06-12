
// Sudoku_SolverDlg.h : header file
//

#pragma once

#include<opencv2/opencv.hpp> 
#include <vector>
#include "SudokuImageReader.h"
#include "SudokuProblemSolver.h"
#include "DigitRecognizer.h"
#include "SudokuSolutionFinder.h"


// CSudoku_SolverDlg dialog
class CSudoku_SolverDlg : public CDialogEx
{
// Construction
public:
	CSudoku_SolverDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SUDOKU_SOLVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	std::string path;							// plik wejsciowy
	SudokuSolutionFinder solver;						// solver that does all processes needed for solving the problem


public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedLoadButton();

};