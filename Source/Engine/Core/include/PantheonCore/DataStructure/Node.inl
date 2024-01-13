#pragma once
#include "PantheonCore/DataStructure/Node.h"

namespace PantheonCore::DataStructure
{
    template <typename T, typename... Args>
    T& Node::addChild(Args&&... args)
    {
        static_assert(std::is_base_of_v<Node, T> || std::is_same_v<Node, T>);

        std::shared_ptr<T> newChild = std::make_shared<T>(std::forward<Args>(args)...);
        newChild->Node::m_parent = this;

        m_children.push_back(newChild);
        onChildAdded(*newChild);

        return *newChild;
    }

    template <typename T>
    void Node::removeChild()
    {
        static_assert(std::is_base_of_v<Node, T> || std::is_same_v<Node, T>);

        const auto findFunc = [](const NodePtr& ptr)
        {
            return typeid(ptr.get()) == typeid(T*) || dynamic_cast<T*>(ptr.get()) != nullptr;
        };

        const auto childIter = std::find_if(m_children.begin(), m_children.end(), findFunc);

        if (childIter != m_children.end())
        {
            onRemoveChild(**childIter);
            (*childIter)->Node::m_parent = nullptr;
            m_children.erase(childIter);
        }
    }

    template <typename T>
    void Node::removeChildren()
    {
        static_assert(std::is_base_of_v<Node, T> || std::is_same_v<Node, T>);

        const auto findFunc = [](const NodePtr& ptr)
        {
            return typeid(ptr.get()) == typeid(T*) || dynamic_cast<T*>(ptr.get()) != nullptr;
        };

        const auto childIter = std::remove_if(m_children.begin(), m_children.end(), findFunc);

        if (childIter != m_children.end())
        {
            for (auto child = childIter; child != m_children.end(); ++child)
            {
                onRemoveChild(**child);
                (*child)->Node::m_parent = nullptr;
            }

            m_children.erase(childIter, m_children.end());
        }
    }
}
