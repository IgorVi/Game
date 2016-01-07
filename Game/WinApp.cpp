#include "Game.h"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpszCmdLine, int nCmdShow)
{
	Game g;
	g.hInst = hInst;
	return DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, Game::DlgProc);
}