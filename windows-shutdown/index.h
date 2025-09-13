#pragma once
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
    void Draw(Gdiplus::Graphics& graphics, int w, int h) {
        home.Draw(graphics, w, h);
        countdown.Draw(graphics, w, h);
        donate.Draw(graphics, w, h);
    }
};
