#pragma once


static ATOM MyRegisterClass(HINSTANCE hInstance);

static BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);