// Shashki.cpp: ���������� ����� ����� ��� ����������.

#include "stdafx.h"
#include "Resource.h"
#include "GamePainter.h"
#include "Player.h"

#define MAX_LOADSTRING 100

// ���������� ����������:
HINSTANCE hInst;								// ������� ���������
TCHAR szTitle[MAX_LOADSTRING];					// ����� ������ ���������
TCHAR szWindowClass[MAX_LOADSTRING];			// ��� ������ �������� ����
HWND hWnd;

GamePainter * gp ;	// ������ ���������
Game * game ;		// ������ ����
Player * player ;   // ������ ������
STEP step ;			// ������� ���
int w ;
int h ;
bool gameover ;		// ����� ����� ����

// ��������� ���������� �������, ���������� � ���� ������ ����:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine, _In_ int       nCmdShow){
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: ���������� ��� �����.
	MSG msg;
	HACCEL hAccelTable;

	// ������������� ���������� �����
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SHASHKI, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ��������� ������������� ����������:
	if (!InitInstance (hInstance, nCmdShow)) return FALSE;
	
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SHASHKI));

	// ���� ��������� ���������:
	while (GetMessage(&msg, NULL, 0, 0))
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))	{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

	return (int) msg.wParam;
}

//   ������������ ����� ����.

ATOM MyRegisterClass(HINSTANCE hInstance){
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SHASHKI));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_SHASHKI);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

// ���������� �����
void refreshForm() {
	InvalidateRect(hWnd,NULL,true) ;
	UpdateWindow(hWnd) ;
}

// ���������� ����
void updateStep() {
	if (gameover) 
		SetWindowTextW(hWnd,L"���� ��������") ;
	else {
		if (step==sHuman) SetWindowTextW(hWnd,L"��� ������") ;
		else SetWindowTextW(hWnd,L"��� ����������") ;
	}
}

// ����� ����
void NewGame() {
	// �������� ��������
	gp = new GamePainter() ;
	game = new Game() ;
	player = new Player(game) ;
	gp->setGame(game) ;
	// ������ ��� ��������
	step = sHuman ;
	gameover = false ;
	updateStep() ;
}

//    � ������ ������� ���������� ���������� ����������� � ���������� ����������, � �����
//    ��������� � ��������� �� ����� ������� ���� ���������.

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow){
   hInst = hInstance; // ��������� ���������� ���������� � ���������� ����������

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd) return FALSE;
     
      // ���������������� ���� �� ������
   int sw = GetSystemMetrics(SM_CXSCREEN); 
   int sh = GetSystemMetrics(SM_CYSCREEN); 

   // ��������� �����
   w = 600 ;
   h = 600 ;
   MoveWindow(hWnd, (sw-w)/2,(sh-h)/2,w,h,true) ;
   
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   updateStep() ;
   return TRUE;
}

// ������� ������� - ��� ����������
void CALLBACK TimerProc(HWND hWnd, UINT Msg, UINT Event, long unsigned int Time) {
	player->makeStep();

	refreshForm();
	bool win;
	step = sHuman;
	// ��������, �� ����� �� ����
	if (game->testWin(&win,step)) 
		if (!gameover) {
			gameover = true;
			if (win) MessageBoxA(hWnd, "�� ��������!", "OK", 0);
			else MessageBoxA(hWnd, "�� ���������!", "OK", 0);
		}
	updateStep() ;
	KillTimer(hWnd,ID_TIMER) ;  
}


//  ������������ ��������� � ������� ����.
//
//  WM_COMMAND	- ��������� ���� ����������
//  WM_PAINT	-��������� ������� ����
//  WM_DESTROY	 - ������ ��������� � ������ � ���������.

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	CLICKRESULT cr ;
	bool win ;

	switch (message){
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// ��������� ����� � ����:
		switch (wmId){
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_START:
			NewGame() ;
			refreshForm() ;
			break ;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		// ��������� �����
		hdc = BeginPaint(hWnd, &ps);
		gp->drawTo(hdc,w,h) ;
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		if (gameover) break ;
		if (step!=sHuman) break ;
		// �������� ������ ����
		cr = gp->doClick(LOWORD(lParam),HIWORD(lParam)) ;
		refreshForm() ;
		// ���� �������� ���
		if (cr==crFinish) {
			step=sComp ;
			// ��������, �� ����� �� ����
			if (game->testWin(&win,step)) {
				if (!gameover) {
					gameover = true;
					if (win) MessageBoxA(hWnd, "�� ��������!", "OK", 0);
					else MessageBoxA(hWnd, "�� ���������!", "OK", 0);
				}
			}
			else {
				SetTimer(hWnd,ID_TIMER,1000,TimerProc) ;
			}
			updateStep() ;
		}
		break;
	case WM_CREATE:
		NewGame() ;
		break ;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// ���������� ��������� ��� ���� "� ���������".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam){
	UNREFERENCED_PARAMETER(lParam);
	switch (message)	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL){
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}