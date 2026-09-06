#pragma once

#include "FENode.h"
#include <vector>
#include <map>
#include <memory>
#include <string>

namespace ForbiddenEngine
{
    // Node graph execution mode
    enum class ExecutionMode
    {
        Sequential,      // Nodes execute one after another
        Parallel,        // Nodes execute in parallel where possible
        OnDemand         // Nodes execute when explicitly called
    };

    // FENodeGraph - Manages collection of nodes and their connections
    class FENodeGraph
    {
    public:
        FENodeGraph(const std::string& graphName);
        ~FENodeGraph();

        // Graph identification
        const std::string& GetName() const { return m_name; }
        void SetName(const std::string& name) { m_name = name; }
        const std::string& GetId() const { return m_id; }

        // Node management
        NodePtr CreateNode(const std::string& nodeName, const std::string& nodeCategory);
        bool AddNode(NodePtr node);
        bool RemoveNode(const std::string& nodeId);
        NodePtr GetNode(const std::string& nodeId) const;
        const std::vector<NodePtr>& GetAllNodes() const { return m_nodes; }
        int GetNodeCount() const { return static_cast<int>(m_nodes.size()); }

        // Connection management
        bool ConnectNodes(const std::string& sourceNodeId, int sourceSocket,
                         const std::string& targetNodeId, int targetSocket);
        bool DisconnectNodes(const std::string& sourceNodeId, int sourceSocket,
                            const std::string& targetNodeId, int targetSocket);
        bool IsConnected(const std::string& sourceNodeId, int sourceSocket,
                        const std::string& targetNodeId, int targetSocket) const;
        const std::vector<FEConnection>& GetAllConnections() const { return m_connections; }
        int GetConnectionCount() const { return static_cast<int>(m_connections.size()); }

        // Connection validation
        bool ValidateConnection(const std::string& sourceNodeId, int sourceSocket,
                               const std::string& targetNodeId, int targetSocket) const;

        // Graph execution
        bool Execute();
        void SetExecutionMode(ExecutionMode mode) { m_executionMode = mode; }
        ExecutionMode GetExecutionMode() const { return m_executionMode; }

        // Graph state
        void SetActive(bool active) { m_isActive = active; }
        bool IsActive() const { return m_isActive; }
        void Clear();
        bool IsEmpty() const { return m_nodes.empty(); }

        // Serialization
        std::string Serialize() const;
        bool Deserialize(const std::string& data);

        // Statistics
        int GetInputNodeCount() const;
        int GetOutputNodeCount() const;
        int GetProcessingNodeCount() const;

    private:
        std::string m_id;
        std::string m_name;
        std::vector<NodePtr> m_nodes;
        std::vector<FEConnection> m_connections;
        ExecutionMode m_executionMode;
        bool m_isActive;

        void GenerateUniqueId();
        bool ValidateSocketTypes(PropertyType sourceType, PropertyType targetType) const;
        std::vector<NodePtr> TopologicalSort() const;
    };

    // Typedef for graph shared pointer
    typedef std::shared_ptr<FENodeGraph> GraphPtr;
}
