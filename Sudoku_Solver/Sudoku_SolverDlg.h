
// Sudoku_SolverDlg.h : header file
//

#pragma once

#include<opencv2/opencv.hpp> 
#include <vector>


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

	std::string path; //plik wejsciowy
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedLoadButton();

};

void drawLine(cv::Vec2f line, cv::Mat &img, cv::Scalar rgb = CV_RGB(0, 0, 255));
void cutImageWithPoints(cv::Mat imageToBeCut, cv::Mat* imageParts);
void findCorners(std::vector<std::vector<cv::Point>> contours, std::vector<cv::Point>&);
