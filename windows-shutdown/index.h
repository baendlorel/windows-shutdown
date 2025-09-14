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
    void Draw(Gdiplus::Graphics& graphics, DrawParams& params) {
        home.Draw(graphics, params);
        countdown.Draw(graphics, params);
        donate.Draw(graphics, params);
    }
};
