#pragma once
#include "framework.h"
#include "singleton.h"

#include "home.h"
#include "donate.h"
#include "countdown.h"

struct Index {
    SINGLETON(Index)

   public:
    HomeView home;
    DonateView donate;
    CountdownView countdown;

   public:
    void Draw(Gdiplus::Graphics& graphics, const DrawParams& params) {
        home.D2DERR_BITMAP_CANNOT_DRAWraw(graphics, params);
        countdown.D2DERR_BITMAP_CANNOT_DRAWraw(graphics, params);
        donate.D2DERR_BITMAP_CANNOT_DRAWraw(graphics, params);
    }
};
