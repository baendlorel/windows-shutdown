#pragma once
#include <unordered_map>
#include "framework.h"
#include "singleton.h"

enum class EventType : unsigned char { MouseMove, MouseClick, KeyPress, Redraw };

// Global event hub
class AppEvent {
    SINGLETON(AppEvent)

   private:
    // event listeners
    std::unordered_map<EventType, std::vector<std::function<void()>>> listeners_;

   public:
    void on(const EventType evt, std::function<void()>& handler) {
        this->listeners_[evt].push_back(std::move(handler));
    }

    void emit(const EventType evt) {
        for (auto& evtListener : this->listeners_[evt]) {
            evtListener();
        }
    }
};