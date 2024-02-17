#pragma once

#include "PantheonCore/Eventing/Event.h"

#include <ranges>

namespace PantheonCore::Eventing
{
    template <class... ArgTypes>
    IEvent::ListenerId Event<ArgTypes...>::subscribe(Action action)
    {
        m_actions[m_currentId] = action;
        return m_currentId++;
    }

    template <class... ArgTypes>
    void Event<ArgTypes...>::unsubscribe(ListenerId listener)
    {
        const auto it = m_actions.find(listener);
        if (it != m_actions.end())
            m_actions.erase(it);
    }

    template <class... ArgTypes>
    size_t Event<ArgTypes...>::subscribersCount() const
    {
        return m_actions.size();
    }

    template <class... ArgTypes>
    void Event<ArgTypes...>::invoke(ArgTypes... args) const
    {
        for (auto& action : m_actions | std::views::values)
            action(args...);
    }

    template <class... ArgTypes>
    void Event<ArgTypes...>::clear()
    {
        m_actions.clear();
    }
}
