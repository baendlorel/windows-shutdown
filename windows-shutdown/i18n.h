#pragma once
#include <string>

#include "app-state.h"
#include "consts.h"

enum class WaitType { Sleep, Shutdown, Restart };

struct I18N {
 public:
  static void Init(const AppState& appState) {
    I18N::getInstance().lang = appState.config.lang;    
  }

  static I18N& getInstance() {
    static I18N instance;
    return instance;
  }

 private:
  I18N();

  Lang lang = Lang::ZH;

 public:
  I18N(const I18N&) = delete;
  I18N& operator=(const I18N&) = delete;

  I18N(I18N&&) = delete;
  I18N& operator=(I18N&&) = delete;

  std::string PressAnyKeyToExitText() const;
  std::string PressAnyKeyToCancelText() const;
  std::string WaitText(WaitType type, int seconds) const;
};