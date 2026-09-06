#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <any>
#include <functional>

namespace ForbiddenEngine
{
    // Forward declarations
    class FENodeGraph;
    class FENode;

    // Node property types
    enum class PropertyType
    {
        Bool,
        Int,
        Float,
        String,
        Vector3,
        Vector4,
        Matrix4,
        Object,
        Custom
    };

    // Socket direction
    enum class SocketDirection
    {
        Input,
        Output
    };

    // Socket definition
    struct FESocket
    {
        std::string name;
        PropertyType type;
        SocketDirection direction;
        std::string description;
        int index;

        FESocket(const std::string& socketName, PropertyType socketType, SocketDirection socketDir, int socketIndex)
            : name(socketName), type(socketType), direction(socketDir), index(socketIndex), description("")
        {
        }
    };

    // Node connection
    struct FEConnection
    {
        std::string sourceNodeId;
        int sourceSocketIndex;
        std::string targetNodeId;
        int targetSocketIndex;

        FEConnection(const std::string& srcNode, int srcSocket, const std::string& tgtNode, int tgtSocket)
            : sourceNodeId(srcNode), sourceSocketIndex(srcSocket), targetNodeId(tgtNode), targetSocketIndex(tgtSocket)
        {
        }
    };

    // Base FENode class
    class FENode
    {
    public:
        FENode(const std::string& nodeName, const std::string& nodeCategory);
        virtual ~FENode() = default;

        // Node identification
        const std::string& GetId() const { return m_id; }
        const std::string& GetName() const { return m_name; }
        void SetName(const std::string& name) { m_name = name; }
        const std::string& GetCategory() const { return m_category; }
        const std::string& GetDescription() const { return m_description; }
        void SetDescription(const std::string& desc) { m_description = desc; }

        // Position/Visualization
        void SetPosition(float x, float y) { m_posX = x; m_posY = y; }
        float GetPositionX() const { return m_posX; }
        float GetPositionY() const { return m_posY; }

        // Node state
        void SetActive(bool active) { m_isActive = active; }
        bool IsActive() const { return m_isActive; }

        // Properties management
        bool AddProperty(const std::string& propName, PropertyType type, const std::any& defaultValue);
        bool SetProperty(const std::string& propName, const std::any& value);
        std::any GetProperty(const std::string& propName) const;
        bool HasProperty(const std::string& propName) const;
        const std::map<std::string, std::pair<PropertyType, std::any>>& GetAllProperties() const { return m_properties; }

        // Socket management
        int AddInputSocket(const std::string& socketName, PropertyType type);
        int AddOutputSocket(const std::string& socketName, PropertyType type);
        const std::vector<FESocket>& GetInputSockets() const { return m_inputSockets; }
        const std::vector<FESocket>& GetOutputSockets() const { return m_outputSockets; }
        const FESocket* GetInputSocket(int index) const;
        const FESocket* GetOutputSocket(int index) const;

        // Execution
        virtual bool Execute() { return true; }
        virtual void OnPropertyChanged(const std::string& propName) { }

        // Serialization
        virtual std::string Serialize() const;
        virtual bool Deserialize(const std::string& data);

        // Graph connection (internal use)
        void SetGraph(FENodeGraph* graph) { m_graph = graph; }
        FENodeGraph* GetGraph() const { return m_graph; }

    protected:
        std::string m_id;
        std::string m_name;
        std::string m_category;
        std::string m_description;
        float m_posX;
        float m_posY;
        bool m_isActive;

        std::map<std::string, std::pair<PropertyType, std::any>> m_properties;
        std::vector<FESocket> m_inputSockets;
        std::vector<FESocket> m_outputSockets;

        FENodeGraph* m_graph;

    private:
        void GenerateUniqueId();
    };

    // Typedef for node shared pointer
    typedef std::shared_ptr<FENode> NodePtr;
}
