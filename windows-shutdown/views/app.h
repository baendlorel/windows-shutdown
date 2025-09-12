#include "home.h"
#include "donate.h"
#include "countdown.h"

struct App {
   public:
    static App& GetInstance() {
        static App instance;
        return instance;
    }

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

   private:
    App() = default;
};