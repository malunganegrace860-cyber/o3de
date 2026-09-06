#include "FEWorkspace.h"
#include <iostream>
#include <fstream>
#include <algorithm>

namespace ForbiddenEngine
{
    FEWorkspace::FEWorkspace(float screenWidth, float screenHeight)
        : m_screenWidth(screenWidth)
        , m_screenHeight(screenHeight)
        , m_currentLayoutName("default")
        , m_isActive(false)
        , m_layoutDirty(true)
    {
    }

    FEWorkspace::~FEWorkspace()
    {
        Shutdown();
    }

    void FEWorkspace::Initialize()
    {
        // Create default panels
        AddPanel("Scene Viewport", PanelType::Scene);
        AddPanel("Hierarchy", PanelType::Hierarchy);
        AddPanel("Inspector", PanelType::Inspector);
        AddPanel("Assets", PanelType::Assets);
        AddPanel("Console", PanelType::Console);

        // Calculate default layout
        CalculateDefaultLayout();

        m_isActive = true;
    }

    void FEWorkspace::Shutdown()
    {
        m_panels.clear();
        m_isActive = false;
    }

    void FEWorkspace::SetScreenSize(float width, float height)
    {
        m_screenWidth = width;
        m_screenHeight = height;
        m_layoutDirty = true;
    }

    bool FEWorkspace::AddPanel(const std::string& panelName, PanelType type)
    {
        // Check if panel already exists
        for (const auto& panel : m_panels)
        {
            if (panel.name == panelName)
            {
                return false;
            }
        }

        m_panels.emplace_back(panelName, type);
        m_layoutDirty = true;
        return true;
    }

    bool FEWorkspace::RemovePanel(const std::string& panelName)
    {
        auto it = std::find_if(m_panels.begin(), m_panels.end(),
            [&panelName](const PanelState& panel) { return panel.name == panelName; });

        if (it != m_panels.end())
        {
            m_panels.erase(it);
            m_layoutDirty = true;
            return true;
        }

        return false;
    }

    PanelState* FEWorkspace::GetPanel(const std::string& panelName)
    {
        for (auto& panel : m_panels)
        {
            if (panel.name == panelName)
            {
                return &panel;
            }
        }
        return nullptr;
    }

    void FEWorkspace::SetPanelVisible(const std::string& panelName, bool visible)
    {
        PanelState* panel = GetPanel(panelName);
        if (panel)
        {
            panel->isVisible = visible;
        }
    }

    bool FEWorkspace::IsPanelVisible(const std::string& panelName) const
    {
        for (const auto& panel : m_panels)
        {
            if (panel.name == panelName)
            {
                return panel.isVisible;
            }
        }
        return false;
    }

    void FEWorkspace::SetPanelDocked(const std::string& panelName, bool docked, int position)
    {
        PanelState* panel = GetPanel(panelName);
        if (panel)
        {
            panel->isDocked = docked;
            panel->dockPosition = position;
            m_layoutDirty = true;
        }
    }

    bool FEWorkspace::IsPanelDocked(const std::string& panelName) const
    {
        for (const auto& panel : m_panels)
        {
            if (panel.name == panelName)
            {
                return panel.isDocked;
            }
        }
        return false;
    }

    void FEWorkspace::SetPanelSize(const std::string& panelName, float width, float height)
    {
        PanelState* panel = GetPanel(panelName);
        if (panel)
        {
            panel->width = width;
            panel->height = height;
        }
    }

    void FEWorkspace::SetPanelPosition(const std::string& panelName, float x, float y)
    {
        PanelState* panel = GetPanel(panelName);
        if (panel)
        {
            panel->posX = x;
            panel->posY = y;
        }
    }

    void FEWorkspace::CalculateDefaultLayout()
    {
        // Default layout: 4-panel configuration
        // Left: Hierarchy (20% width)
        // Center: Scene Viewport (60% width)
        // Right: Inspector (20% width)
        // Bottom: Console (100% width, 20% height)

        float leftPanelWidth = m_screenWidth * 0.20f;
        float centerPanelWidth = m_screenWidth * 0.60f;
        float rightPanelWidth = m_screenWidth * 0.20f;
        float topHeight = m_screenHeight * 0.80f;
        float bottomHeight = m_screenHeight * 0.20f;

        for (auto& panel : m_panels)
        {
            panel.isVisible = true;
            panel.isDocked = true;

            if (panel.name == "Hierarchy")
            {
                panel.width = leftPanelWidth;
                panel.height = topHeight;
                panel.posX = 0.0f;
                panel.posY = 0.0f;
                panel.dockPosition = 0; // Left
            }
            else if (panel.name == "Scene Viewport")
            {
                panel.width = centerPanelWidth;
                panel.height = topHeight;
                panel.posX = leftPanelWidth;
                panel.posY = 0.0f;
                panel.dockPosition = 1; // Center
            }
            else if (panel.name == "Inspector")
            {
                panel.width = rightPanelWidth;
                panel.height = topHeight;
                panel.posX = leftPanelWidth + centerPanelWidth;
                panel.posY = 0.0f;
                panel.dockPosition = 2; // Right
            }
            else if (panel.name == "Console")
            {
                panel.width = m_screenWidth;
                panel.height = bottomHeight;
                panel.posX = 0.0f;
                panel.posY = topHeight;
                panel.dockPosition = 3; // Bottom
            }
            else if (panel.name == "Assets")
            {
                panel.isVisible = false; // Hidden by default
                panel.width = rightPanelWidth;
                panel.height = topHeight;
                panel.posX = leftPanelWidth + centerPanelWidth;
                panel.posY = 0.0f;
            }
        }

        m_layoutDirty = false;
    }

    void FEWorkspace::ArrangePanels()
    {
        if (!m_layoutDirty)
            return;

        // Recalculate panel positions based on current screen size
        for (auto& panel : m_panels)
        {
            if (!panel.isDocked)
                continue;

            // Adjust positions based on screen size
            if (panel.dockPosition == 3) // Bottom panel
            {
                panel.width = m_screenWidth;
                panel.posX = 0.0f;
            }
        }

        m_layoutDirty = false;
    }

    void FEWorkspace::UpdateLayout()
    {
        ArrangePanels();
    }

    void FEWorkspace::Render()
    {
        if (!m_isActive)
            return;

        // This would be implemented with actual rendering backend (Qt, ImGui, etc.)
        // For now, this is a placeholder
    }

    void FEWorkspace::ResetLayout()
    {
        m_layoutDirty = true;
        CalculateDefaultLayout();
    }

    void FEWorkspace::SaveLayout(const std::string& layoutPath)
    {
        try
        {
            std::ofstream layoutFile(layoutPath);
            if (!layoutFile.is_open())
            {
                return;
            }

            layoutFile << "{\n";
            layoutFile << "  \"name\": \"" << m_currentLayoutName << "\",\n";
            layoutFile << "  \"screenWidth\": " << m_screenWidth << ",\n";
            layoutFile << "  \"screenHeight\": " << m_screenHeight << ",\n";
            layoutFile << "  \"panels\": [\n";

            for (size_t i = 0; i < m_panels.size(); ++i)
            {
                const auto& panel = m_panels[i];
                layoutFile << "    {\n";
                layoutFile << "      \"name\": \"" << panel.name << "\",\n";
                layoutFile << "      \"visible\": " << (panel.isVisible ? "true" : "false") << ",\n";
                layoutFile << "      \"docked\": " << (panel.isDocked ? "true" : "false") << ",\n";
                layoutFile << "      \"width\": " << panel.width << ",\n";
                layoutFile << "      \"height\": " << panel.height << ",\n";
                layoutFile << "      \"posX\": " << panel.posX << ",\n";
                layoutFile << "      \"posY\": " << panel.posY << "\n";
                layoutFile << "    }" << (i < m_panels.size() - 1 ? "," : "") << "\n";
            }

            layoutFile << "  ]\n";
            layoutFile << "}";
            layoutFile.close();
        }
        catch (...) { /* Ignore save errors */ }
    }

    void FEWorkspace::LoadLayout(const std::string& layoutPath)
    {
        try
        {
            std::ifstream layoutFile(layoutPath);
            if (!layoutFile.is_open())
            {
                return;
            }

            // Basic JSON parsing for layout (use proper JSON library in production)
            layoutFile.close();
        }
        catch (...) { /* Ignore load errors */ }
    }

    void FEWorkspace::PrintLayout() const
    {
        std::cout << "\n=== FORBIDDEN ENGINE WORKSPACE LAYOUT ===" << std::endl;
        std::cout << "Screen: " << m_screenWidth << "x" << m_screenHeight << std::endl;
        std::cout << "Panels: " << m_panels.size() << std::endl;
        std::cout << "\nPanel Configuration:\n" << std::endl;

        for (const auto& panel : m_panels)
        {
            std::cout << "  [" << (panel.isVisible ? "V" : " ") << "] ";
            std::cout << panel.name << " (" << panel.width << "x" << panel.height << ")";
            std::cout << " @ (" << panel.posX << ", " << panel.posY << ")";
            std::cout << " [" << (panel.isDocked ? "Docked" : "Float") << "]" << std::endl;
        }

        std::cout << "\n========================================\n" << std::endl;
    }
}
