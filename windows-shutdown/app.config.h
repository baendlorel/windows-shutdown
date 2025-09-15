#pragma once
#include "framework.h"
#include "utils.class.h"

#include "consts.config.h"
#include "consts.app.h"
#include "consts.warning.h"
#include "style.color.h"

class AppConfig {
    SINGLETON(AppConfig)

   public:
    static std::wstring get_config_path();

    // config entries
   public:
    cfg::Lang lang = cfg::Lang::Zh;
    app::Action action = app::Action::None;
    cfg::Instruction instruction = cfg::Instruction::Show;
    std::vector<app::Action> menu_buttons = {app::Action::Donate,  app::Action::Config,
                                             app::Action::Lock,    app::Action::Sleep,
                                             app::Action::Restart, app::Action::Shutdown};
    cfg::CountdownStyle countdown_style = cfg::CountdownStyle::SteinsGate;
    int delay = cfg::CFG_DEFAULT_DELAY;
    Gdiplus::Color background_color = color_set::BACKGROUND;
    std::vector<warning::Info> warnings;

   public:
    void load();

    [[nodiscard]] bool is_immediate() const {
        return action != app::Action::None;
    }

   private:
    warning::Code load_key_value(const std::string& key, const std::string& value);
};
