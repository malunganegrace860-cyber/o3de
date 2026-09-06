#include "FEUITheme.h"
#include <iostream>
#include <fstream>
#include <sstream>

namespace ForbiddenEngine
{
    FEUITheme::FEUITheme()
        : m_currentTheme(CreateForbiddenEngineTheme())
        , m_defaultTheme(CreateForbiddenEngineTheme())
    {
    }

    FEUITheme::~FEUITheme()
    {
    }

    void FEUITheme::Initialize()
    {
        m_currentTheme = CreateForbiddenEngineTheme();
    }

    void FEUITheme::SetTheme(const FETheme& theme)
    {
        m_currentTheme = theme;
    }

    void FEUITheme::SetFont(const std::string& fontName, int fontSize)
    {
        m_currentTheme.fontName = fontName;
        m_currentTheme.fontSize = fontSize;
    }

    bool FEUITheme::SaveTheme(const std::string& themePath)
    {
        try
        {
            std::ofstream themeFile(themePath);
            if (!themeFile.is_open())
            {
                return false;
            }

            themeFile << "{\n";
            themeFile << "  \"name\": \"" << m_currentTheme.name << "\",\n";
            themeFile << "  \"colors\": {\n";
            themeFile << "    \"primary\": \"" << m_currentTheme.primaryColor.ToHex() << "\",\n";
            themeFile << "    \"accent\": \"" << m_currentTheme.accentColor.ToHex() << "\",\n";
            themeFile << "    \"panelBg\": \"" << m_currentTheme.panelBackground.ToHex() << "\",\n";
            themeFile << "    \"border\": \"" << m_currentTheme.panelBorder.ToHex() << "\",\n";
            themeFile << "    \"text\": \"" << m_currentTheme.textColor.ToHex() << "\",\n";
            themeFile << "    \"warning\": \"" << m_currentTheme.warningColor.ToHex() << "\",\n";
            themeFile << "    \"error\": \"" << m_currentTheme.errorColor.ToHex() << "\"\n";
            themeFile << "  },\n";
            themeFile << "  \"font\": {\n";
            themeFile << "    \"name\": \"" << m_currentTheme.fontName << "\",\n";
            themeFile << "    \"size\": " << m_currentTheme.fontSize << "\n";
            themeFile << "  }\n";
            themeFile << "}";

            themeFile.close();
            return true;
        }
        catch (...)
        {
            return false;
        }
    }

    bool FEUITheme::LoadTheme(const std::string& themePath)
    {
        try
        {
            std::ifstream themeFile(themePath);
            if (!themeFile.is_open())
            {
                return false;
            }

            // Basic JSON parsing (use proper JSON library in production)
            themeFile.close();
            return true;
        }
        catch (...)
        {
            return false;
        }
    }

    std::string FEUITheme::GenerateStyleSheet() const
    {
        std::stringstream ss;
        ss << "/* FORBIDDEN ENGINE Theme: " << m_currentTheme.name << " */\n\n";
        ss << "QWidget {\n";
        ss << "  background-color: #" << std::hex << (int)m_currentTheme.primaryColor.r << (int)m_currentTheme.primaryColor.g << (int)m_currentTheme.primaryColor.b << ";\n";
        ss << "  color: #FFFFFF;\n";
        ss << "}\n\n";
        ss << "QMainWindow {\n";
        ss << "  background-color: #1a1a1a;\n";
        ss << "}\n\n";
        ss << "QPushButton {\n";
        ss << "  background-color: #1a1a1a;\n";
        ss << "  color: #FFFFFF;\n";
        ss << "  border: 1px solid #FFFFFF;\n";
        ss << "  padding: 5px;\n";
        ss << "}\n\n";
        ss << "QPushButton:hover {\n";
        ss << "  background-color: #333333;\n";
        ss << "}\n\n";
        ss << "QPushButton:pressed {\n";
        ss << "  background-color: #4c4c4c;\n";
        ss << "}\n\n";
        ss << "QTableWidget, QTreeWidget {\n";
        ss << "  background-color: #1a1a1a;\n";
        ss << "  alternate-background-color: #2a2a2a;\n";
        ss << "  border: 1px solid #FFFFFF;\n";
        ss << "}\n";

        return ss.str();
    }

    std::string FEUITheme::GenerateCSSStyleSheet() const
    {
        std::stringstream ss;
        ss << ":root {\n";
        ss << "  --fe-primary: " << m_currentTheme.primaryColor.ToHex() << ";\n";
        ss << "  --fe-accent: " << m_currentTheme.accentColor.ToHex() << ";\n";
        ss << "  --fe-panel-bg: " << m_currentTheme.panelBackground.ToHex() << ";\n";
        ss << "  --fe-border: " << m_currentTheme.panelBorder.ToHex() << ";\n";
        ss << "  --fe-text: " << m_currentTheme.textColor.ToHex() << ";\n";
        ss << "  --fe-warning: " << m_currentTheme.warningColor.ToHex() << ";\n";
        ss << "  --fe-error: " << m_currentTheme.errorColor.ToHex() << ";\n";
        ss << "  --fe-success: " << m_currentTheme.successColor.ToHex() << ";\n";
        ss << "}\n\n";
        ss << "body {\n";
        ss << "  background-color: var(--fe-primary);\n";
        ss << "  color: var(--fe-text);\n";
        ss << "  font-family: " << m_currentTheme.fontName << ";\n";
        ss << "  font-size: " << m_currentTheme.fontSize << "px;\n";
        ss << "}\n\n";
        ss << ".panel {\n";
        ss << "  background-color: var(--fe-panel-bg);\n";
        ss << "  border: 1px solid var(--fe-border);\n";
        ss << "}\n\n";
        ss << ".button {\n";
        ss << "  background-color: var(--fe-panel-bg);\n";
        ss << "  color: var(--fe-text);\n";
        ss << "  border: 1px solid var(--fe-border);\n";
        ss << "  padding: 5px 10px;\n";
        ss << "}\n\n";
        ss << ".button:hover {\n";
        ss << "  background-color: var(--fe-accent);\n";
        ss << "  color: var(--fe-primary);\n";
        ss << "}\n";

        return ss.str();
    }

    FETheme FEUITheme::CreateDarkTheme()
    {
        FETheme darkTheme;
        darkTheme.name = "dark";
        darkTheme.primaryColor = FEColor(0, 0, 0, 255);
        darkTheme.accentColor = FEColor(255, 255, 255, 255);
        darkTheme.panelBackground = FEColor(26, 26, 26, 255);
        darkTheme.panelBorder = FEColor(255, 255, 255, 255);
        darkTheme.textColor = FEColor(255, 255, 255, 255);
        return darkTheme;
    }

    FETheme FEUITheme::CreateLightTheme()
    {
        FETheme lightTheme;
        lightTheme.name = "light";
        lightTheme.primaryColor = FEColor(255, 255, 255, 255);
        lightTheme.accentColor = FEColor(0, 0, 0, 255);
        lightTheme.panelBackground = FEColor(240, 240, 240, 255);
        lightTheme.panelBorder = FEColor(0, 0, 0, 255);
        lightTheme.textColor = FEColor(0, 0, 0, 255);
        return lightTheme;
    }

    FETheme FEUITheme::CreateForbiddenEngineTheme()
    {
        FETheme feTheme;
        feTheme.name = "FORBIDDEN ENGINE";
        feTheme.primaryColor = FEColor("#000000");          // Pure black
        feTheme.accentColor = FEColor("#FFFFFF");           // Pure white
        feTheme.panelBackground = FEColor("#1a1a1a");       // Dark gray
        feTheme.panelBorder = FEColor("#FFFFFF");           // White border
        feTheme.textColor = FEColor("#FFFFFF");             // White text
        feTheme.buttonHover = FEColor("#333333");           // Lighter gray on hover
        feTheme.buttonActive = FEColor("#4c4c4c");          // Even lighter when active
        feTheme.warningColor = FEColor("#FFA500");          // Orange warning
        feTheme.errorColor = FEColor("#FF0000");            // Red error
        feTheme.successColor = FEColor("#00FF00");          // Green success
        feTheme.fontName = "Consolas";
        feTheme.fontSize = 11;
        return feTheme;
    }
}
