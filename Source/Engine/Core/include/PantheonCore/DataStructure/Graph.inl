#pragma once
#include "PantheonCore/DataStructure/Graph.h"

namespace PantheonCore::DataStructure
{
    template <class NodeT>
    template <typename DataT>
    DataT& Graph<NodeT>::addNode(DataT& node)
    {
        static_assert(std::is_same_v<NodeT, DataT> || std::is_base_of_v<NodeT, DataT>);
        const auto addedNode = std::make_shared<DataT>(node);
        m_nodes.push_back(addedNode);
        return *addedNode;
    }

    template <class NodeT>
    template <typename DataT, typename... Args>
    DataT& Graph<NodeT>::addNode(Args&&... args)
    {
        static_assert(std::is_same_v<NodeT, DataT> || std::is_base_of_v<NodeT, DataT>);
        const auto addedNode = std::make_shared<DataT>(std::forward<Args>(args)...);
        m_nodes.push_back(addedNode);
        return *addedNode;
    }

    template <class NodeT>
    void Graph<NodeT>::removeNode(const NodeT& node)
    {
        const auto nodeIter = std::find(m_nodes.begin(), m_nodes.end(), std::make_shared<NodeT>(&node));

        if (nodeIter != m_nodes.end())
            m_nodes.erase(nodeIter);
    }

    template <class NodeT>
    std::vector<std::shared_ptr<NodeT>> Graph<NodeT>::getNodes()
    {
        return m_nodes;
    }

    template <class NodeT>
    std::vector<std::shared_ptr<const NodeT>> Graph<NodeT>::getNodes() const
    {
        std::vector<std::shared_ptr<const NodeT>> nodes;
        nodes.reserve(m_nodes.size());

        for (const auto& node : m_nodes)
            nodes.push_back(std::dynamic_pointer_cast<const NodeT>(node));

        return nodes;
    }

    template <class NodeT>
    bool Graph<NodeT>::isEmpty() const
    {
        return m_nodes.empty();
    }

    template <class NodeT>
    void Graph<NodeT>::clear()
    {
        if (!m_nodes.empty())
            m_nodes.clear();
    }
}
