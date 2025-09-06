#pragma once
#include "framework.h"
#include "consts/core.h"
#include "consts/effects.h"

void ExecuteRestart();

void ExecuteShutdown();

void ExecuteSleep();

void ExecuteLock();

void StartCountdown(HWND hWnd, Action action);

void CancelCountdown(HWND hWnd);

void TriggerDonate(HWND hWnd);

void TriggerConfig(HWND hWnd);

void TriggerLock(HWND hWnd);

void TriggerSleep(HWND hWnd);

void TriggerRestart(HWND hWnd);

void TriggerShutdown(HWND hWnd);
