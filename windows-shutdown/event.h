#pragma once
#include <functional>
#include <vector>
#include <unordered_map>

#include "consts.app.h"
#include "utils.class.h"

struct ListenerBase {
    NO_COPY_NOR_MOVE(ListenerBase)
    ListenerBase() = default;
    virtual ~ListenerBase() = default;
};

template <typename... Args>
struct ListenerEntry : ListenerBase {
    NO_COPY_NOR_MOVE(ListenerEntry)

   private:
    static unsigned int next_id_;

   public:
    unsigned int id;

    std::function<void(Args...)> listener;
    explicit ListenerEntry(std::function<void(Args...)> _listener)
        : id(next_id_++), listener(std::move(_listener)) {
    }

    ~ListenerEntry() override = default;
};

// struct ListenerEntry {
//     NO_COPY_NOR_MOVE(ListenerEntry)
//
//    private:
//     static unsigned int next_id_;
//
//    public:
//     unsigned int id;
//     std::function<void()> listener;
//
//     explicit ListenerEntry(std::function<void()> _listener);
//     ~ListenerEntry() = default;
// };

// event hub for every element
class Event {
    NO_COPY_NOR_MOVE(Event)

   public:
    Event() = default;

    static Event& get_global() {
        static Event e;
        return e;
    }

   private:
    // event listeners
    std::unordered_map<app::EventType, std::vector<ListenerEntry*>> listeners_map_;

   public:
    ~Event();

   public:
    unsigned int on(const app::EventType evt, std::function<void()> listener);

    void emit(const app::EventType evt);

    bool off(unsigned int id);
};
