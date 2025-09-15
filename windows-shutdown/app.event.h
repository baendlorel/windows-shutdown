#pragma once
#include <stdexcept>
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
    int on(const app::EventType evt, std::function<void()> handler) {
        auto& evtListeners = this->listeners_[evt];
        evtListeners.push_back(std::move(handler));
        return static_cast<int>(evtListeners.size()) - 1;
    }

    void emit(const app::EventType evt) {
        for (auto& evtListener : this->listeners_[evt]) {
            if (evtListener) {
                evtListener();
            }
        }
    }

    void off(const app::EventType evt, const size_t listener_id) {
        auto& evtListeners = this->listeners_[evt];
        if (evtListeners.size() <= listener_id) {
            throw std::runtime_error("Cannot find listener id" + static_cast<int>(listener_id));
        }
        evtListeners[listener_id] = nullptr;
    }
};