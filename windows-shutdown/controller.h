#pragma once
#include "framework.h"

void ExecuteRestart();

void ExecuteShutdown();

void ExecuteSleep();

void ExecuteLock();

void StartCountdown(HWND hWnd, bool isRestart, bool isSleep = false);

void CancelCountdown(HWND hWnd);

void TriggerRestart(HWND hWnd);

void TriggerShutdown(HWND hWnd);

void TriggerSleep(HWND hWnd);

void TriggerLock(HWND hWnd);

void TriggerConfig(HWND hWnd);
