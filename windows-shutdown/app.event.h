#pragma once
#include <ranges>
#include <unordered_map>

#include "consts.app.h"
#include "framework.h"
#include "utils.class.h"

// Global event hub
class AppEvent {
    SINGLETON(AppEvent)

   private:
    unsigned int id_ = 0;

    // event listeners
    std::unordered_map<app::EventType, std::unordered_map<unsigned int, std::function<void()>>>
        listeners_map_;

   public:
    unsigned int on(const app::EventType evt, std::function<void()> handler) {
        this->id_++;
        auto& evtListeners = this->listeners_map_[evt];
        evtListeners[this->id_] = std::move(handler);
        return this->id_;
    }

    void emit(const app::EventType evt) {
        for (auto& listener : this->listeners_map_[evt] | std::views::values) {
            if (listener) {
                listener();
            }
        }
    }

    void off(const app::EventType evt, const unsigned int listener_id) {
        this->listeners_map_[evt].erase(listener_id);
    }
};