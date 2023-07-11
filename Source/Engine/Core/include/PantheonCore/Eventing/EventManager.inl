#pragma once

#include "PantheonCore/Eventing/EventManager.h"

#include <type_traits>
#include <ranges>

namespace PantheonEngine::Core::Eventing
{
    template <typename EventType>
    IEvent::ListenerId EventManager::subscribe(typename EventType::Action action)
    {
        static_assert(std::is_base_of_v<IEvent, EventType>);

        const size_t typeHash = typeid(EventType).hash_code();

        if (!m_events.contains(typeHash))
            m_events[typeHash] = std::make_unique<EventType>();

        return static_cast<EventType*>(m_events[typeHash].get())->subscribe(action);
    }

    template <typename EventType>
    void EventManager::unsubscribe(const IEvent::ListenerId listener)
    {
        static_assert(std::is_base_of_v<IEvent, EventType>);

        const size_t typeHash = typeid(EventType).hash_code();

        if (!m_events.contains(typeHash))
            return;

        m_events[typeHash]->unsubscribe(listener);
    }

    template <typename EventType, typename... Args>
    void EventManager::broadcast(Args... args)
    {
        static_assert(std::is_base_of_v<IEvent, EventType>);

        const size_t typeHash = typeid(EventType).hash_code();

        if (!m_events.contains(typeHash))
            return;

        static_cast<EventType*>(m_events[typeHash].get())->invoke(args...);
    }

    inline void EventManager::clear()
    {
        for (const auto& event : m_events | std::views::values)
            event->clear();

        m_events.clear();
    }
}
