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

        clearChildren();
    }

    Node* Node::getParent()
    {
        return m_parent;
    }

    const Node* Node::getParent() const
    {
        return m_parent;
    }

    std::vector<Node::NodePtr> Node::getChildren()
    {
        return m_children;
    }

    std::vector<Node::ConstNodePtr> Node::getChildren() const
    {
        std::vector<ConstNodePtr> children;
        children.reserve(m_children.size());

        for (const auto& child : m_children)
            children.push_back(std::dynamic_pointer_cast<const Node>(child));

        return children;
    }

    void Node::removeChild(Node& child)
    {
        const auto findFunc = [child](const NodePtr& ptr)
        {
            return ptr.get() == &child;
        };

        const auto childIter = std::ranges::find_if(m_children, findFunc);

        if (childIter != m_children.end())
        {
            onRemoveChild(**childIter);
            (*childIter)->m_parent = nullptr;
            m_children.erase(childIter);
        }
    }

    void Node::clearChildren()
    {
        for (NodePtr& child : m_children)
        {
            onRemoveChild(*child);
            child->m_parent = nullptr;
        }

        m_children.clear();
    }
}
