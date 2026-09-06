#include "FESceneEditor.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <random>

namespace ForbiddenEngine
{
    // ===== FESceneObject Implementation =====
    FESceneObject::FESceneObject(const std::string& objectName)
        : m_name(objectName)
        , m_position(0.0f, 0.0f, 0.0f)
        , m_rotation(0.0f, 0.0f, 0.0f)
        , m_scale(1.0f, 1.0f, 1.0f)
        , m_isActive(true)
        , m_isVisible(true)
        , m_parent(nullptr)
    {
        GenerateId();
    }

    FESceneObject::~FESceneObject()
    {
        m_children.clear();
        m_parent = nullptr;
    }

    void FESceneObject::GenerateId()
    {
        auto now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 999999);

        std::stringstream ss;
        ss << std::hex << millis << "_" << dis(gen) << "_obj";
        m_id = ss.str();
    }

    void FESceneObject::SetPosition(float x, float y, float z)
    {
        m_position = glm::vec3(x, y, z);
    }

    void FESceneObject::SetRotation(float x, float y, float z)
    {
        m_rotation = glm::vec3(x, y, z);
    }

    void FESceneObject::SetScale(float x, float y, float z)
    {
        m_scale = glm::vec3(x, y, z);
    }

    bool FESceneObject::AddChild(std::shared_ptr<FESceneObject> child)
    {
        if (!child)
            return false;

        child->SetParent(std::make_shared<FESceneObject>(*this));
        m_children.push_back(child);
        return true;
    }

    bool FESceneObject::RemoveChild(const std::string& childId)
    {
        auto it = std::find_if(m_children.begin(), m_children.end(),
            [&childId](const std::shared_ptr<FESceneObject>& child) { return child->GetId() == childId; });

        if (it != m_children.end())
        {
            m_children.erase(it);
            return true;
        }

        return false;
    }

    std::string FESceneObject::Serialize() const
    {
        std::stringstream ss;
        ss << "{\n";
        ss << "  \"id\": \"" << m_id << "\",\n";
        ss << "  \"name\": \"" << m_name << "\",\n";
        ss << "  \"position\": { \"x\": " << m_position.x << ", \"y\": " << m_position.y << ", \"z\": " << m_position.z << " },\n";
        ss << "  \"rotation\": { \"x\": " << m_rotation.x << ", \"y\": " << m_rotation.y << ", \"z\": " << m_rotation.z << " },\n";
        ss << "  \"scale\": { \"x\": " << m_scale.x << ", \"y\": " << m_scale.y << ", \"z\": " << m_scale.z << " },\n";
        ss << "  \"active\": " << (m_isActive ? "true" : "false") << ",\n";
        ss << "  \"visible\": " << (m_isVisible ? "true" : "false") << "\n";
        ss << "}";
        return ss.str();
    }

    bool FESceneObject::Deserialize(const std::string& data)
    {
        // Basic JSON parsing (use proper JSON library in production)
        return true;
    }

    // ===== FECamera Implementation =====
    FECamera::FECamera(const std::string& cameraName)
        : m_name(cameraName)
        , m_position(0.0f, 5.0f, 10.0f)
        , m_lookAt(0.0f, 0.0f, 0.0f)
        , m_fov(45.0f)
        , m_nearPlane(0.1f)
        , m_farPlane(1000.0f)
    {
    }

    FECamera::~FECamera()
    {
    }

    // ===== FEGizmo Implementation =====
    FEGizmo::FEGizmo()
        : m_type(GizmoType::Move)
        , m_position(0.0f, 0.0f, 0.0f)
        , m_isActive(false)
    {
    }

    FEGizmo::~FEGizmo()
    {
    }

    void FEGizmo::SetPosition(float x, float y, float z)
    {
        m_position = glm::vec3(x, y, z);
    }

    void FEGizmo::ApplyTransform(float deltaX, float deltaY, float deltaZ)
    {
        if (!m_targetObject || !m_isActive)
            return;

        switch (m_type)
        {
        case GizmoType::Move:
        {
            glm::vec3 currentPos = m_targetObject->GetPosition();
            m_targetObject->SetPosition(
                currentPos.x + deltaX,
                currentPos.y + deltaY,
                currentPos.z + deltaZ
            );
            break;
        }
        case GizmoType::Rotate:
        {
            glm::vec3 currentRot = m_targetObject->GetRotation();
            m_targetObject->SetRotation(
                currentRot.x + deltaX,
                currentRot.y + deltaY,
                currentRot.z + deltaZ
            );
            break;
        }
        case GizmoType::Scale:
        {
            glm::vec3 currentScale = m_targetObject->GetScale();
            m_targetObject->SetScale(
                currentScale.x + deltaX,
                currentScale.y + deltaY,
                currentScale.z + deltaZ
            );
            break;
        }
        default:
            break;
        }
    }

    // ===== FESceneEditor Implementation =====
    FESceneEditor::FESceneEditor(const std::string& sceneName)
        : m_sceneName(sceneName)
        , m_camera("DefaultCamera")
        , m_selectedObject(nullptr)
    {
        GenerateSceneId();
    }

    FESceneEditor::~FESceneEditor()
    {
        Shutdown();
    }

    void FESceneEditor::GenerateSceneId()
    {
        auto now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

        std::stringstream ss;
        ss << std::hex << millis << "_scene";
        m_sceneId = ss.str();
    }

    bool FESceneEditor::Initialize()
    {
        return CreateDefaultScene();
    }

    void FESceneEditor::Shutdown()
    {
        m_allObjects.clear();
        m_rootObjects.clear();
        m_selectedObject = nullptr;
    }

    bool FESceneEditor::CreateDefaultScene()
    {
        // Clear existing scene
        m_allObjects.clear();
        m_rootObjects.clear();
        m_selectedObject = nullptr;

        // Create root objects
        // 1. Create Daylight object (Sun + Sky)
        auto daylightObject = std::make_shared<FESceneObject>("Daylight");
        daylightObject->SetPosition(0.0f, 10.0f, 10.0f);
        AddObject(daylightObject);

        // 2. Create Grid Floor
        auto gridFloor = std::make_shared<FESceneObject>("GridFloor");
        gridFloor->SetPosition(0.0f, 0.0f, 0.0f);
        gridFloor->SetScale(10.0f, 0.1f, 10.0f);
        AddObject(gridFloor);

        // 3. Create White Box at fixed position (0, 1, 0)
        auto whiteBox = std::make_shared<FESceneObject>("WhiteBox");
        whiteBox->SetPosition(0.0f, 1.0f, 0.0f);
        whiteBox->SetScale(1.0f, 1.0f, 1.0f);
        AddObject(whiteBox);
        SetSelectedObject(whiteBox->GetId());

        // 4. Setup camera
        m_camera.SetPosition(0.0f, 5.0f, 10.0f);
        m_camera.SetLookAt(0.0f, 1.0f, 0.0f);
        m_camera.SetFOV(45.0f);

        // 5. Setup gizmo
        m_gizmo.SetTarget(whiteBox);
        m_gizmo.SetActive(false);
        m_gizmo.SetType(GizmoType::Move);

        return true;
    }

    std::shared_ptr<FESceneObject> FESceneEditor::CreateObject(const std::string& objectName)
    {
        return std::make_shared<FESceneObject>(objectName);
    }

    bool FESceneEditor::AddObject(std::shared_ptr<FESceneObject> object)
    {
        if (!object)
            return false;

        // Add to all objects
        m_allObjects.push_back(object);

        // Add to root objects if no parent
        if (!object->GetParent())
        {
            m_rootObjects.push_back(object);
        }

        return true;
    }

    bool FESceneEditor::RemoveObject(const std::string& objectId)
    {
        // Remove from all objects
        auto allIt = std::find_if(m_allObjects.begin(), m_allObjects.end(),
            [&objectId](const std::shared_ptr<FESceneObject>& obj) { return obj->GetId() == objectId; });

        if (allIt == m_allObjects.end())
            return false;

        // Remove from root objects if applicable
        auto rootIt = std::find_if(m_rootObjects.begin(), m_rootObjects.end(),
            [&objectId](const std::shared_ptr<FESceneObject>& obj) { return obj->GetId() == objectId; });

        if (rootIt != m_rootObjects.end())
        {
            m_rootObjects.erase(rootIt);
        }

        // Clear selection if removing selected object
        if (m_selectedObject && m_selectedObject->GetId() == objectId)
        {
            m_selectedObject = nullptr;
        }

        m_allObjects.erase(allIt);
        return true;
    }

    std::shared_ptr<FESceneObject> FESceneEditor::GetObject(const std::string& objectId) const
    {
        for (const auto& obj : m_allObjects)
        {
            if (obj->GetId() == objectId)
                return obj;
        }
        return nullptr;
    }

    void FESceneEditor::SetSelectedObject(const std::string& objectId)
    {
        auto obj = GetObject(objectId);
        if (obj)
        {
            m_selectedObject = obj;
            m_gizmo.SetTarget(obj);
        }
    }

    void FESceneEditor::ClearSelection()
    {
        m_selectedObject = nullptr;
        m_gizmo.SetTarget(nullptr);
    }

    void FESceneEditor::SetGizmoType(GizmoType type)
    {
        m_gizmo.SetType(type);
    }

    int FESceneEditor::GetObjectCount() const
    {
        return static_cast<int>(m_allObjects.size());
    }

    std::string FESceneEditor::SerializeScene() const
    {
        std::stringstream ss;
        ss << "{\n";
        ss << "  \"id\": \"" << m_sceneId << "\",\n";
        ss << "  \"name\": \"" << m_sceneName << "\",\n";
        ss << "  \"objectCount\": " << GetObjectCount() << "\n";
        ss << "}";
        return ss.str();
    }

    bool FESceneEditor::DeserializeScene(const std::string& data)
    {
        // Basic JSON parsing (use proper JSON library in production)
        return true;
    }

    void FESceneEditor::PrintHierarchy() const
    {
        std::cout << "\n=== SCENE HIERARCHY: " << m_sceneName << " ===" << std::endl;
        std::cout << "Total Objects: " << GetObjectCount() << std::endl;
        std::cout << "\nScene Tree:" << std::endl;

        for (const auto& rootObj : m_rootObjects)
        {
            PrintHierarchyRecursive(rootObj, 0);
        }

        if (m_selectedObject)
        {
            std::cout << "\nSelected: " << m_selectedObject->GetName() << std::endl;
        }

        std::cout << "\n========================================\n" << std::endl;
    }

    void FESceneEditor::PrintHierarchyRecursive(const std::shared_ptr<FESceneObject>& object, int depth) const
    {
        std::string indent(depth * 2, ' ');
        std::cout << indent << "[" << (object->IsActive() ? "A" : " ") << "] ";
        std::cout << object->GetName();
        
        if (object->IsVisible())
            std::cout << " (V)";
        else
            std::cout << " (H)";
        
        std::cout << std::endl;

        for (const auto& child : object->GetChildren())
        {
            PrintHierarchyRecursive(child, depth + 1);
        }
    }
}
