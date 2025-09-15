#pragma once
#include "framework.h"
#include "utils.class.h"

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
    void draw(Gdiplus::Graphics& graphics, const DrawParams& params) {
        home.update(graphics, params);
        countdown.update(graphics, params);
        donate.update(graphics, params);
    }
};
