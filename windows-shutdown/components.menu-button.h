#pragma once
#include "mini-ui.h"
#include "consts.app.h"

class MenuButton : public Div {
   public:
    Action action;
    int res_id;
    Gdiplus::Bitmap* png;

    BYTE __drawingalpha = 0;

   public:
    MenuButton(int x, int y, Action action);

   public:
    bool MouseHit(int mx, int my) override;
    void draw(Gdiplus::Graphics& graphics, const DrawParams& params) override;

    // menu buttons are centered horizontally
    void center(int button_count, int index, int w, int h);

   private:
    void DrawView(Gdiplus::Graphics& graphics, const DrawParams& params) override {
    }
};