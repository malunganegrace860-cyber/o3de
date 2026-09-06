#pragma once

#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>

namespace ForbiddenEngine
{
    // Forward declaration
    class FESceneObject;
    class FECamera;

    // Gizmo types
    enum class GizmoType
    {
        Move,
        Rotate,
        Scale,
        Universal
    };

    // Scene object (Entity-like)
    class FESceneObject
    {
    public:
        FESceneObject(const std::string& objectName);
        ~FESceneObject();

        // Identity
        const std::string& GetName() const { return m_name; }
        void SetName(const std::string& name) { m_name = name; }
        const std::string& GetId() const { return m_id; }

        // Transform
        void SetPosition(float x, float y, float z);
        glm::vec3 GetPosition() const { return m_position; }
        
        void SetRotation(float x, float y, float z);
        glm::vec3 GetRotation() const { return m_rotation; }
        
        void SetScale(float x, float y, float z);
        glm::vec3 GetScale() const { return m_scale; }

        // Object properties
        void SetActive(bool active) { m_isActive = active; }
        bool IsActive() const { return m_isActive; }
        
        void SetVisible(bool visible) { m_isVisible = visible; }
        bool IsVisible() const { return m_isVisible; }

        // Hierarchy
        void SetParent(std::shared_ptr<FESceneObject> parent) { m_parent = parent; }
        std::shared_ptr<FESceneObject> GetParent() const { return m_parent; }
        
        bool AddChild(std::shared_ptr<FESceneObject> child);
        bool RemoveChild(const std::string& childId);
        const std::vector<std::shared_ptr<FESceneObject>>& GetChildren() const { return m_children; }

        // Serialization
        std::string Serialize() const;
        bool Deserialize(const std::string& data);

    private:
        std::string m_id;
        std::string m_name;
        glm::vec3 m_position;
        glm::vec3 m_rotation;
        glm::vec3 m_scale;
        bool m_isActive;
        bool m_isVisible;
        std::shared_ptr<FESceneObject> m_parent;
        std::vector<std::shared_ptr<FESceneObject>> m_children;

        void GenerateId();
    };

    // Simple camera
    class FECamera
    {
    public:
        FECamera(const std::string& cameraName);
        ~FECamera();

        const std::string& GetName() const { return m_name; }
        
        void SetPosition(float x, float y, float z) { m_position = glm::vec3(x, y, z); }
        glm::vec3 GetPosition() const { return m_position; }
        
        void SetLookAt(float x, float y, float z) { m_lookAt = glm::vec3(x, y, z); }
        glm::vec3 GetLookAt() const { return m_lookAt; }
        
        void SetFOV(float fov) { m_fov = fov; }
        float GetFOV() const { return m_fov; }
        
        void SetNearPlane(float near) { m_nearPlane = near; }
        float GetNearPlane() const { return m_nearPlane; }
        
        void SetFarPlane(float far) { m_farPlane = far; }
        float GetFarPlane() const { return m_farPlane; }

    private:
        std::string m_name;
        glm::vec3 m_position;
        glm::vec3 m_lookAt;
        float m_fov;
        float m_nearPlane;
        float m_farPlane;
    };

    // Gizmo for manipulation
    class FEGizmo
    {
    public:
        FEGizmo();
        ~FEGizmo();

        void SetType(GizmoType type) { m_type = type; }
        GizmoType GetType() const { return m_type; }

        void SetTarget(std::shared_ptr<FESceneObject> target) { m_targetObject = target; }
        std::shared_ptr<FESceneObject> GetTarget() const { return m_targetObject; }

        void SetActive(bool active) { m_isActive = active; }
        bool IsActive() const { return m_isActive; }

        void SetPosition(float x, float y, float z);
        glm::vec3 GetPosition() const { return m_position; }

        // Gizmo operations
        void ApplyTransform(float deltaX, float deltaY, float deltaZ);

    private:
        GizmoType m_type;
        std::shared_ptr<FESceneObject> m_targetObject;
        glm::vec3 m_position;
        bool m_isActive;
    };

    // Scene editor
    class FESceneEditor
    {
    public:
        FESceneEditor(const std::string& sceneName);
        ~FESceneEditor();

        // Scene identification
        const std::string& GetName() const { return m_sceneName; }
        void SetName(const std::string& name) { m_sceneName = name; }
        const std::string& GetId() const { return m_sceneId; }

        // Initialization
        bool Initialize();
        void Shutdown();

        // Scene management
        bool CreateDefaultScene();
        std::string SerializeScene() const;
        bool DeserializeScene(const std::string& data);

        // Object management
        std::shared_ptr<FESceneObject> CreateObject(const std::string& objectName);
        bool AddObject(std::shared_ptr<FESceneObject> object);
        bool RemoveObject(const std::string& objectId);
        std::shared_ptr<FESceneObject> GetObject(const std::string& objectId) const;
        const std::vector<std::shared_ptr<FESceneObject>>& GetRootObjects() const { return m_rootObjects; }
        int GetObjectCount() const;

        // Selection
        void SetSelectedObject(const std::string& objectId);
        std::shared_ptr<FESceneObject> GetSelectedObject() const { return m_selectedObject; }
        void ClearSelection();

        // Camera
        FECamera& GetCamera() { return m_camera; }
        const FECamera& GetCamera() const { return m_camera; }

        // Gizmo
        FEGizmo& GetGizmo() { return m_gizmo; }
        const FEGizmo& GetGizmo() const { return m_gizmo; }
        void SetGizmoType(GizmoType type);

        // Hierarchy tree
        void PrintHierarchy() const;

    private:
        std::string m_sceneId;
        std::string m_sceneName;
        std::vector<std::shared_ptr<FESceneObject>> m_allObjects;
        std::vector<std::shared_ptr<FESceneObject>> m_rootObjects;
        std::shared_ptr<FESceneObject> m_selectedObject;
        FECamera m_camera;
        FEGizmo m_gizmo;

        void GenerateSceneId();
        std::shared_ptr<FESceneObject> FindObjectRecursive(const std::string& objectId) const;
        void PrintHierarchyRecursive(const std::shared_ptr<FESceneObject>& object, int depth) const;
    };

    typedef std::shared_ptr<FESceneEditor> ScenePtr;
}
