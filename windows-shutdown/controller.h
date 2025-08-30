#pragma once

static void ExecuteRestart();

static void ExecuteShutdown();

static void StartCountdown(HWND hWnd, bool isRestart);

static void CancelCountdown(HWND hWnd);

static void TriggerRestart(HWND hWnd);

static void TriggerShutdown(HWND hWnd);
