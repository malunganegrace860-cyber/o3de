#include "FENodeGraph.h"
#include <algorithm>
#include <queue>
#include <set>
#include <sstream>
#include <ctime>

namespace ForbiddenEngine
{
    FENodeGraph::FENodeGraph(const std::string& graphName)
        : m_name(graphName)
        , m_executionMode(ExecutionMode::Sequential)
        , m_isActive(true)
    {
        GenerateUniqueId();
    }

    FENodeGraph::~FENodeGraph()
    {
        Clear();
    }

    void FENodeGraph::GenerateUniqueId()
    {
        auto now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

        std::stringstream ss;
        ss << std::hex << millis << "_" << std::rand() << "_graph";
        m_id = ss.str();
    }

    NodePtr FENodeGraph::CreateNode(const std::string& nodeName, const std::string& nodeCategory)
    {
        NodePtr node = std::make_shared<FENode>(nodeName, nodeCategory);
        node->SetGraph(this);
        m_nodes.push_back(node);
        return node;
    }

    bool FENodeGraph::AddNode(NodePtr node)
    {
        if (!node)
        {
            return false;
        }

        // Check if node already exists
        for (const auto& existingNode : m_nodes)
        {
            if (existingNode->GetId() == node->GetId())
            {
                return false;
            }
        }

        node->SetGraph(this);
        m_nodes.push_back(node);
        return true;
    }

    bool FENodeGraph::RemoveNode(const std::string& nodeId)
    {
        // Remove all connections involving this node
        m_connections.erase(
            std::remove_if(m_connections.begin(), m_connections.end(),
                [&nodeId](const FEConnection& conn)
                {
                    return conn.sourceNodeId == nodeId || conn.targetNodeId == nodeId;
                }),
            m_connections.end()
        );

        // Remove the node
        auto it = std::find_if(m_nodes.begin(), m_nodes.end(),
            [&nodeId](const NodePtr& node) { return node->GetId() == nodeId; });

        if (it != m_nodes.end())
        {
            m_nodes.erase(it);
            return true;
        }

        return false;
    }

    NodePtr FENodeGraph::GetNode(const std::string& nodeId) const
    {
        for (const auto& node : m_nodes)
        {
            if (node->GetId() == nodeId)
            {
                return node;
            }
        }
        return nullptr;
    }

    bool FENodeGraph::ConnectNodes(const std::string& sourceNodeId, int sourceSocket,
                                   const std::string& targetNodeId, int targetSocket)
    {
        // Validate connection
        if (!ValidateConnection(sourceNodeId, sourceSocket, targetNodeId, targetSocket))
        {
            return false;
        }

        // Check if connection already exists
        if (IsConnected(sourceNodeId, sourceSocket, targetNodeId, targetSocket))
        {
            return false;
        }

        m_connections.emplace_back(sourceNodeId, sourceSocket, targetNodeId, targetSocket);
        return true;
    }

    bool FENodeGraph::DisconnectNodes(const std::string& sourceNodeId, int sourceSocket,
                                      const std::string& targetNodeId, int targetSocket)
    {
        auto it = std::find_if(m_connections.begin(), m_connections.end(),
            [&](const FEConnection& conn)
            {
                return conn.sourceNodeId == sourceNodeId &&
                       conn.sourceSocketIndex == sourceSocket &&
                       conn.targetNodeId == targetNodeId &&
                       conn.targetSocketIndex == targetSocket;
            });

        if (it != m_connections.end())
        {
            m_connections.erase(it);
            return true;
        }

        return false;
    }

    bool FENodeGraph::IsConnected(const std::string& sourceNodeId, int sourceSocket,
                                  const std::string& targetNodeId, int targetSocket) const
    {
        for (const auto& conn : m_connections)
        {
            if (conn.sourceNodeId == sourceNodeId &&
                conn.sourceSocketIndex == sourceSocket &&
                conn.targetNodeId == targetNodeId &&
                conn.targetSocketIndex == targetSocket)
            {
                return true;
            }
        }
        return false;
    }

    bool FENodeGraph::ValidateConnection(const std::string& sourceNodeId, int sourceSocket,
                                        const std::string& targetNodeId, int targetSocket) const
    {
        // Get source and target nodes
        NodePtr sourceNode = GetNode(sourceNodeId);
        NodePtr targetNode = GetNode(targetNodeId);

        if (!sourceNode || !targetNode)
        {
            return false;
        }

        // Can't connect node to itself
        if (sourceNodeId == targetNodeId)
        {
            return false;
        }

        // Validate source socket is an output socket
        const FESocket* srcSocket = sourceNode->GetOutputSocket(sourceSocket);
        if (!srcSocket)
        {
            return false;
        }

        // Validate target socket is an input socket
        const FESocket* tgtSocket = targetNode->GetInputSocket(targetSocket);
        if (!tgtSocket)
        {
            return false;
        }

        // Validate socket types match
        if (!ValidateSocketTypes(srcSocket->type, tgtSocket->type))
        {
            return false;
        }

        return true;
    }

    bool FENodeGraph::ValidateSocketTypes(PropertyType sourceType, PropertyType targetType) const
    {
        // For now, allow direct type matches
        // In future, can add type conversion rules
        return sourceType == targetType;
    }

    bool FENodeGraph::Execute()
    {
        if (!m_isActive || m_nodes.empty())
        {
            return true;
        }

        if (m_executionMode == ExecutionMode::Sequential)
        {
            // Execute nodes in topological order
            std::vector<NodePtr> sortedNodes = TopologicalSort();
            for (const auto& node : sortedNodes)
            {
                if (node->IsActive())
                {
                    if (!node->Execute())
                    {
                        return false;
                    }
                }
            }
        }
        else if (m_executionMode == ExecutionMode::OnDemand)
        {
            // Execute all active nodes (caller responsible for order)
            for (const auto& node : m_nodes)
            {
                if (node->IsActive())
                {
                    if (!node->Execute())
                    {
                        return false;
                    }
                }
            }
        }

        return true;
    }

    std::vector<NodePtr> FENodeGraph::TopologicalSort() const
    {
        std::vector<NodePtr> sorted;
        std::set<std::string> visited;
        std::set<std::string> visiting; // For cycle detection

        std::function<bool(const std::string&)> visit = [&](const std::string& nodeId) -> bool
        {
            if (visited.count(nodeId))
                return true;
            if (visiting.count(nodeId))
                return false; // Cycle detected

            visiting.insert(nodeId);

            // Visit all children
            for (const auto& conn : m_connections)
            {
                if (conn.sourceNodeId == nodeId)
                {
                    if (!visit(conn.targetNodeId))
                    {
                        return false; // Cycle detected
                    }
                }
            }

            visiting.erase(nodeId);
            visited.insert(nodeId);
            NodePtr node = GetNode(nodeId);
            if (node)
            {
                sorted.push_back(node);
            }
            return true;
        };

        // Visit all nodes
        for (const auto& node : m_nodes)
        {
            if (!visited.count(node->GetId()))
            {
                visit(node->GetId());
            }
        }

        return sorted;
    }

    void FENodeGraph::Clear()
    {
        m_nodes.clear();
        m_connections.clear();
    }

    std::string FENodeGraph::Serialize() const
    {
        std::stringstream ss;
        ss << "{\n";
        ss << "  \"id\": \"" << m_id << "\",\n";
        ss << "  \"name\": \"" << m_name << "\",\n";
        ss << "  \"nodeCount\": " << m_nodes.size() << ",\n";
        ss << "  \"connectionCount\": " << m_connections.size() << "\n";
        ss << "}";
        return ss.str();
    }

    bool FENodeGraph::Deserialize(const std::string& data)
    {
        // Basic implementation - use JSON library in production
        return true;
    }

    int FENodeGraph::GetInputNodeCount() const
    {
        int count = 0;
        for (const auto& node : m_nodes)
        {
            if (!node->GetInputSockets().empty() && node->GetOutputSockets().empty())
            {
                count++;
            }
        }
        return count;
    }

    int FENodeGraph::GetOutputNodeCount() const
    {
        int count = 0;
        for (const auto& node : m_nodes)
        {
            if (node->GetOutputSockets().empty() && !node->GetInputSockets().empty())
            {
                count++;
            }
        }
        return count;
    }

    int FENodeGraph::GetProcessingNodeCount() const
    {
        int count = 0;
        for (const auto& node : m_nodes)
        {
            if (!node->GetInputSockets().empty() && !node->GetOutputSockets().empty())
            {
                count++;
            }
        }
        return count;
    }
}
