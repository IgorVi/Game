
#include "Game.h"


#define WIDTH  60
#define HEIGHT 60

Game::Game()
{
	ptr = this;
}

Game* Game::ptr = NULL;

void Game::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}

BOOL Game::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	srand((unsigned)time(0));
	hDialog = hwnd;

	// дескрипторы элементов управления
	hStart = GetDlgItem(hDialog, IDC_NEWGAME);
	
	hEditRez = GetDlgItem(hDialog, IDC_LIST);
	
	hProgress = GetDlgItem(hDialog, IDC_PROGRESS1);
	SendMessage(hProgress, PBM_SETSTEP, 1, 0); // Установка шага приращения  индикатора 
	SendMessage(hProgress, PBM_SETPOS, 0, 0);
	SendMessage(hProgress, PBM_SETBKCOLOR, 0, LPARAM(RGB(255, 255, 0))); // Установка цвета фона индикатора
	SendMessage(hProgress, PBM_SETBARCOLOR, 0, LPARAM(RGB(255, 0, 0))); // Установка цвета заполняемых прямоугольников

	hEdit = GetDlgItem(hDialog, IDC_EDIT);
	hSpin = GetDlgItem(hDialog, IDC_SPIN1);
	// диапазон счётчика
	SendMessage(hSpin, UDM_SETRANGE32, 1, 100);	
	// установка "приятеля" для счётчика
	SendMessage(hSpin, UDM_SETBUDDY, WPARAM(hEdit), 0);
	SetWindowText(hEdit, TEXT("30"));

	// загрузка иконки из ресурсов
	hIcon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(IDI_ICON1));

	TCHAR buf[3] = { 0 };
	for (int i = 0; i < N; ++i){
		for (int j = 0; j < N; ++j){
			h[i][j] = CreateWindowEx(WS_EX_DLGMODALFRAME, TEXT("BUTTON"), 0, WS_CHILD | WS_VISIBLE,
				(j * WIDTH) + 11, (i * HEIGHT) + 11, WIDTH, HEIGHT, hwnd, 0, hInst, 0);
		}
	}

	return TRUE;
}

void Game::GenerationNumbersButtons(HWND hwnd) {
	vec.clear();
	int n;
	do {
		n = rand() % 100;
		unsigned int i = 0;
		for (; i < vec.size() && vec[i] != n; ++i){}
		
		if (i == vec.size()) vec.push_back(n);
	} while (vec.size() < N*N);


	TCHAR buf[3] = { 0 };
	for (int j = 0; j < N * N; ++j){
		wsprintf(buf, TEXT("%d"), vec[j]);
		SetWindowText(h[0][j], buf);
		EnableWindow(h[0][j], 1);
	}
	
	sort(vec.begin(), vec.end(), std::greater<int>());
}

void Game::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	if (id == IDC_NEWGAME) {
		SendMessage(hEditRez, LB_RESETCONTENT, 0, 0);
		GenerationNumbersButtons(hwnd);
		MaxTic = SendMessage(hSpin, UDM_GETPOS32, 0, 0);
		SendMessage(hProgress, PBM_SETRANGE, 0, MAKELPARAM(0, MaxTic)); // Установка интервала для индикатора 
		SendMessage(hProgress, PBM_SETPOS, 1, 0); // Установка текущей позиции индикатора
		SetTimer(hwnd, 1, 1000, 0);
		tic = 0;
		SendMessage(hProgress, PBM_SETPOS, WPARAM(0), 0);
		SetWindowText(hwnd, TEXT("00:0"));
	}
	else if (codeNotify == BN_CLICKED && tic < MaxTic) {
		TCHAR text[3] = { 0 };
		GetWindowText(hwndCtl, text, 3);

		if (vec.back() == _wtoi(text)) {
			EnableWindow(hwndCtl, 0);
			SendMessage(hEditRez, LB_ADDSTRING, 0, LPARAM(text));
			vec.pop_back();
			if (vec.size() == 0) {
				KillTimer(hwnd, 1);
				MessageBox(0, TEXT("Вы выиграли"), TEXT(""), MB_OK);
				SetFocus(hStart);
			}
		}
		else {
			if ((tic += 4) < MaxTic){ ; }
			else { tic = MaxTic - 1; }
			SendMessage(hwnd, WM_TIMER, 1, 0);
		}
	}
}

void Game::Cls_OnTimer(HWND hwnd, UINT id)
{
	++tic;
	TCHAR str[6];
	wsprintf(str, TEXT("0%d:%d"), tic / 60, tic % 60);
	SetWindowText(hwnd, str);
	SendMessage(hProgress, PBM_SETPOS, WPARAM(tic), 0); // установка текущей позиции индикатора
	if (tic >= MaxTic) {
		KillTimer(hwnd, 1);
		if (vec.size() > 0){
			MessageBox(0, TEXT("Вы проиграли"), TEXT(""), MB_OK);
			SetFocus(hStart);
		}
	}
}

BOOL CALLBACK Game::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
		HANDLE_MSG(hwnd, WM_TIMER, ptr->Cls_OnTimer);
	}
	return FALSE;
}