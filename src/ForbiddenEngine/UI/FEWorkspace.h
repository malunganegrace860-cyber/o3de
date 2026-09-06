#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>

namespace ForbiddenEngine
{
    // Panel types
    enum class PanelType
    {
        Scene,
        Hierarchy,
        Inspector,
        Assets,
        Console,
        Properties,
        Custom
    };

    // Panel state
    struct PanelState
    {
        std::string name;
        PanelType type;
        float width;
        float height;
        float posX;
        float posY;
        bool isVisible;
        bool isDocked;
        int dockPosition; // 0=Left, 1=Center, 2=Right, 3=Bottom

        PanelState(const std::string& panelName, PanelType panelType)
            : name(panelName), type(panelType), width(300.0f), height(400.0f),
              posX(0.0f), posY(0.0f), isVisible(true), isDocked(true), dockPosition(0)
        {
        }
    };

    // Workspace layout
    class FEWorkspace
    {
    public:
        FEWorkspace(float screenWidth, float screenHeight);
        ~FEWorkspace();

        // Initialization
        void Initialize();
        void Shutdown();

        // Screen/Window management
        void SetScreenSize(float width, float height);
        float GetScreenWidth() const { return m_screenWidth; }
        float GetScreenHeight() const { return m_screenHeight; }

        // Panel management
        bool AddPanel(const std::string& panelName, PanelType type);
        bool RemovePanel(const std::string& panelName);
        PanelState* GetPanel(const std::string& panelName);
        const std::vector<PanelState>& GetAllPanels() const { return m_panels; }
        int GetPanelCount() const { return static_cast<int>(m_panels.size()); }

        // Panel visibility/docking
        void SetPanelVisible(const std::string& panelName, bool visible);
        bool IsPanelVisible(const std::string& panelName) const;
        void SetPanelDocked(const std::string& panelName, bool docked, int position = 0);
        bool IsPanelDocked(const std::string& panelName) const;

        // Panel sizing
        void SetPanelSize(const std::string& panelName, float width, float height);
        void SetPanelPosition(const std::string& panelName, float x, float y);

        // Layout management
        void ResetLayout();
        void SaveLayout(const std::string& layoutPath);
        void LoadLayout(const std::string& layoutPath);
        std::string GetCurrentLayoutName() const { return m_currentLayoutName; }

        // Workspace rendering
        void UpdateLayout();
        void Render();

        // Workspace state
        void SetActive(bool active) { m_isActive = active; }
        bool IsActive() const { return m_isActive; }

        // Workspace information
        void PrintLayout() const;

    private:
        float m_screenWidth;
        float m_screenHeight;
        std::vector<PanelState> m_panels;
        std::string m_currentLayoutName;
        bool m_isActive;
        bool m_layoutDirty;

        void CalculateDefaultLayout();
        void ArrangePanels();
    };

    typedef std::shared_ptr<FEWorkspace> WorkspacePtr;
}
