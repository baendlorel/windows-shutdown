#pragma once
#include "framework.h"
#include "singleton.h"

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
    std::vector<Action> menu_buttons = {Action::Donate, Action::Config,  Action::Lock,
                                        Action::Sleep,  Action::Restart, Action::Shutdown};
    CFG::CountdownStyle countdown_style = CFG::CountdownStyle::SteinsGate;
    int delay = CFG::CFG_DEFAULT_DELAY;
    Gdiplus::Color background_color = ColorSet::BACKGROUND;
    std::vector<CFG::WarnInfo> warnings;

   public:
    void load();

    [[nodiscard]] bool is_immediate() const {
        return action != Action::None;
    }

    std::wstring get_config_path();

   private:
    CFG::Warning load_key_value(std::string& key, std::string& value);
};
