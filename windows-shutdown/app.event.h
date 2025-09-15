#pragma once
#include <unordered_map>

#include "consts.app.h"
#include "framework.h"
#include "utils.class.h"

// Global event hub
class AppEvent {
    SINGLETON(AppEvent)

   private:
    // event listeners
    std::unordered_map<app::EventType, std::vector<std::function<void()>>> listeners_;

   public:
    void on(const app::EventType evt, std::function<void()> handler) {
        this->listeners_[evt].push_back(std::move(handler));
    }

    void emit(const app::EventType evt) {
        for (auto& evtListener : this->listeners_[evt]) {
            evtListener();
        }
    }
};