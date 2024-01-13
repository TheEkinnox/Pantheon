#pragma once

#include <memory>

#include "PantheonCore/Eventing/Event.h"

namespace PantheonCore::Eventing
{
    class EventManager
    {
    public:
        using EventPtr = std::unique_ptr<IEvent>;
        using EventMap = std::unordered_map<size_t, EventPtr>;

        /**
         * \brief Subscribes the given action to the provided event type
         * \tparam EventType The type of the event to subscribe to
         * \param action The action to perform when the event is triggered
         * \return The subscribed event's listener id on success, 0 otherwise.
         */
        template <typename EventType>
        IEvent::ListenerId subscribe(typename EventType::Action action);

        /**
         * \brief Unsubscribes the listener with the given id from the event with the given type
         * \tparam EventType The type of the event to unsubscribe from
         * \param listener The id of the listener to unsubscribe from the given event type
         */
        template <typename EventType>
        void unsubscribe(IEvent::ListenerId listener);

        /**
         * \brief Invokes all actions linked to events of the given type subscribed to this event manager
         * \tparam EventType The type of the event to broadcast
         * \tparam Args The arguments to pass to the event's actions
         * \param args The arguments to pass to the event's actions
         */
        template <typename EventType, typename... Args>
        void broadcast(Args... args);

        /**
         * \brief Unsubscribes all the events subscribed to this event manager
         */
        void clear();

    private:
        EventMap m_events;
    };
}

#include "PantheonCore/Eventing/EventManager.inl"
