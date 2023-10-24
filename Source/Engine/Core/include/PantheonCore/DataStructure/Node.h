#pragma once
#include <vector>

namespace PantheonEngine::Core::DataStructure
{
    class Node
    {
    public:
        using NodePtr = std::shared_ptr<Node>;
        using ConstNodePtr = std::shared_ptr<const Node>;

        Node() = default;
        explicit Node(Node* parent);
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
        std::vector<NodePtr> getChildren();

        /**
         * \brief Gets the node's children
         * \return The node's children
         */
        std::vector<ConstNodePtr> getChildren() const;

        /**
         * \brief Removes all the node's children
         */
        void clearChildren();

        /**
         * \brief Adds a child of the given type to the node
         * \tparam T The added child's type
         * \tparam Args The added child's constructor arguments' types
         * \param args The added child's constructor arguments
         * \return The added child
         */
        template <typename T, typename... Args>
        T& addChild(Args&&... args);

        /**
         * \brief Removes the first child of the given type from the node
         * \tparam T The removed child's type
         */
        template <typename T>
        void removeChild();

        /**
         * \brief Removes all the children of the given type from the node
         * \tparam T The removed children's type
         */
        template <typename T>
        void removeChildren();

        /**
         * \brief Removes the given node from this node's children
         * \param child A pointer to the child to remove from the node's children
         */
        void removeChild(Node& child);

    protected:
        /**
         * \brief The action to perform after a child was added
         * \param child The added child
         */
        virtual void onChildAdded([[maybe_unused]] Node& child)
        {
        }

        /**
         * \brief The action to perform before a child is removed
         * \param child The child to remove
         */
        virtual void onRemoveChild([[maybe_unused]] Node& child)
        {
        }

    private:
        Node*                m_parent = nullptr;
        std::vector<NodePtr> m_children;
    };
}

#include "PantheonCore/DataStructure/Node.inl"
