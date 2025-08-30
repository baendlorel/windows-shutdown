#pragma once
#include "framework.h"


static ATOM MyRegisterClass(HINSTANCE hInstance);

static BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);