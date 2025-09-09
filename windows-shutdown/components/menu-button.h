#pragma once
#include "framework.h"
#include <functional>

// consts
#include "consts/core.h"

struct MenuButton {
   public:
    Action action;

    int x;
    int y;

    int resId;
    Gdiplus::Bitmap* png;

    // Callback invoked when this menu button is clicked. Receives the HWND of the window.
    std::function<void(HWND)> onClickCallback;

   public:
    MenuButton(int resId, Action action);

    void LoadPNGFromResource(HINSTANCE hInst);

    void Center(int buttonCount, int index, int w, int h);

    bool MouseHit(int mx, int my) const;

    void OnClick(std::function<void(HWND)> cb) {
        this->onClickCallback = std::move(cb);
    }
};