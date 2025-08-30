#pragma once

void ExecuteRestart();

void ExecuteShutdown();

void StartCountdown(HWND hWnd, bool isRestart);

void CancelCountdown(HWND hWnd);

void TriggerRestart(HWND hWnd);

void TriggerShutdown(HWND hWnd);
