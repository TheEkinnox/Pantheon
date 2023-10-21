#pragma once
#include <vector>

namespace PantheonEngine::Core::DataStructure
{
    class Node
    {
    public:
        Node() = default;
        Node(Node* parent);
        Node(const Node& other) = default;
        Node(Node&& other) noexcept = default;
        virtual ~Node();

        Node& operator=(const Node& other) = default;
        Node& operator=(Node&& other) noexcept = default;

        /**
         * \brief Gets the node's parent
         * \return A pointer to the node's parent
         */
        Node* getParent();

        /**
         * \brief Gets the node's parent
         * \return A pointer to the node's parent
         */
        const Node* getParent() const;

        /**
         * \brief Gets the node's children
         * \return The node's children
         */
        std::vector<Node*> getChildren();

        /**
         * \brief Adds the given node as a child of the current node
         * \param child A pointer to the child to add to the current node
         */
        virtual void addChild(Node& child);

        /**
         * \brief Removes the given node from this node's children
         * \param child A pointer to the child to remove from the node's children
         */
        virtual void removeChild(Node& child);

    private:
        Node*              m_parent = nullptr;
        std::vector<Node*> m_children;
    };
}
