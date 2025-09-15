#pragma once
#include "framework.h"
#include "app.core.h"

namespace controller {
void execute_restart();
void execute_shutdown();
void execute_sleep();
void execute_lock();
void execute_action(HWND hWnd, app::Action action);
void start_countdown(HWND hWnd, app::Action action);
void cancel_countdown(HWND hWnd);
void trigger_donate(HWND hWnd);
void trigger_config(HWND hWnd);
void trigger_lock(HWND hWnd);
};  // namespace controller
