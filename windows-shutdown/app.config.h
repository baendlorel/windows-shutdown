#pragma once
#include "framework.h"
#include "singleton.h"

// consts
#include "consts.config.h"
#include "consts.app.h"
#include "style.color.h"

class AppConfig {
    SINGLETON(AppConfig)

    // config entries
   public:
    CFG::Lang lang = CFG::Lang::Zh;
    Action action = Action::None;
    CFG::Instruction instruction = CFG::Instruction::Show;
    std::vector<Action> menuButtons = {Action::Donate, Action::Config,  Action::Lock,
                                       Action::Sleep,  Action::Restart, Action::Shutdown};
    CFG::CountdownStyle countdownStyle = CFG::CountdownStyle::SteinsGate;
    int delay = CFG::CFG_DEFAULT_DELAY;
    Gdiplus::Color backgroundColor = ColorSet::GetInstance().BackgroundColor;
    std::vector<CFG::WarnInfo> warnings;

   public:
    void Load();

    bool IsImmediate() const {
        return action != Action::None;
    }

    std::wstring GetConfigPath();

   private:
    CFG::Warning LoadKeyValue(std::string& key, std::string& value);
};
