
// SudokuView.h : CSudokuView Ŭ������ �������̽�
//

#pragma once


class CSudokuView : public CView
{
protected: // serialization������ ��������ϴ�.
	CSudokuView();
	DECLARE_DYNCREATE(CSudokuView)

// Ư���Դϴ�.
public:
	CSudokuDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CSudokuView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

private:
	int sudoku_grid[9][9]; // ������ ��
	bool duplicated[9][9]; //����, ����, ���� �׷� �ȿ� �ߺ��� ���ڰ� �ִ����� ���ο� ���� ������ ������ �ִ� �迭
	int input_err; // �Է� ���� ��

	bool selected; // ���� ���õ� ���������� ����
	int sel_x; // ���õ� ���� ��
	int sel_y; // ���õ� ���� ��

	int blocksize; // ������ ���� 1ĭ�� ���� ����
	int move; // ���������� ���� �� �𼭸��� ��ġ
};

#ifndef _DEBUG  // SudokuView.cpp�� ����� ����
inline CSudokuDoc* CSudokuView::GetDocument() const
   { return reinterpret_cast<CSudokuDoc*>(m_pDocument); }
#endif

