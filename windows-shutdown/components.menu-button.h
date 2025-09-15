#pragma once
#include "mini-ui.h"
#include "consts.app.h"

class MenuButton : public Div {
   public:
    app::Action action;
    int res_id;
    Gdiplus::Bitmap* png;

    BYTE debug_drawing_alpha = 0;

   public:
    MenuButton(int x, int y, app::Action action);

   public:
    bool mouse_hit(int mx, int my) override;
    void draw(Gdiplus::Graphics& graphics, const DrawParams& params) override;

    // menu buttons are centered horizontally
    void center(int button_count, int index, int w, int h);

   private:
    void draw_view(Gdiplus::Graphics& graphics, const DrawParams& params) override;
};