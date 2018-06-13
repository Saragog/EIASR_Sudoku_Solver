/*
	Creators Andrzej Dackiewicz, Mateusz Jarzemski
	This file is a part of a university project Sudoku Solver
	that was developed as a EIASR project.
	The aim is to create a software program, that is capable of recognizing sudoku problems
	read from photographs. The program is using OpenCV library.

	This file describes a class that is used as a simple aplication that can be run from our
	simple gui.
*/

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CSudoku_SolverApp:
// See Sudoku_Solver.cpp for the implementation of this class
//

class CSudoku_SolverApp : public CWinApp
{
public:
	CSudoku_SolverApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CSudoku_SolverApp theApp;