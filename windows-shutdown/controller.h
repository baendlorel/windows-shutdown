#pragma once
#include "framework.h"
#include "consts/core.h"
#include "consts/effects.h"

void ExecuteAction(HWND hWnd, Action action);

void StartCountdown(HWND hWnd, Action action);

void CancelCountdown(HWND hWnd);

void TriggerDonate(HWND hWnd);

void TriggerConfig(HWND hWnd);

void TriggerLock(HWND hWnd);