// Shashki.cpp: определяет точку входа для приложения.

#include "stdafx.h"
#include "Resource.h"
#include "GamePainter.h"
#include "Player.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;								// текущий экземпляр
TCHAR szTitle[MAX_LOADSTRING];					// Текст строки заголовка
TCHAR szWindowClass[MAX_LOADSTRING];			// имя класса главного окна
HWND hWnd;

GamePainter * gp ;	// Объект рисования
Game * game ;		// Объект игры
Player * player ;   // Объект игрока
STEP step ;			// Текущий ход
int w ;
int h ;
bool gameover ;		// Метка конца игры

// Отправить объявления функций, включенных в этот модуль кода:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine, _In_ int       nCmdShow){
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: разместите код здесь.
	MSG msg;
	HACCEL hAccelTable;

	// Инициализация глобальных строк
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SHASHKI, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance (hInstance, nCmdShow)) return FALSE;
	
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SHASHKI));

	// Цикл основного сообщения:
	while (GetMessage(&msg, NULL, 0, 0))
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))	{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

	return (int) msg.wParam;
}

//   регистрируем класс окна.

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

// Обновление формы
void refreshForm() {
	InvalidateRect(hWnd,NULL,true) ;
	UpdateWindow(hWnd) ;
}

// Обновление хода
void updateStep() {
	if (gameover) 
		SetWindowTextW(hWnd,L"Игра окончена") ;
	else {
		if (step==sHuman) SetWindowTextW(hWnd,L"Ход игрока") ;
		else SetWindowTextW(hWnd,L"Ход компьютера") ;
	}
}

// Новая игра
void NewGame() {
	// Создание объектов
	gp = new GamePainter() ;
	game = new Game() ;
	player = new Player(game) ;
	gp->setGame(game) ;
	// Первый ход человека
	step = sHuman ;
	gameover = false ;
	updateStep() ;
}

//    В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//    создается и выводится на экран главное окно программы.

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow){
   hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd) return FALSE;
     
      // Позиционирование окна по центру
   int sw = GetSystemMetrics(SM_CXSCREEN); 
   int sh = GetSystemMetrics(SM_CYSCREEN); 

   // Центровка формы
   w = 600 ;
   h = 600 ;
   MoveWindow(hWnd, (sw-w)/2,(sh-h)/2,w,h,true) ;
   
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   updateStep() ;
   return TRUE;
}

// Функция таймера - ход компьютера
void CALLBACK TimerProc(HWND hWnd, UINT Msg, UINT Event, long unsigned int Time) {
	player->makeStep();

	refreshForm();
	bool win;
	step = sHuman;
	// Проверка, не конец ли игры
	if (game->testWin(&win,step)) 
		if (!gameover) {
			gameover = true;
			if (win) MessageBoxA(hWnd, "Вы победили!", "OK", 0);
			else MessageBoxA(hWnd, "Вы проиграли!", "OK", 0);
		}
	updateStep() ;
	KillTimer(hWnd,ID_TIMER) ;  
}


//  обрабатываем сообщения в главном окне.
//
//  WM_COMMAND	- обработка меню приложения
//  WM_PAINT	-Закрасить главное окно
//  WM_DESTROY	 - ввести сообщение о выходе и вернуться.

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
		// Разобрать выбор в меню:
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
		// Рисование доски
		hdc = BeginPaint(hWnd, &ps);
		gp->drawTo(hdc,w,h) ;
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		if (gameover) break ;
		if (step!=sHuman) break ;
		// Отправка щелчка мыши
		cr = gp->doClick(LOWORD(lParam),HIWORD(lParam)) ;
		refreshForm() ;
		// Если успешный ход
		if (cr==crFinish) {
			step=sComp ;
			// Проверка, не конец ли игре
			if (game->testWin(&win,step)) {
				if (!gameover) {
					gameover = true;
					if (win) MessageBoxA(hWnd, "Вы победили!", "OK", 0);
					else MessageBoxA(hWnd, "Вы проиграли!", "OK", 0);
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

// Обработчик сообщений для окна "О программе".
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