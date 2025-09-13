#pragma once
#include "framework.h"
#include "singleton.h"
#include "app.core.h"

class Controller {
    SINGLETON(Controller)

   private:
    App& app = App::GetInstance();

   public:
    void ExecuteRestart();
    void ExecuteShutdown();
    void ExecuteSleep();
    void ExecuteLock();
    void ExecuteAction(HWND hWnd, Action action);
    void StartCountdown(HWND hWnd, Action action);
    void CancelCountdown(HWND hWnd);
    void TriggerDonate(HWND hWnd);
    void TriggerConfig(HWND hWnd);
    void TriggerLock(HWND hWnd);
};
