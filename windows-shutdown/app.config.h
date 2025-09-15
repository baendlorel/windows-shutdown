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
    cfg::Lang lang = cfg::Lang::Zh;
    Action action = Action::None;
    cfg::Instruction instruction = cfg::Instruction::Show;
    std::vector<Action> menu_buttons = {Action::Donate, Action::Config,  Action::Lock,
                                        Action::Sleep,  Action::Restart, Action::Shutdown};
    cfg::CountdownStyle countdown_style = cfg::CountdownStyle::SteinsGate;
    int delay = cfg::CFG_DEFAULT_DELAY;
    Gdiplus::Color background_color = color_set::BACKGROUND;
    std::vector<cfg::WarnInfo> warnings;

   public:
    void load();

    [[nodiscard]] bool is_immediate() const {
        return action != Action::None;
    }

    std::wstring get_config_path();

   private:
    cfg::Warning load_key_value(std::string& key, std::string& value);
};
