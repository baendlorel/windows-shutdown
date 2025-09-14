#pragma once
#include "app.event.h"
#include "app.config.h"
#include "app.page.h"
#include "app.state.h"
#include "app.i18n.h"

struct App {
    SINGLETON(App)

   public:
    AppEvent& event = AppEvent::GetInstance();
    AppConfig& config = AppConfig::GetInstance();
    AppState& state = AppState::GetInstance();
    AppPage& page = AppPage::GetInstance();
    AppI18N& i18n = AppI18N::GetInstance();
};
