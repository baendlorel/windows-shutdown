#pragma once
#include "framework.h"
#include "singleton.h"

// consts
#include "consts.config.h"
#include "consts.app.h"
#include "style.color.h"

class AppConfig {
    SINGLETON(AppConfig)

   public:
    Lang lang = Lang::Zh;
    Action action = Action::None;
    Instruction instruction = Instruction::Show;
    std::vector<Action> menuButtons;
    CountdownStyle countdownStyle = CountdownStyle::SteinsGate;
    int delay = CFG_DEFAULT_DELAY;
    Gdiplus::Color backgroundColor = ColorSet::GetInstance().BackgroundColor;
    std::vector<ConfigWarnInfo> warnings;

   public:
    void Load();

    bool IsImmediate() const {
        return action != Action::None;
    }

    std::wstring GetConfigPath();

   private:
    ConfigWarning LoadKeyValue(std::string& key, std::string& value);
};
