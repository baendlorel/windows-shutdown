#pragma once
#include "mini-ui.h"
#include "consts.app.h"

class MenuButton : public Div {
   public:
    app::Action action;
    int res_id;
    Gdiplus::Bitmap* png;

    BYTE __drawingalpha = 0;

   public:
    MenuButton(const int x, const int y, const app::Action action);

   public:
    bool MouseHit(const int mx, const int my) override;
    void draw(Gdiplus::Graphics& graphics, const DrawParams& params) override;

    // menu buttons are centered horizontally
    void center(const int button_count, const int index, const int w, const int h);

   private:
    void draw_view(Gdiplus::Graphics& graphics, const DrawParams& params) override {
    }
};