
// SudokuView.h : CSudokuView 클래스의 인터페이스
//

#pragma once


class CSudokuView : public CView
{
protected: // serialization에서만 만들어집니다.
	CSudokuView();
	DECLARE_DYNCREATE(CSudokuView)

// 특성입니다.
public:
	CSudokuDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CSudokuView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

private:
	int sudoku_grid[9][9]; // 스도쿠 판
	bool duplicated[9][9]; //세로, 가로, 같은 그룹 안에 중복된 숫자가 있는지의 여부에 대한 정보를 가지고 있는 배열
	int input_err; // 입력 오류 값

	bool selected; // 셀이 선택된 상태인지의 여부
	int sel_x; // 선택된 셀의 열
	int sel_y; // 선택된 셀의 행

	int blocksize; // 스도쿠 판의 1칸의 변의 길이
	int move; // 스도쿠판의 왼쪽 위 모서리의 위치
};

#ifndef _DEBUG  // SudokuView.cpp의 디버그 버전
inline CSudokuDoc* CSudokuView::GetDocument() const
   { return reinterpret_cast<CSudokuDoc*>(m_pDocument); }
#endif

