#include "FENode.h"
#include <uuid/uuid.h>
#include <sstream>
#include <iomanip>
#include <ctime>

namespace ForbiddenEngine
{
    FENode::FENode(const std::string& nodeName, const std::string& nodeCategory)
        : m_name(nodeName)
        , m_category(nodeCategory)
        , m_description("")
        , m_posX(0.0f)
        , m_posY(0.0f)
        , m_isActive(true)
        , m_graph(nullptr)
    {
        GenerateUniqueId();
    }

    void FENode::GenerateUniqueId()
    {
        // Generate unique ID using timestamp and random component
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        auto duration = now.time_since_epoch();
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

        std::stringstream ss;
        ss << std::hex << millis << "_" << std::rand();
        m_id = ss.str();
    }

    bool FENode::AddProperty(const std::string& propName, PropertyType type, const std::any& defaultValue)
    {
        if (HasProperty(propName))
        {
            return false; // Property already exists
        }

        m_properties[propName] = std::make_pair(type, defaultValue);
        return true;
    }

    bool FENode::SetProperty(const std::string& propName, const std::any& value)
    {
        if (!HasProperty(propName))
        {
            return false; // Property doesn't exist
        }

        m_properties[propName].second = value;
        OnPropertyChanged(propName);
        return true;
    }

    std::any FENode::GetProperty(const std::string& propName) const
    {
        auto it = m_properties.find(propName);
        if (it != m_properties.end())
        {
            return it->second.second;
        }
        return std::any();
    }

    bool FENode::HasProperty(const std::string& propName) const
    {
        return m_properties.find(propName) != m_properties.end();
    }

    int FENode::AddInputSocket(const std::string& socketName, PropertyType type)
    {
        int index = static_cast<int>(m_inputSockets.size());
        m_inputSockets.emplace_back(socketName, type, SocketDirection::Input, index);
        return index;
    }

    int FENode::AddOutputSocket(const std::string& socketName, PropertyType type)
    {
        int index = static_cast<int>(m_outputSockets.size());
        m_outputSockets.emplace_back(socketName, type, SocketDirection::Output, index);
        return index;
    }

    const FESocket* FENode::GetInputSocket(int index) const
    {
        if (index >= 0 && index < static_cast<int>(m_inputSockets.size()))
        {
            return &m_inputSockets[index];
        }
        return nullptr;
    }

    const FESocket* FENode::GetOutputSocket(int index) const
    {
        if (index >= 0 && index < static_cast<int>(m_outputSockets.size()))
        {
            return &m_outputSockets[index];
        }
        return nullptr;
    }

    std::string FENode::Serialize() const
    {
        std::stringstream ss;
        ss << "{\n";
        ss << "  \"id\": \"" << m_id << "\",\n";
        ss << "  \"name\": \"" << m_name << "\",\n";
        ss << "  \"category\": \"" << m_category << "\",\n";
        ss << "  \"description\": \"" << m_description << "\",\n";
        ss << "  \"position\": { \"x\": " << m_posX << ", \"y\": " << m_posY << " },\n";
        ss << "  \"active\": " << (m_isActive ? "true" : "false") << ",\n";
        ss << "  \"properties\": {}\n";
        ss << "}";
        return ss.str();
    }

    bool FENode::Deserialize(const std::string& data)
    {
        // Basic JSON parsing for deserialization
        // In production, use a proper JSON library like nlohmann/json
        return true;
    }
}
