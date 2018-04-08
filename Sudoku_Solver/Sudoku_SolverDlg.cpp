
// Sudoku_SolverDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Sudoku_Solver.h"
#include "Sudoku_SolverDlg.h"
#include "afxdialogex.h"
#include <vector>

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
	// TODO: Add your control notification handler code here
	cv::Mat imgOriginal;        // input image
	cv::Mat imgGrayscale;       // grayscale of input image
	cv::Mat imgBlurred;         // intermediate blured image
	//cv::Mat imgCanny;           // Canny edge image
	cv::Mat imgTreshold;		// tresholded image
	cv::Mat imgBitwiseNot;		// bitwise not image
	cv::Mat imgDilate;			// dilated image
	cv::Mat imgBiggestBlob;		// biggest blob image
	cv::Mat imgEroded;			// eroded back image
	cv::Mat imgDetectedLines;	// detected lines image

	cv::Mat imgCrossingPoints[16]; // crossing points images

	
	imgOriginal = cv::imread(cv::String(path));

	if (imgOriginal.empty()) {                                  // if unable to open image
		//TODO messege box
		std::cout << "error: image not read from file\n\n";     // show error message on command line
		_getch();                                               // may have to modify this line if not using Windows
		return;                                              // and exit program
	}

	cv::cvtColor(imgOriginal, imgGrayscale, CV_BGR2GRAY);       // convert to grayscale

	cv::GaussianBlur(imgGrayscale,          // input image
		imgBlurred,                         // output image
		cv::Size(11, 11),					// smoothing window width and height in pixels
		0);                               // sigma value, determines how much the image will be blurred

	cv::adaptiveThreshold(imgBlurred, imgTreshold, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 5, 2);
	
	/*
	cv::Canny(imgBlurred,           // input image
		imgCanny,                   // output image
		82,                         // low threshold
		164);                       // high threshold
	*/

	cv::bitwise_not(imgTreshold, imgBitwiseNot);

	cv::Mat kernel = (cv::Mat_<uchar>(3, 3) << 0, 1, 0, 1, 1, 1, 0, 1, 0);
	cv::dilate(imgBitwiseNot, imgDilate, kernel);

	// _____

	imgBiggestBlob = imgDilate.clone();

	int count = 0;
	int max = -1;

	Point maxPt;

	for (int y = 0; y<imgBiggestBlob.size().height; y++)
	{
		uchar *row = imgBiggestBlob.ptr(y);
		for (int x = 0; x<imgBiggestBlob.size().width; x++)
		{
			if (row[x] >= 128)
			{

				int area = floodFill(imgBiggestBlob, Point(x, y), CV_RGB(0, 0, 64));

				if (area>max)
				{
					maxPt = Point(x, y);
					max = area;
				}
			}
		}

	}

	floodFill(imgBiggestBlob, maxPt, CV_RGB(255, 255, 255));


	for (int y = 0; y<imgBiggestBlob.size().height; y++)
	{
		uchar *row = imgBiggestBlob.ptr(y);
		for (int x = 0; x<imgBiggestBlob.size().width; x++)
		{
			if (row[x] == 64 && x != maxPt.x && y != maxPt.y)
			{
				int area = floodFill(imgBiggestBlob, Point(x, y), CV_RGB(0, 0, 0));
			}
		}
	}

	erode(imgBiggestBlob, imgEroded, kernel);

	cutImageWithPoints(imgEroded, imgCrossingPoints);

	std::vector<std::vector<Point>> contours;

	findContours(imgEroded, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	
	std::vector<Point> corners;
	findCorners(contours, corners);
	for (int circleIndex = 0; circleIndex < 4; circleIndex++)
		circle(imgOriginal, corners[circleIndex], 5, CV_RGB(255, 0, 0), -1);



	//drawContours(imgOriginal, contours, -1, CV_RGB(255, 0, 0));

	imgDetectedLines = imgEroded.clone();

	std::vector<Vec2f> lines;
	HoughLines(imgDetectedLines, lines, 1, CV_PI / 180, 200);

	/*
	for (int i = 0; i<lines.size(); i++)
	{
		drawLine(lines[i], imgDetectedLines, CV_RGB(0, 0, 128));
	}
	*/


	// _____
									// declare windows
	cv::namedWindow("imgOriginal", CV_WINDOW_NORMAL);     // note: you can use CV_WINDOW_NORMAL which allows resizing the window
	//cv::namedWindow("imgCanny", CV_WINDOW_AUTOSIZE);        // or CV_WINDOW_AUTOSIZE for a fixed size window matching the resolution of the image
															// CV_WINDOW_AUTOSIZE is the default
	cv::namedWindow("imgBlurred", CV_WINDOW_NORMAL);
	cv::namedWindow("ingTreshold", CV_WINDOW_NORMAL);
	cv::namedWindow("imgBitwiseNot", CV_WINDOW_NORMAL);
	cv::namedWindow("imgDilate", CV_WINDOW_NORMAL);
	cv::namedWindow("imgBiggestBlob", CV_WINDOW_NORMAL);
	cv::namedWindow("imgEroded", CV_WINDOW_NORMAL);
	cv::namedWindow("imgDetectedLines", CV_WINDOW_NORMAL);
	cv::namedWindow("imgFirstPartOfImage", CV_WINDOW_NORMAL);
	
	cv::imshow("imgOriginal", imgOriginal);
	cv::imshow("imgBlurred", imgBlurred);     // show windows
	cv::imshow("ingTreshold", imgTreshold);
	cv::imshow("imgBitwiseNot", imgBitwiseNot);
	cv::imshow("imgDilate", imgDilate);
	cv::imshow("imgBiggestBlob", imgBiggestBlob);
	cv::imshow("imgEroded", imgEroded);
	cv::imshow("imgDetectedLines", imgDetectedLines);
	cv::imshow("imgFirstPartOfImage", imgCrossingPoints[5]);

	//
	//cv::imshow("imgFirstPartOfImage", contours);


	//cv::imshow("imgCanny", imgCanny);

	cv::waitKey(0);                 // hold windows open until user presses a key
	return;
}

void drawLine(cv::Vec2f line, cv::Mat &img, cv::Scalar rgb)
{
	if (line[1] != 0)
	{
		float m = -1 / tan(line[1]);

		float c = line[0] / sin(line[1]);

		cv::line(img, Point(0, c), Point(img.size().width, m*img.size().width + c), rgb);
	}
	else
	{
		cv::line(img, Point(line[0], 0), Point(line[0], img.size().height), rgb);
	}

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

void cutImageWithPoints(cv::Mat imageToBeCut, cv::Mat* imageParts)
{
	int yDimSize = imageToBeCut.size().height;
	int xDimSize = imageToBeCut.size().width;
	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			cv::Rect rect = cv::Rect(0 + col * xDimSize / 4, 0 + row * yDimSize / 4, xDimSize / 4, yDimSize / 4);
			imageParts[row * 3 + col] = cv::Mat(imageToBeCut, rect).clone();
		}
	}



}

void findCorners(std::vector<std::vector<Point>> contours, std::vector<Point> &corners)
{	
	corners.push_back(Point(contours[0][0]));							// upper left corner
	for (int step = 0; step < 3; step++) corners.push_back(Point(contours[0][0]));	// upper right / down left / down right corners

	int size = contours[0].size();
	for (int i = 0; i < size; i++)
	{
		if (contours[0][i].x + contours[0][i].y < corners[0].x + corners[0].y)
			corners[0] = Point(contours[0][i]);
		else if (contours[0][i].x - contours[0][i].y > corners[1].x - corners[1].y)
			corners[1] = Point(contours[0][i]);
		else if (contours[0][i].y - contours[0][i].x > corners[2].y - corners[2].x)
			corners[2] = Point(contours[0][i]);
		else if (contours[0][i].x + contours[0][i].y > corners[3].x + corners[3].y)
			corners[3] = Point(contours[0][i]);
	}

}