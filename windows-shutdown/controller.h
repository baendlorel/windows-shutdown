#pragma once
#include "framework.h"
#include "singleton.h"
#include "app.core.h"
#include "consts.app.h"

namespace controller {
void execute_restart();
void execute_shutdown();
void execute_sleep();
void execute_lock();
void execute_action(const HWND hWnd, const app::Action action);
void start_countdown(const HWND hWnd, const app::Action action);
void cancel_countdown(const HWND hWnd);
void trigger_donate(const HWND hWnd);
void trigger_config(const HWND hWnd);
void trigger_lock(const HWND hWnd);
};  // namespace controller
