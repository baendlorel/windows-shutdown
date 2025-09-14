#include "framework.h"
#include "singleton.h"
#include <unordered_map>

enum class EventType {
    MouseMove,
    MouseClick,
    KeyPress,
};

// Global event hub
class AppEvent {
    SINGLETON(AppEvent)

   private:
    // event listeners
    std::unordered_map<EventType, std::vector<std::function<void()>>> listeners;

   public:
    void On(EventType evt, std::function<void()> handler) {
        this->listeners[evt].push_back(std::move(handler));
    }

    void Emit(EventType evt) {
        auto& evtListeners = this->listeners[evt];
        for (auto& evtListener : evtListeners) {
            evtListener();
        }
    }
};