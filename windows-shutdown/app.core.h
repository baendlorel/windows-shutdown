#pragma once

// Explicitly merge namespace app
// ReSharper disable once CppUnusedIncludeDirective
#include "consts.app.h"

// app series
#include "app.event.h"
#include "app.config.h"
#include "app.page.h"
#include "app.state.h"
#include "app.i18n.h"

namespace app {
inline AppEvent& event = AppEvent::get_instance();
inline AppConfig& config = AppConfig::get_instance();
inline AppState& state = AppState::get_instance();
inline AppPage& page = AppPage::get_instance();
// ReSharper disable once CppInconsistentNaming
inline AppI18N& i18n = AppI18N::get_instance();
}  // namespace app
