#pragma once
#include "framework.h"
#include "render.h"

namespace window {

inline Render* render = Render::GetInstancePtr();

ATOM my_register_class();
void register_menu_button_click_callback();
BOOL init_instance(int nCmdShow);
void handle_timer(HWND hWnd, WPARAM wParam);
void handle_cancel(HWND hWnd);
void handle_mouse_move(HWND hWnd, LPARAM lParam);
void handle_click(HWND hWnd, LPARAM lParam);

// ReSharper disable once CppInconsistentNaming
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};  // namespace window
