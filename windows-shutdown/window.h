#pragma once
#include "framework.h"
#include "consts/state.h"
#include "consts/effects.h"

ATOM MyRegisterClass();

BOOL InitInstance(int nCmdShow);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);