#pragma once


ATOM MyRegisterClass();

BOOL InitInstance(int nCmdShow);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);