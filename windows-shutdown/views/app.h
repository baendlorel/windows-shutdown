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

   private:
    App() = default;
};