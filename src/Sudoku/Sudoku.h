
// Sudoku.h : Sudoku ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CSudokuApp:
// �� Ŭ������ ������ ���ؼ��� Sudoku.cpp�� �����Ͻʽÿ�.
//

class CSudokuApp : public CWinApp
{
public:
	CSudokuApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CSudokuApp theApp;
