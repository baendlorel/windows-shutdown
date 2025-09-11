#pragma once
#include "framework.h"
#include <functional>

// consts
#include "consts/core.h"

struct MenuButton {
   public:
    int x;
    int y;
    std::function<void(HWND)> onClickCallback;

    Action action;

    int resId;
    Gdiplus::Bitmap* png;

   public:
    MenuButton(Action action);

    void LoadPNGFromResource(HINSTANCE hInst);

    void Center(int buttonCount, int index, int w, int h);

    bool MouseHit(int mx, int my) const;

    void OnClick(std::function<void(HWND)> cb) {
        this->onClickCallback = std::move(cb);
    }
};