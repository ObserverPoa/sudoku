
// SudokuView.cpp : CSudokuView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Sudoku.h"
#endif

#include "SudokuDoc.h"
#include "SudokuView.h"
#include <fstream>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSudokuView

IMPLEMENT_DYNCREATE(CSudokuView, CView)

BEGIN_MESSAGE_MAP(CSudokuView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

// CSudokuView 생성/소멸

CSudokuView::CSudokuView()
{
	std::ifstream input;

	input.open("input.txt");
	if (!input)
	{
		input_err = 1; // 입력파일을 읽지 못했을 경우, input_err은 1이다.
		return;
	}

	// input.txt의 내용형태를 검사하는 부분이다.
	std::string data; // 입력파일에서 읽은 1줄을 저장하는 문자열이다.  
	std::string sub; // data에서 공백문자를 기준으로 파싱한 문자열이다.
	std::size_t sub_startpos; // data에서 sub의 시작위치
	std::size_t sub_endpos; // data에서 sub의 마지막위치 + 1
	int i, j;

	for (i = 0; getline(input, data) && i < 9;) // 텍스트 파일을 한 줄씩 읽어오고, 읽어온 줄을 공백문자를 기준으로 나눈뒤, 나뉜 문자열들을 검사하도록 한다.
	{
		if (data.find_first_not_of(" ") == std::string::npos) // 공백문자만 있는 줄은 건너뛴다.
		{
			continue;
		}

		for (j = 0, sub_endpos = 0; j < 9; ++j)
		{
			sub_startpos = data.find_first_not_of(" ", sub_endpos); // 이전의 sub_endpos에서 탐색한다.
			if (sub_startpos == std::string::npos) // sub_startpos를 찾지 못한경우(1줄에 9개미만의 sub가 있는 경우)
			{
				input_err = 2; // 입력파일의 내용형태가 올바르지 않은 경우, input_err은 2이다.
				input.close();
				return;
			}

			sub_endpos = data.find_first_of(" ", sub_startpos);
			if ((j < 8 && sub_endpos == std::string::npos)) //  j < 8인데 sub_endpos를 찾지 못한경우 (1줄에 9개미만의 sub가 있는 경우)
			{
				input_err = 2;
				input.close();
				return;
			}

			// sub를 data에서 얻어내는 부분이다.
			if (j == 8 && sub_endpos == std::string::npos)
			{
				sub = data.substr(sub_startpos);
			}
			else
			{
				sub = data.substr(sub_startpos, sub_endpos - sub_startpos);
			}
			

			if (sub.size() == 1 && sub[0] >= '0' && sub[0] <= '9') // sub가 1자리 숫자인경우
			{
				sudoku_grid[i][j] = std::stoi(sub) * -1; // disabled block과 enabled block을 구분하기 위해, disabled block의 숫자는 음수로 저장한다.
			}
			else // sub가 1자리 숫자가 아닌 경우
			{
				input_err = 2;
				input.close();
				return;
			}
		}

		if (sub_endpos != std::string::npos && data.find_first_not_of(" ", sub_endpos) != std::string::npos) // 어떤 줄에서 공백으로 구분된 9개의 숫자 뒤에 공백을 제외한 문자가 존재하는 경우 
		{
			input_err = 2;
			input.close();
			return;
		}
		
		++i;
	}
	if (i < 9) // 입력파일에서 빈 줄이 아닌 줄의 개수가 9보다 작은 경우,
	{
		input_err = 2;
		input.close();
		return;
	}
	while (getline(input, data))
	{
		if (data.find_first_not_of(" ") != std::string::npos) // 입력파일에서 빈 줄이 아닌 줄의 개수가 9보다 큰 경우,
		{
			input_err = 2;
			input.close();
			return;
		}
	}
	input.close();


	// input.txt의 내용이 스도쿠 규칙에 맞는지 확인하는 부분이다.
	for (i = 0; i < 9; ++i)
	{
		for (j = 0; j < 9; ++j)
		{
			if (sudoku_grid[i][j] == 0)
			{
				continue; // sudoku_grid[i][j]가 0인것은 건너뛴다.
			}

			// sudoku_grid[i][j]가 속한 행의 원소랑 값을 비교하는 부분이다.
			for (int k = 0; k < 9; ++k)
			{
				if (k == i)
				{
					continue; // 자기자신과는 비교하지 않는다.
				}
				if (sudoku_grid[i][j] == sudoku_grid[k][j]) // 숫자가 중복된 경우
				{
					input_err = 3; // 입력파일의 내용형태는 올바르지만 스도쿠 규칙에 맞지 않는경우, input_err은 3이다.
					return;
				}
			}

			// sudoku_grid[i][j]가 속한 열의 원소랑 값을 비교하는 부분이다.
			for (int k = 0; k < 9; ++k)
			{
				if (k == j)
				{
					continue; // 자기자신과는 비교하지 않는다.
				}
				if (sudoku_grid[i][j] == sudoku_grid[i][k]) // 숫자가 중복된 경우
				{
					input_err = 3;
					return;
				}
			}

			// sudoku_grid[i][j]가 속한 3*3 그룹의 원소랑 값을 비교하는 부분이다.
			for (int k = (i / 3) * 3; k < (i / 3) * 3 + 3; ++k)
			{
				for (int l = (j / 3) * 3; l < (j / 3) * 3 + 3; ++l)
				{
					if (k == i && l == j) // 자기자신과는 비교하지 않는다.
					{
						continue;
					}
					if (sudoku_grid[i][j] == sudoku_grid[k][l]) // 숫자가 중복된 경우
					{
						input_err = 3;
						return;
					}
				}
			}
		}
	}

	// duplicated 배열을 초기화 한다.
	for (int i = 0; i < 9; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			duplicated[i][j] = false;
		}
	}

	input_err = 0; // 입력에 관한 문제가 없다면, input_err은 0이다.
	selected = false;

	// 스도쿠 판의 1칸의 변의 길이와, 스도쿠판의 왼쪽 위 모서리의 위치를 설정한다.
	blocksize = 60;
	move = 90;
}

CSudokuView::~CSudokuView()
{
}

BOOL CSudokuView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CSudokuView 그리기

void CSudokuView::OnDraw(CDC* /*pDC*/)
{
	CSudokuDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	
	CClientDC dc(this);

	if (input_err == 1) // input.txt를 열지 못했을 경우
	{
		dc.TextOutW(275, 320, _T("Failed to open input text file."));
		return;
	}
	else if (input_err == 2) // input.txt의 내용형태가 올바르지 않은경우
	{
		dc.TextOutW(320, 320, _T("Invalid input."));
		return;
	}
	else if (input_err == 3) // input.txt의 내용형태는 올바르지만 스도쿠 규칙에 맞지 않는경우
	{
		dc.TextOutW(235, 320, _T("The input is against the rules of Sudoku."));
		return;
	}


	// 스도쿠 판과 숫자를 표시하는 부분이다.
	CString str;
	CBrush brush;
	CBrush* oldbrush;

	for (int i = 0; i < 9; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			if (sudoku_grid[i][j] < 0) // sudoku_grid[i][j]가 disabled block인 경우
			{
				brush.CreateSolidBrush(RGB(192, 192, 192)); // 출력할 사각형 셀의 색상을 회색으로 설정한다.
				dc.SetBkColor(RGB(192, 192, 192)); // 문자열의 배경 색상을 셀의 색상과 똑같이 설정 한다.
				str.Format(_T("%d"), sudoku_grid[i][j] * -1); // disabled block은 sudoku_grid[i][j]에 음수로 저장되어있으므로, -1을 곱해서 양수로 출력하도록 한다.
			}
			else if (selected == true && sel_y == i && sel_x == j) // sudoku_grid[i][j]가 현재 선택된 셀인경우
			{
				brush.CreateSolidBrush(RGB(153, 255, 153)); // 출력할 사각형 셀의 색상을 녹색으로 설정한다.
				dc.SetBkColor(RGB(153, 255, 153)); // 문자열의 배경 색상을 셀의 색상과 똑같이 설정 한다.
				str.Format(_T("%d"), sudoku_grid[i][j]);
			}
			else // sudoku_grid[i][j]가 enabled block이고, 현재 선택된 셀이 아닌 경우
			{
				brush.CreateSolidBrush(RGB(255, 255, 255)); // 출력할 사각형 셀의 색상을 흰색으로 설정한다.
				dc.SetBkColor(RGB(255, 255, 255)); // 문자열의 배경 색상을 셀의 색상과 똑같이 설정 한다.
				str.Format(_T("%d"), sudoku_grid[i][j]);
			}

			oldbrush = dc.SelectObject(&brush);
			dc.Rectangle(j * blocksize + move, i * blocksize + move, j * blocksize + (blocksize + move) + 1, i * blocksize + (blocksize + move) + 1); //사각형 셀을 출력한다.
			dc.SelectObject(oldbrush);
			brush.DeleteObject();

			if (sudoku_grid[i][j] != 0) // sudoku_grid[i][j] 가 0이 아닌 경우에만 숫자를 출력한다.
			{
				if (duplicated[i][j] == true)
				{
					dc.SetTextColor(RGB(255, 51, 51)); // duplicated[i][j] 가 true인 셀인 경우, 숫자를 빨간색으로 설정한다. 
				}
				else
				{
					dc.SetTextColor(RGB(0, 0, 0));
				}

				dc.TextOutW(j * blocksize + (move + blocksize / 2 - 3), i * blocksize + (move + blocksize / 2 - 6), str); // 현재 셀의 숫자를 출력한다.
			}
		}
	}

	// 3*3크기의 그룹들을 구분하는 굵은 선을 표시하는 부분이다.
	brush.CreateSolidBrush(RGB(0, 0, 0));
	oldbrush = dc.SelectObject(&brush);

	for (int i = 0; i < 4; ++i)
	{
		dc.Rectangle((move - 1) + (i * blocksize * 3), (move - 1), (move + 1) + (i * blocksize * 3) + 1, (move + 1) + (blocksize * 9) + 1);
		dc.Rectangle((move - 1), (move - 1) + (i * blocksize * 3), (move + 1) + (blocksize * 9) + 1, (move + 1) + (i * blocksize * 3) + 1);
	}

	dc.SelectObject(oldbrush);
	brush.DeleteObject();
}


// CSudokuView 인쇄

BOOL CSudokuView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CSudokuView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CSudokuView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CSudokuView 진단

#ifdef _DEBUG
void CSudokuView::AssertValid() const
{
	CView::AssertValid();
}

void CSudokuView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSudokuDoc* CSudokuView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSudokuDoc)));
	return (CSudokuDoc*)m_pDocument;
}
#endif //_DEBUG


// CSudokuView 메시지 처리기


void CSudokuView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (input_err || selected == false) // input error가 발생한 상태이거나, 선택된 셀이 없는 상태인 경우
	{
		return;
	}

	if (nChar >= 0x31 && nChar <= 0x39) // 1~9 사이의 키가 눌렸을 경우
	{
		sudoku_grid[sel_y][sel_x] = nChar - 48; //눌린 숫자키의 숫자를 현재 선택된 셀에 적용한다.
		selected = false;
	}
	else if (nChar == 0x1B || nChar == 0x2E)// Esc 또는 Delete키가 눌렸을 경우
	{
		sudoku_grid[sel_y][sel_x] = 0; //현재 선택된 셀의 숫자를 지운다.
		selected = false;
	}
	else // 그 외의 키가 눌렸을 경우
	{
		selected = false;
	}
	
	// 스도쿠판이 완성됬는지 확인하고, duplicated배열을 업데이트하는 부분이다.
	bool gameover = true; // 스도쿠판이 완성됬는지의 여부
	bool cur_duplicated; // 현재 검사중인 셀(sudoku_grid[i][j])이 가로, 세로, 3*3 그룹의 숫자와 중복됬는지의 여부

	for (int i = 0; i < 9; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			if (sudoku_grid[i][j] == 0)
			{
				gameover = false;
				continue; // sudoku_grid[i][j]가 0인것은 건너뛴다.
			}

			cur_duplicated = false;

			// sudoku_grid[i][j]이 속한 행의 원소랑 값을 비교하는 부분이다.
			for (int k = 0; k < 9; ++k)
			{
				if (k == i)
				{
					continue; // 자기자신과는 비교하지 않는다.
				}
				if (sudoku_grid[i][j] == sudoku_grid[k][j] || sudoku_grid[i][j] == sudoku_grid[k][j] * -1) //disabled block은 숫자가 음수로 저장되어있기 때문에, -1을 곱한것과도 비교를 한다.
				{
					gameover = false;
					duplicated[i][j] = true;
					duplicated[k][j] = true;
					cur_duplicated = true;
				}
			}

			// sudoku_grid[i][j]이 속한 열의 원소랑 값을 비교하는 부분이다.
			for (int k = 0; k < 9; ++k)
			{
				if (k == j)
				{
					continue; // 자기자신과는 비교하지 않는다.
				}
				if (sudoku_grid[i][j] == sudoku_grid[i][k] || sudoku_grid[i][j] == sudoku_grid[i][k] * -1)
				{
					gameover = false;
					duplicated[i][j] = true;
					duplicated[i][k] = true;
					cur_duplicated = true;
				}
			}

			// sudoku_grid[i][j]이 속한 3*3 그룹의 원소랑 값을 비교하는 부분이다.
			for (int k = (i / 3) * 3; k < (i / 3) * 3 + 3; ++k)
			{
				for (int l = (j / 3) * 3; l < (j / 3) * 3 + 3; ++l)
				{
					if (k == i && l == j) // 자기자신과는 비교하지 않는다.
					{
						continue;
					}
					if (sudoku_grid[i][j] == sudoku_grid[k][l] || sudoku_grid[i][j] == sudoku_grid[k][l] * -1)
					{
						gameover = false;
						duplicated[i][j] = true;
						duplicated[k][l] = true;
						cur_duplicated = true;
					}
				}
			}

			if (cur_duplicated == false) //sudoku_grid[i][j])이 가로, 세로, 3*3 그룹의 숫자와 중복되지 않은 경우
			{
				duplicated[i][j] = false; //duplicated[i][j]를 false로 한다.
			}
		}
	}
	
	CWnd::Invalidate(); // 창이 다음에 업데이트 될때 그리기를 다시 하도록 한다(OnDraw를 호출하도록 한다).
	CWnd::UpdateWindow(); // 창을 업데이트 시킨다.

	if (gameover == true)
	{
		AfxMessageBox(_T("Game Over!")); //스도쿠판이 완성된 경우, message box를 띄운다.
	}
	
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CSudokuView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (input_err
		|| point.x < move + 2 || point.y < move + 2 || point.x > move + 9 * blocksize -  2 || point.y > move + 9 * blocksize - 2) // input error가 발생한 상태이거나, 스도쿠 판 영역의 바깥에서 더블 클릭이 일어난 경우 
	{
		return;
	}

	// 더블클릭된 좌표룰 스도쿠판의 행과 열 값으로 변환한다.
	sel_x = (point.x - move) / blocksize;
	sel_y = (point.y - move) / blocksize;

	if (sudoku_grid[sel_y][sel_x] < 0) // disabled block을 더블클릭한 경우
	{
		return;
	}
	else // enabled block을 더블클릭한 경우
	{
		selected = true;
	}

	CWnd::Invalidate(); // 창이 다음에 업데이트 될때 그리기를 다시 하도록 한다(OnDraw를 호출하도록 한다).
	CWnd::UpdateWindow(); // 창을 업데이트 시킨다.

	CView::OnLButtonDblClk(nFlags, point);
}