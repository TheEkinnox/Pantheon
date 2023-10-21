#pragma once
#include <memory>
#include <vector>

#include "PantheonCore/DataStructure/Node.h"

namespace PantheonEngine::Core::DataStructure
{
    template <class NodeT>
    class Graph
    {
        static_assert(std::is_base_of_v<Node, NodeT>);

    public:
        Graph() = default;
        Graph(const Graph& other) = default;
        Graph(Graph&& other) noexcept = default;
        virtual ~Graph() = default;

        Graph& operator=(const Graph& other) = default;
        Graph& operator=(Graph&& other) noexcept = default;

        /**
         * \brief Adds the given node to the graph
         * \param node The node to add to the graph
         * \return A reference to the added node
         */
        template <typename DataT>
        DataT& addNode(DataT& node);

        /**
         * \brief Adds the given node to the graph
         * \param args The arguments to pass to the created node's constructor
         * \return A reference to the added node
         */
        template <typename DataT, typename... Args>
        DataT& addNode(Args&&... args);

        /**
         * \brief Removes the given node from the graph
         * \param node The node to remove from the graph
         */
        void removeNode(const NodeT& node);

        /**
         * \brief Gets the graph's root nodes list
         * \return The graph's root nodes list
         */
        std::vector<std::shared_ptr<NodeT>> getNodes();

        /**
         * \brief Checks whether or not the graph is empty
         * \return True if the graph is empty. False otherwise
         */
        bool isEmpty() const;

        /**
         * \brief Removes all nodes from the graph
         */
        void clear();

    private:
        std::vector<std::shared_ptr<NodeT>> m_nodes;
    };
}

#include "PantheonCore/DataStructure/Graph.inl"
