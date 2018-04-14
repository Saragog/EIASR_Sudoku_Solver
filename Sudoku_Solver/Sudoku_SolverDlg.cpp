
// Sudoku_SolverDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Sudoku_Solver.h"
#include "Sudoku_SolverDlg.h"
#include "afxdialogex.h"
#include <vector>
#include <string>

#include<conio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace cv;
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSudoku_SolverDlg dialog



CSudoku_SolverDlg::CSudoku_SolverDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SUDOKU_SOLVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	sudokuImageReader = SudokuImageReader();
	sudokuProblemSolver = SudokuProblemSolver();
}

void CSudoku_SolverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSudoku_SolverDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CSudoku_SolverDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_LOAD_BUTTON, &CSudoku_SolverDlg::OnBnClickedLoadButton)
END_MESSAGE_MAP()


// CSudoku_SolverDlg message handlers

BOOL CSudoku_SolverDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSudoku_SolverDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSudoku_SolverDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSudoku_SolverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSudoku_SolverDlg::OnBnClickedOk()
{
	digitRecognizer.train("F:\\programowanie\\sudoku_solver\\x64\\Debug\\train_data"); 
	Mat** sudokuProblem = sudokuImageReader.readSudokuFromImage(path);
	if (sudokuProblem != NULL)
	{
		int a = digitRecognizer.classify(sudokuProblem[0][3]);
		putText(sudokuProblem[0][3], std::to_string(a), Point(10, 40), cv::FONT_HERSHEY_COMPLEX, 1, CV_RGB(0, 0, 64));
		cv::imshow("imgThresholdDigitsImages", sudokuProblem[0][3]);
		//sudokuProblemSolver.solveSudokuProblem(sudokuProblem);
	}
	// TODO show sudoku solution
	// TODO clean the sudoku problem 2d array somewhere (delete)
	return;
}

void CSudoku_SolverDlg::OnBnClickedLoadButton()
{
	CFileDialog fOpenDlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, NULL, this);
	if (fOpenDlg.DoModal() == IDOK)
	{
		CT2CA convertedAnsiString(fOpenDlg.GetPathName());
		path = std::string(convertedAnsiString);
	}
}