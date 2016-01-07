#pragma once

#include "header.h"

#define N      4

class Game
{
public:
	Game();
public:
	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static Game* ptr;
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	void Cls_OnClose(HWND hwnd);
	void Cls_OnTimer(HWND hwnd, UINT id);

	HWND hDialog, hSpin, hStart, hEditRez, hEdit, hProgress;
	HICON hIcon;
	HINSTANCE hInst;
private:
	void GenerationNumbersButtons(HWND hwnd);
	int tic;
	int MaxTic;
	HWND h[N][N];
	std::vector<int> vec;
};