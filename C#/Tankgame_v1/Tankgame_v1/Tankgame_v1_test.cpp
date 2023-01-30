#include <windows.h>
#include <TCHAR.H>
#include "resource.h"
#include <MMSystem.h>

#define MAXSTAGE 3
#define BW 32
#define BH 32

int tBit;
static int bBit;
void DrawScreen(HDC hdc);
BOOL TestEnd();
void Move(int dir);
void Shot(int);
void InitStage();
void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit);
static BOOL test = false;
static int count = 0;
static int ax = 0, ay = 0;

//RECT CheckTankPos(POINT); // ballR�� ���� ������ TIMER�� �����ϱ� ���� �Լ�
void DrawObject(HDC, RECT&, COLORREF, int, int ropCode = R2_XORPEN);
void DrawObject(HDC, RECT&, COLORREF, COLORREF, int, int ropCode = R2_XORPEN);

POINT CenterPoint(); //������ �߾� ��ġ ���� �Լ�
//int HitTest(RECT &);

static int ballToggleX;	// ball�� ��/���� ���� ��ȯ�� ���̴� toggle
static int ballToggleY;		// ball: ��/�� ���� ��ȯ�� ���̴� toggle
static RECT ballR;	// ball	(1�� ����)
static char ns[18][21];
int nStage;
int nx, ny;
int bx, by;
int nMove;
HBITMAP hBit[20];
static POINT p;
static int tCount;
static bool mTrue = true;


char arStage[MAXSTAGE][18][21] = {
	{
	"####################",
	"#         0        #",
	"#  ##         ##   #",
	"#  ##         ##   #",
	"#                  #",
	"#                  #",
	"#                  #",
	"#    +++++++++     #",
	"#                  #",
	"#        W        ##",
	"#                  #",
	"#              +++ #",
	"#  +++           0 #",
	"#   0+             #",
	"#  ##         ##   #",
	"#  ##         ##   #",
	"#               .  #",
	"####################"
},{
	"####################",
	"####################",
	"#                  #",
	"#                  #",
	"#         W        #",
	"#                  #",
	"#                  #",
	"#                  #",
	"#                  #",
	"#                  #",
	"#                  #",
	"#++++++++++++++++++#",
	"#++++++++++++++++++#",
	"#++++++++++++++++++#",
	"#                  #",
	"#    0   0  0  0   #",
	"#  0   0   0  0  0 #",
	"####################"
},{
	"####################",
	"##+++#+###+#+#######",
	"###+##+++##+########",
	"###+##+#+#+#+#######",
	"##########     W####",
	"#            #######",
	"########          ##",
	"#########    #######",
	"#########    #######",
	"##########++########",
	"##########++########",
	"##    0 ##++########",
	"##++#  ###++########",
	"#++++++###++      +#",
	"#++++  ###++      +#",
	"#+++++++++++      +#",
	"#       ############",
	"####################"
	}
};

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = _T("TankGame_v1_test");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, 0, 0,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	hWndMain = hWnd; // hWnd ������ ���������� ����!

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage,
	WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	RECT crt;
	int i;
	TCHAR Message[256];

	static RECT textR; // ���� ���
	

	static int alphaX;	// ȭ��ǥ ���� -1, ������ 1�� ���̴� toggle
	
	int hitPosition;	// bar�� ball �� �ε�ģ �κ� ���� ���� (bar ��=1, ��=2)
	static int hitNumCount;
	
	
	TCHAR str[100];	// bar ��ܿ� ���� Ƚ�� ���� ��¿� ���

	switch (iMessage) {
	case WM_CREATE:
		hWndMain = hWnd;
		if (mTrue) {
			sndPlaySound(_T("MetalBGM.wav"), SND_ASYNC);
		}
		

		SetRect(&crt, 0, 0, 900, BH * 18);
		AdjustWindowRect(&crt, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE);
		SetWindowPos(hWnd, NULL, 0, 0, crt.right - crt.left, crt.bottom - crt.top,
			SWP_NOMOVE | SWP_NOZORDER);
		
		SetTimer(hWnd, 1, 30, NULL);		//1�� Ÿ�̸�(���� �̵�)
		SetTimer(hWnd, 2, 30, NULL);		//1�� Ÿ�̸�(���� �̵�)

		for (i = 0; i<20; i++) {
			hBit[i] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1 + i));
		}
		nStage = 0;
		InitStage();		
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		//p = CenterPoint();
		//SetRect(&ballR, CenterPoint().x - 5, CenterPoint().y - 5, CenterPoint().x + 5, CenterPoint().y + 5); // ball ����	
		
		//DrawObject(hdc, ballR, RGB(255, 255, 255), RGB(255, 0, 255), 1);

		DrawScreen(hdc);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_LEFT:
		case VK_RIGHT:
		case VK_UP:
		case VK_DOWN:
			Move(wParam);
			if (TestEnd()) {
				wsprintf(Message, TEXT("Stage %d Clear !!!\n")
					TEXT("���� ���������� �̵��մϴ�."), nStage + 1);
				MessageBox(hWnd, Message, _T("�˸�"), MB_OK);
				if (nStage < MAXSTAGE - 1) {
					nStage++;
				}
				InitStage();
			}
			break;
		case VK_SPACE:			
			Shot(wParam);			
			break;
		case 'Q':
			DestroyWindow(hWnd);
			break;
		case 'R':
			InitStage();
			break;
		case 'N':
			if (nStage < MAXSTAGE - 1) {
				nStage++;
				InitStage();
			}
			break;
		case 'P':
			if (nStage > 0) {
				nStage--;
				InitStage();
			}
			break;
		case 'M':
			mTrue = false;
			break;
		}
		return 0;
	case WM_TIMER:		
		hdc = GetDC(hWnd);
		if (wParam == 1) {

//			DrawObject(hdc, ballR, RGB(255, 255, 255), RGB(255, 0, 255), 1);
			if (test) {
				count++;
				if (ns[by + ay * (nMove+1)][bx + ax * (nMove+1)] == '#') {
					DrawBitmap(hdc, bx*BW + ax * (nMove) * 32, by * BH + ay * (nMove) * 32, hBit[17]);
					test = false;
					nMove = 0;					
				}
				else if (ns[by + ay * (nMove + 1)][bx + ax * (nMove + 1)] == '0' || ns[by + ay * (nMove + 1)][bx + ax * (nMove + 1)] == '+') {
					DrawBitmap(hdc, bx*BW + ax * (nMove+1) * 32, by * BH + ay * (nMove+1) * 32, hBit[17]);
					ns[by + ay * (nMove + 1)][bx + ax * (nMove + 1)] = ' ';	// ��ź�� �������� ���
					test = false;
					nMove = 0;
					tCount++;
					
				}
				else {
					if (count == 2) {
						DrawBitmap(hdc, bx*BW + ax * (nMove) * 32, by * BH + ay * (nMove) * 32, hBit[17]);
						ns[by + ay * (nMove+1)][bx + ax * (nMove+1)] = ' ';	// ��ź�� �������� ���
						nMove++;
						count = 0;						
					}
					//��ź ���
					DrawBitmap(hdc, bx*BW + ax * nMove * 32, by * BH + ay * nMove * 32, hBit[bBit + 5 + count]);
				}
				
			}

			//OffsetRect(&ballR, 3 * ballToggleX, 3 * ballToggleY); //
		//	DrawObject(hdc, ballR, RGB(255, 255, 255), RGB(255, 0, 255), 1);				
		}
		else if(wParam == 2) {
			//p = CenterPoint();
			//SetRect(&ballR, p.x - 5, p.y - 5, p.x + 5, p.y + 5); // ball ����	
		}

		ReleaseDC(hWnd, hdc);	// GetDC( ) �޸� ����
			
		return 0;
	case WM_DESTROY:
		for (i = 0; i < 20; i++) {
			DeleteObject(hBit[i]);
		}
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

void DrawScreen(HDC hdc) {
	int x, y;
	int iBit;
	TCHAR Message[256];	

	for (y = 0; y < 18; y++) {
		for (x = 0; x < 20; x++) {
			switch (ns[y][x]) {
			case '#':
				iBit = 0;		//����
				break;
			case 'W':
				iBit = 1;		//�� ��ũ(��)
				break;
			case 'A':
				iBit = 2;		//�� ��ũ(��)
				break;
			case 'S':
				iBit = 3;		//�� ��ũ(��)
				break;
			case 'D':
				iBit = 4;		//�� ��ũ(��)
				break;
			case '0':
				iBit = 18;		//ĳ����
				break;
				//��ź ��Ʈ�� �ּ�
			case ' ':
				iBit = 17;		//�����
				break;	
			case '+':	//����
				iBit = 19;
				break;
				
			}
			DrawBitmap(hdc, x*BW, y*BH, hBit[iBit]);
		}
	}
	//��ũ ���
	DrawBitmap(hdc, nx*BW, ny * BH, hBit[tBit]);	

	wsprintf(Message, _T("Tank Game"));
	TextOut(hdc, 700, 10, Message, lstrlen(Message));
	wsprintf(Message, _T("Q: ����, R: �ٽ� ����"));
	TextOut(hdc, 700, 30, Message, lstrlen(Message));
	wsprintf(Message, _T("N: ����, P: ����"));
	TextOut(hdc, 700, 50, Message, lstrlen(Message));
	wsprintf(Message, _T("�������� : %d"), nStage + 1);
	TextOut(hdc, 700, 70, Message, lstrlen(Message));
	wsprintf(Message, _T("���� : %d"), tCount*100);
	TextOut(hdc, 700, 90, Message, lstrlen(Message));
	//wsprintf(Message, _T("M: ���Ұ�"));
	//TextOut(hdc, 700, 110, Message, lstrlen(Message));
}

BOOL TestEnd() {
	int x, y;

	for (y = 0; y < 18; y++) {
		for (x = 0; x < 20; x++) {
			if (ns[y][x] == '0')
				return FALSE;
		}
	}
	return TRUE;
}
void Move(int dir) {
	int dx = 0, dy = 0;	
	switch (dir) {
	case VK_LEFT:
		tBit = 2;
		dx = -1;
		InvalidateRect(hWndMain, NULL, TRUE);
		if (ns[ny + dy][nx + dx] != '#'&& ns[ny + dy][nx + dx] != '0' && ns[ny + dy][nx + dx] != '+') {	//���� ��ġ�� ���� �ƴѰ��
			ns[ny][nx] = ' ';	//���� ������ ���������
			ns[ny + dy][nx + dx] = 'A';
			nx += dx;	//���� ��ġ ����
			ny += dy;	//���� y��ǥ ����				
		}
		else
			return;
		break;
	case VK_RIGHT:
		tBit = 4;
		dx = 1;
		InvalidateRect(hWndMain, NULL, TRUE);
		if (ns[ny + dy][nx + dx] != '#'&& ns[ny + dy][nx + dx] != '0' && ns[ny + dy][nx + dx] != '+') {	//���� ��ġ�� ���� �ƴѰ��
			ns[ny][nx] = ' ';	//���� ������ ���������
			ns[ny + dy][nx + dx] = 'D';
			nx += dx;	//���� ��ġ ����
			ny += dy;	//���� y��ǥ ����		
		}
		else
			return;
		break;
	case VK_UP:		
		tBit = 1;
		dy = -1;
		InvalidateRect(hWndMain, NULL, TRUE);
		if (ns[ny + dy][nx + dx] != '#'&& ns[ny + dy][nx + dx] != '0' && ns[ny + dy][nx + dx] != '+') {	//���� ��ġ�� ���� �ƴѰ��
			ns[ny][nx] = ' ';	//���� ������ ���������
			ns[ny + dy][nx + dx] = 'W';
			nx += dx;	//���� ��ġ ����
			ny += dy;	//���� y��ǥ ����	
			
			//OffsetRect(&ballR, 0, -32);
		}
		else
			return;
		break;
	case VK_DOWN:
		tBit = 3;
		dy = 1;
		InvalidateRect(hWndMain, NULL, TRUE);
		if (ns[ny + dy][nx + dx] != '#'&& ns[ny + dy][nx + dx] != '0' && ns[ny + dy][nx + dx] != '+') {	//���� ��ġ�� ���� �ƴѰ��
			ns[ny][nx] = ' ';	//���� ������ ���������
			ns[ny + dy][nx + dx] = 'S';
			nx += dx;	//���� ��ġ ����
			ny += dy;	//���� y��ǥ ����
		}
		else
			return;
		break;
	}
	
}
void InitStage() {
	int x, y;
	tCount = 0;
	tBit = 1;		// ���� ��ũ ���� �̹���
	memcpy(ns, arStage[nStage], sizeof(ns));
	for (y = 0; y < 18; y++) {
		for (x = 0; x < 20; x++) {
			if (ns[y][x] == 'W') {	//�ڽ��� ĳ���� ��ġ�ΰ���
				nx = x;	//x��ǥ ����
				ny = y;	//y��ǥ ����	���� ���� ��ŸƮ!!!!!!
				ns[y][x] = ' ';	// �ʱ� �ڽ��� ��ġ ���������				
			}			
		}
	}			
	InvalidateRect(hWndMain, NULL, TRUE);
}
void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit) {
	HDC MemDC;
	HBITMAP OldBitmap;
	int bx, by;
	BITMAP bit;

	MemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);

	GetObject(hBit, sizeof(BITMAP), &bit);
	bx = bit.bmWidth;
	by = bit.bmHeight;

	BitBlt(hdc, x, y, bx, by, MemDC, 0, 0, SRCCOPY);

	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}

POINT CenterPoint() {	
	int x, y;
	
	for (y = 0; y < 18; y++) {
		for (x = 0; x < 20; x++) {
			if (ns[y][x] == 'W' || ns[y][x] == 'A' || ns[y][x] == 'S' || ns[y][x] == 'D' ) {	//�ڽ��� ĳ���� ��ġ�ΰ���
				p.x = nx * 32 + 16;
				p.y = ny * 32 + 16;
			}
		}
	}	
	return p;
}

void Shot(int dir) {	
	int x, y;			
	
	bx = nx;
	by = ny;
	switch (dir) {
	case VK_SPACE:
		for (y = 0; y < 18; y++) {
			for (x = 0; x < 20; x++) {
				switch (ns[y][x]) {
				case '#':
					break;
				case 'W':	
					if (test == false) {
						ax = 0;
						test = true;
						bBit = 1;// +count;					
						ay = -1;
						if (ns[by + ay][bx + ax] != '#') {	//��ũ ���� ��ġ�� ���� �ƴѰ��						
							bx += ax;	//���� ��ġ ����
							by += ay;	//���� y��ǥ ����		
						}
						else//���� ��ġ�� ���̸� ���� �ȵǵ���(�켱)
							return;
					}
					break;
				case 'A':
					if (test == false) {
						ax = -1;
						test = true;
						bBit = 4;// +count;					
						ay = 0;
						if (ns[by + ay][bx + ax] != '#') {	//��ũ ���� ��ġ�� ���� �ƴѰ��						
							bx += ax;	//���� ��ġ ����
							by += ay;	//���� y��ǥ ����		
						}
						else//���� ��ġ�� ���̸� ���� �ȵǵ���(�켱)
							return;
					}
					break;
				case 'S':
					if (test == false) {
						ax = 0;
						test = true;
						bBit = 7;// +count;					
						ay = 1;
						if (ns[by + ay][bx + ax] != '#') {	//��ũ ���� ��ġ�� ���� �ƴѰ��						
							bx += ax;	//���� ��ġ ����
							by += ay;	//���� y��ǥ ����		
						}
						else//���� ��ġ�� ���̸� ���� �ȵǵ���(�켱)
							return;
					}
					break;
				case 'D':
					if (test == false) {
						ax = 1;
						test = true;
						bBit = 10;// +count;					
						ay = 0;
						if (ns[by + ay][bx + ax] != '#') {	//��ũ ���� ��ġ�� ���� �ƴѰ��						
							bx += ax;	//���� ��ġ ����
							by += ay;	//���� y��ǥ ����		
						}
						else//���� ��ġ�� ���̸� ���� �ȵǵ���(�켱)
							return;
					}
					break;
				}

			}
		}
	}
}

void DrawObject(HDC hdc, RECT& r, COLORREF color, int type, int ropCode)
{
	DrawObject(hdc, r, color, color, type);
}

//��ü(��ź �׸��� �Լ�)
//2�� �����Ͽ� ���� ������� ����
void DrawObject(HDC hdc, RECT& r, COLORREF penC, COLORREF brushC,
	int type, int ropCode)
{
	HPEN hPen, hOldPen;
	HBRUSH hBrush, hOldBrush;
	SetROP2(hdc, ropCode);	// ROP ���� �Լ� �߰�

	hPen = CreatePen(PS_SOLID, 1, penC);
	hOldPen = (HPEN)SelectObject(hdc, hPen);
	hBrush = CreateSolidBrush(brushC);
	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	switch (type) {		// type�� 0: �簢��, 1: ��
	case 0:
		Rectangle(hdc, r.left, r.top, r.right, r.bottom);
		break;
	case 1:
		Ellipse(hdc, r.left, r.top, r.right, r.bottom);
		break;
	}

	SelectObject(hdc, hOldPen);			SelectObject(hdc, hOldBrush);
	DeleteObject(hPen);		DeleteObject(hBrush);
}