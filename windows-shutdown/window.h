#pragma once
#include "framework.h"
#include "singleton.h"
#include "controller.h"
#include "render.h"

class Window {
    SINGLETON(Window)

   public:
    controller& controller = Controller::GetInstance();
    Render& render = Render::GetInstance();

   public:
    ATOM MyRegisterClass();
    void RegisterMenuButtonClickCallback();
    BOOL InitInstance(int nCmdShow);
    void HandleTimer(HWND hWnd, WPARAM wParam);
    void HandleCancel(HWND hWnd);
    void HandleMoustMove(HWND hWnd, LPARAM lParam);
    void HandleClick(HWND hWnd, LPARAM lParam);
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
