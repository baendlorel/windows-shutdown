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
inline constexpr AppEvent& event = AppEvent::GetInstance();
inline constexpr AppConfig& config = AppConfig::GetInstance();
inline constexpr AppState& state = AppState::GetInstance();
inline constexpr AppPage& page = AppPage::GetInstance();
// ReSharper disable once CppInconsistentNaming
inline constexpr AppI18N& i18n = AppI18N::GetInstance();
}  // namespace app
