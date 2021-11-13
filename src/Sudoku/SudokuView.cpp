
// SudokuView.cpp : CSudokuView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

// CSudokuView ����/�Ҹ�

CSudokuView::CSudokuView()
{
	std::ifstream input;

	input.open("input.txt");
	if (!input)
	{
		input_err = 1; // �Է������� ���� ������ ���, input_err�� 1�̴�.
		return;
	}

	// input.txt�� �������¸� �˻��ϴ� �κ��̴�.
	std::string data; // �Է����Ͽ��� ���� 1���� �����ϴ� ���ڿ��̴�.  
	std::string sub; // data���� ���鹮�ڸ� �������� �Ľ��� ���ڿ��̴�.
	std::size_t sub_startpos; // data���� sub�� ������ġ
	std::size_t sub_endpos; // data���� sub�� ��������ġ + 1
	int i, j;

	for (i = 0; getline(input, data) && i < 9;) // �ؽ�Ʈ ������ �� �پ� �о����, �о�� ���� ���鹮�ڸ� �������� ������, ���� ���ڿ����� �˻��ϵ��� �Ѵ�.
	{
		if (data.find_first_not_of(" ") == std::string::npos) // ���鹮�ڸ� �ִ� ���� �ǳʶڴ�.
		{
			continue;
		}

		for (j = 0, sub_endpos = 0; j < 9; ++j)
		{
			sub_startpos = data.find_first_not_of(" ", sub_endpos); // ������ sub_endpos���� Ž���Ѵ�.
			if (sub_startpos == std::string::npos) // sub_startpos�� ã�� ���Ѱ��(1�ٿ� 9���̸��� sub�� �ִ� ���)
			{
				input_err = 2; // �Է������� �������°� �ùٸ��� ���� ���, input_err�� 2�̴�.
				input.close();
				return;
			}

			sub_endpos = data.find_first_of(" ", sub_startpos);
			if ((j < 8 && sub_endpos == std::string::npos)) //  j < 8�ε� sub_endpos�� ã�� ���Ѱ�� (1�ٿ� 9���̸��� sub�� �ִ� ���)
			{
				input_err = 2;
				input.close();
				return;
			}

			// sub�� data���� ���� �κ��̴�.
			if (j == 8 && sub_endpos == std::string::npos)
			{
				sub = data.substr(sub_startpos);
			}
			else
			{
				sub = data.substr(sub_startpos, sub_endpos - sub_startpos);
			}
			

			if (sub.size() == 1 && sub[0] >= '0' && sub[0] <= '9') // sub�� 1�ڸ� �����ΰ��
			{
				sudoku_grid[i][j] = std::stoi(sub) * -1; // disabled block�� enabled block�� �����ϱ� ����, disabled block�� ���ڴ� ������ �����Ѵ�.
			}
			else // sub�� 1�ڸ� ���ڰ� �ƴ� ���
			{
				input_err = 2;
				input.close();
				return;
			}
		}

		if (sub_endpos != std::string::npos && data.find_first_not_of(" ", sub_endpos) != std::string::npos) // � �ٿ��� �������� ���е� 9���� ���� �ڿ� ������ ������ ���ڰ� �����ϴ� ��� 
		{
			input_err = 2;
			input.close();
			return;
		}
		
		++i;
	}
	if (i < 9) // �Է����Ͽ��� �� ���� �ƴ� ���� ������ 9���� ���� ���,
	{
		input_err = 2;
		input.close();
		return;
	}
	while (getline(input, data))
	{
		if (data.find_first_not_of(" ") != std::string::npos) // �Է����Ͽ��� �� ���� �ƴ� ���� ������ 9���� ū ���,
		{
			input_err = 2;
			input.close();
			return;
		}
	}
	input.close();


	// input.txt�� ������ ������ ��Ģ�� �´��� Ȯ���ϴ� �κ��̴�.
	for (i = 0; i < 9; ++i)
	{
		for (j = 0; j < 9; ++j)
		{
			if (sudoku_grid[i][j] == 0)
			{
				continue; // sudoku_grid[i][j]�� 0�ΰ��� �ǳʶڴ�.
			}

			// sudoku_grid[i][j]�� ���� ���� ���Ҷ� ���� ���ϴ� �κ��̴�.
			for (int k = 0; k < 9; ++k)
			{
				if (k == i)
				{
					continue; // �ڱ��ڽŰ��� ������ �ʴ´�.
				}
				if (sudoku_grid[i][j] == sudoku_grid[k][j]) // ���ڰ� �ߺ��� ���
				{
					input_err = 3; // �Է������� �������´� �ùٸ����� ������ ��Ģ�� ���� �ʴ°��, input_err�� 3�̴�.
					return;
				}
			}

			// sudoku_grid[i][j]�� ���� ���� ���Ҷ� ���� ���ϴ� �κ��̴�.
			for (int k = 0; k < 9; ++k)
			{
				if (k == j)
				{
					continue; // �ڱ��ڽŰ��� ������ �ʴ´�.
				}
				if (sudoku_grid[i][j] == sudoku_grid[i][k]) // ���ڰ� �ߺ��� ���
				{
					input_err = 3;
					return;
				}
			}

			// sudoku_grid[i][j]�� ���� 3*3 �׷��� ���Ҷ� ���� ���ϴ� �κ��̴�.
			for (int k = (i / 3) * 3; k < (i / 3) * 3 + 3; ++k)
			{
				for (int l = (j / 3) * 3; l < (j / 3) * 3 + 3; ++l)
				{
					if (k == i && l == j) // �ڱ��ڽŰ��� ������ �ʴ´�.
					{
						continue;
					}
					if (sudoku_grid[i][j] == sudoku_grid[k][l]) // ���ڰ� �ߺ��� ���
					{
						input_err = 3;
						return;
					}
				}
			}
		}
	}

	// duplicated �迭�� �ʱ�ȭ �Ѵ�.
	for (int i = 0; i < 9; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			duplicated[i][j] = false;
		}
	}

	input_err = 0; // �Է¿� ���� ������ ���ٸ�, input_err�� 0�̴�.
	selected = false;

	// ������ ���� 1ĭ�� ���� ���̿�, ���������� ���� �� �𼭸��� ��ġ�� �����Ѵ�.
	blocksize = 60;
	move = 90;
}

CSudokuView::~CSudokuView()
{
}

BOOL CSudokuView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CSudokuView �׸���

void CSudokuView::OnDraw(CDC* /*pDC*/)
{
	CSudokuDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	
	CClientDC dc(this);

	if (input_err == 1) // input.txt�� ���� ������ ���
	{
		dc.TextOutW(275, 320, _T("Failed to open input text file."));
		return;
	}
	else if (input_err == 2) // input.txt�� �������°� �ùٸ��� �������
	{
		dc.TextOutW(320, 320, _T("Invalid input."));
		return;
	}
	else if (input_err == 3) // input.txt�� �������´� �ùٸ����� ������ ��Ģ�� ���� �ʴ°��
	{
		dc.TextOutW(235, 320, _T("The input is against the rules of Sudoku."));
		return;
	}


	// ������ �ǰ� ���ڸ� ǥ���ϴ� �κ��̴�.
	CString str;
	CBrush brush;
	CBrush* oldbrush;

	for (int i = 0; i < 9; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			if (sudoku_grid[i][j] < 0) // sudoku_grid[i][j]�� disabled block�� ���
			{
				brush.CreateSolidBrush(RGB(192, 192, 192)); // ����� �簢�� ���� ������ ȸ������ �����Ѵ�.
				dc.SetBkColor(RGB(192, 192, 192)); // ���ڿ��� ��� ������ ���� ����� �Ȱ��� ���� �Ѵ�.
				str.Format(_T("%d"), sudoku_grid[i][j] * -1); // disabled block�� sudoku_grid[i][j]�� ������ ����Ǿ������Ƿ�, -1�� ���ؼ� ����� ����ϵ��� �Ѵ�.
			}
			else if (selected == true && sel_y == i && sel_x == j) // sudoku_grid[i][j]�� ���� ���õ� ���ΰ��
			{
				brush.CreateSolidBrush(RGB(153, 255, 153)); // ����� �簢�� ���� ������ ������� �����Ѵ�.
				dc.SetBkColor(RGB(153, 255, 153)); // ���ڿ��� ��� ������ ���� ����� �Ȱ��� ���� �Ѵ�.
				str.Format(_T("%d"), sudoku_grid[i][j]);
			}
			else // sudoku_grid[i][j]�� enabled block�̰�, ���� ���õ� ���� �ƴ� ���
			{
				brush.CreateSolidBrush(RGB(255, 255, 255)); // ����� �簢�� ���� ������ ������� �����Ѵ�.
				dc.SetBkColor(RGB(255, 255, 255)); // ���ڿ��� ��� ������ ���� ����� �Ȱ��� ���� �Ѵ�.
				str.Format(_T("%d"), sudoku_grid[i][j]);
			}

			oldbrush = dc.SelectObject(&brush);
			dc.Rectangle(j * blocksize + move, i * blocksize + move, j * blocksize + (blocksize + move) + 1, i * blocksize + (blocksize + move) + 1); //�簢�� ���� ����Ѵ�.
			dc.SelectObject(oldbrush);
			brush.DeleteObject();

			if (sudoku_grid[i][j] != 0) // sudoku_grid[i][j] �� 0�� �ƴ� ��쿡�� ���ڸ� ����Ѵ�.
			{
				if (duplicated[i][j] == true)
				{
					dc.SetTextColor(RGB(255, 51, 51)); // duplicated[i][j] �� true�� ���� ���, ���ڸ� ���������� �����Ѵ�. 
				}
				else
				{
					dc.SetTextColor(RGB(0, 0, 0));
				}

				dc.TextOutW(j * blocksize + (move + blocksize / 2 - 3), i * blocksize + (move + blocksize / 2 - 6), str); // ���� ���� ���ڸ� ����Ѵ�.
			}
		}
	}

	// 3*3ũ���� �׷���� �����ϴ� ���� ���� ǥ���ϴ� �κ��̴�.
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


// CSudokuView �μ�

BOOL CSudokuView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CSudokuView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CSudokuView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CSudokuView ����

#ifdef _DEBUG
void CSudokuView::AssertValid() const
{
	CView::AssertValid();
}

void CSudokuView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSudokuDoc* CSudokuView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSudokuDoc)));
	return (CSudokuDoc*)m_pDocument;
}
#endif //_DEBUG


// CSudokuView �޽��� ó����


void CSudokuView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (input_err || selected == false) // input error�� �߻��� �����̰ų�, ���õ� ���� ���� ������ ���
	{
		return;
	}

	if (nChar >= 0x31 && nChar <= 0x39) // 1~9 ������ Ű�� ������ ���
	{
		sudoku_grid[sel_y][sel_x] = nChar - 48; //���� ����Ű�� ���ڸ� ���� ���õ� ���� �����Ѵ�.
		selected = false;
	}
	else if (nChar == 0x1B || nChar == 0x2E)// Esc �Ǵ� DeleteŰ�� ������ ���
	{
		sudoku_grid[sel_y][sel_x] = 0; //���� ���õ� ���� ���ڸ� �����.
		selected = false;
	}
	else // �� ���� Ű�� ������ ���
	{
		selected = false;
	}
	
	// ���������� �ϼ������ Ȯ���ϰ�, duplicated�迭�� ������Ʈ�ϴ� �κ��̴�.
	bool gameover = true; // ���������� �ϼ�������� ����
	bool cur_duplicated; // ���� �˻����� ��(sudoku_grid[i][j])�� ����, ����, 3*3 �׷��� ���ڿ� �ߺ�������� ����

	for (int i = 0; i < 9; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			if (sudoku_grid[i][j] == 0)
			{
				gameover = false;
				continue; // sudoku_grid[i][j]�� 0�ΰ��� �ǳʶڴ�.
			}

			cur_duplicated = false;

			// sudoku_grid[i][j]�� ���� ���� ���Ҷ� ���� ���ϴ� �κ��̴�.
			for (int k = 0; k < 9; ++k)
			{
				if (k == i)
				{
					continue; // �ڱ��ڽŰ��� ������ �ʴ´�.
				}
				if (sudoku_grid[i][j] == sudoku_grid[k][j] || sudoku_grid[i][j] == sudoku_grid[k][j] * -1) //disabled block�� ���ڰ� ������ ����Ǿ��ֱ� ������, -1�� ���ѰͰ��� �񱳸� �Ѵ�.
				{
					gameover = false;
					duplicated[i][j] = true;
					duplicated[k][j] = true;
					cur_duplicated = true;
				}
			}

			// sudoku_grid[i][j]�� ���� ���� ���Ҷ� ���� ���ϴ� �κ��̴�.
			for (int k = 0; k < 9; ++k)
			{
				if (k == j)
				{
					continue; // �ڱ��ڽŰ��� ������ �ʴ´�.
				}
				if (sudoku_grid[i][j] == sudoku_grid[i][k] || sudoku_grid[i][j] == sudoku_grid[i][k] * -1)
				{
					gameover = false;
					duplicated[i][j] = true;
					duplicated[i][k] = true;
					cur_duplicated = true;
				}
			}

			// sudoku_grid[i][j]�� ���� 3*3 �׷��� ���Ҷ� ���� ���ϴ� �κ��̴�.
			for (int k = (i / 3) * 3; k < (i / 3) * 3 + 3; ++k)
			{
				for (int l = (j / 3) * 3; l < (j / 3) * 3 + 3; ++l)
				{
					if (k == i && l == j) // �ڱ��ڽŰ��� ������ �ʴ´�.
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

			if (cur_duplicated == false) //sudoku_grid[i][j])�� ����, ����, 3*3 �׷��� ���ڿ� �ߺ����� ���� ���
			{
				duplicated[i][j] = false; //duplicated[i][j]�� false�� �Ѵ�.
			}
		}
	}
	
	CWnd::Invalidate(); // â�� ������ ������Ʈ �ɶ� �׸��⸦ �ٽ� �ϵ��� �Ѵ�(OnDraw�� ȣ���ϵ��� �Ѵ�).
	CWnd::UpdateWindow(); // â�� ������Ʈ ��Ų��.

	if (gameover == true)
	{
		AfxMessageBox(_T("Game Over!")); //���������� �ϼ��� ���, message box�� ����.
	}
	
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CSudokuView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (input_err
		|| point.x < move + 2 || point.y < move + 2 || point.x > move + 9 * blocksize -  2 || point.y > move + 9 * blocksize - 2) // input error�� �߻��� �����̰ų�, ������ �� ������ �ٱ����� ���� Ŭ���� �Ͼ ��� 
	{
		return;
	}

	// ����Ŭ���� ��ǥ�� ���������� ��� �� ������ ��ȯ�Ѵ�.
	sel_x = (point.x - move) / blocksize;
	sel_y = (point.y - move) / blocksize;

	if (sudoku_grid[sel_y][sel_x] < 0) // disabled block�� ����Ŭ���� ���
	{
		return;
	}
	else // enabled block�� ����Ŭ���� ���
	{
		selected = true;
	}

	CWnd::Invalidate(); // â�� ������ ������Ʈ �ɶ� �׸��⸦ �ٽ� �ϵ��� �Ѵ�(OnDraw�� ȣ���ϵ��� �Ѵ�).
	CWnd::UpdateWindow(); // â�� ������Ʈ ��Ų��.

	CView::OnLButtonDblClk(nFlags, point);
}