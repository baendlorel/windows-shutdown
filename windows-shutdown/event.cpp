#include "event.h"
#include <ranges>
#include <algorithm>

unsigned int ListenerEntry::next_id_ = 0;

ListenerEntry::ListenerEntry(std::function<void()> _listener)
    : id(next_id_++), listener(std::move(_listener)) {
}

Event::~Event() {
    static auto delete_listener_entries = [this](const app::EventType evt) {
        for (const auto& entry : this->listeners_map_[evt]) {
            delete entry;
        }
    };

    delete_listener_entries(app::EventType::Redraw);
    delete_listener_entries(app::EventType::MouseClick);
    delete_listener_entries(app::EventType::MouseMove);
    delete_listener_entries(app::EventType::KeyPress);
}

unsigned int Event::on(const app::EventType evt, std::function<void()> listener) {
    auto* entry = new ListenerEntry(std::move(listener));
    this->listeners_map_[evt].push_back(entry);
    return entry->id;
}

void Event::emit(const app::EventType evt) {
    for (auto& listener : this->listeners_map_[evt]) {
        if (listener) {
            listener();
        }
    }
}
bool Event::off(const unsigned int id) {
    static auto off_evt = [this](const app::EventType evt, const unsigned int _id) {
        auto& entries = this->listeners_map_[evt];

        const auto it =
            std::ranges::find_if(entries, [_id](const auto* entry) { return entry->id == _id; });

        if (it != entries.end()) {
            entries.erase(it);
            return true;
        }
        return false;
    };

    if (off_evt(app::EventType::Redraw, id)) {
        return true;
    }
    if (off_evt(app::EventType::MouseClick, id)) {
        return true;
    }
    if (off_evt(app::EventType::MouseMove, id)) {
        return true;
    }
    if (off_evt(app::EventType::KeyPress, id)) {
        return true;
    }

    return false;
}
