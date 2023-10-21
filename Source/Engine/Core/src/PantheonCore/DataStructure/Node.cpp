#include "PantheonCore/DataStructure/Graph.h"

namespace PantheonEngine::Core::DataStructure
{
    Node::Node(Node* parent)
        : m_parent(parent)
    {
    }

    Node::~Node()
    {
        if (m_parent != nullptr)
            m_parent->removeChild(*this);

        for (Node* child : m_children)
            child->m_parent = nullptr;
    }

    Node* Node::getParent()
    {
        return m_parent;
    }

    const Node* Node::getParent() const
    {
        return m_parent;
    }

    std::vector<Node*> Node::getChildren()
    {
        return m_children;
    }

    void Node::addChild(Node& child)
    {
        for (const Node* childNode : m_children)
        {
            if (childNode == &child)
                return;
        }

        m_children.push_back(&child);
        child.m_parent = this;
    }

    void Node::removeChild(Node& child)
    {
        const auto childIter = std::find(m_children.begin(), m_children.end(), &child);

        if (childIter != m_children.end())
        {
            m_children.erase(childIter);
            child.m_parent = nullptr;
        }
    }
}
