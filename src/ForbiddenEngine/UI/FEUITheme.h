#pragma once

#include <string>
#include <map>
#include <array>

namespace ForbiddenEngine
{
    // RGBA Color structure
    struct FEColor
    {
        unsigned char r, g, b, a;

        FEColor(unsigned char red = 0, unsigned char green = 0, unsigned char blue = 0, unsigned char alpha = 255)
            : r(red), g(green), b(blue), a(alpha)
        {
        }

        // Hex color constructor
        explicit FEColor(const std::string& hexColor)
        {
            if (hexColor.length() == 7 && hexColor[0] == '#')
            {
                r = static_cast<unsigned char>(std::stoi(hexColor.substr(1, 2), nullptr, 16));
                g = static_cast<unsigned char>(std::stoi(hexColor.substr(3, 2), nullptr, 16));
                b = static_cast<unsigned char>(std::stoi(hexColor.substr(5, 2), nullptr, 16));
                a = 255;
            }
            else
            {
                r = g = b = 0;
                a = 255;
            }
        }

        std::string ToHex() const
        {
            char buffer[8];
            sprintf_s(buffer, "#%02X%02X%02X", r, g, b);
            return std::string(buffer);
        }
    };

    // Theme structure
    struct FETheme
    {
        std::string name;
        FEColor primaryColor;        // Black for FORBIDDEN ENGINE
        FEColor accentColor;         // White for FORBIDDEN ENGINE
        FEColor panelBackground;     // Dark gray
        FEColor panelBorder;         // White border
        FEColor textColor;           // White text
        FEColor buttonHover;         // Slightly lighter
        FEColor buttonActive;        // Even lighter
        FEColor warningColor;        // Red/Orange for warnings
        FEColor errorColor;          // Red for errors
        FEColor successColor;        // Green for success
        std::string fontName;
        int fontSize;

        FETheme()
            : name("dark")
            , primaryColor(0, 0, 0, 255)                    // Black
            , accentColor(255, 255, 255, 255)              // White
            , panelBackground(26, 26, 26, 255)             // Dark Gray
            , panelBorder(255, 255, 255, 255)              // White
            , textColor(255, 255, 255, 255)                // White
            , buttonHover(51, 51, 51, 255)                 // Lighter gray
            , buttonActive(76, 76, 76, 255)                // Even lighter
            , warningColor(255, 165, 0, 255)               // Orange
            , errorColor(255, 0, 0, 255)                   // Red
            , successColor(0, 255, 0, 255)                 // Green
            , fontName("Arial")
            , fontSize(12)
        {
        }
    };

    // Theme manager
    class FEUITheme
    {
    public:
        FEUITheme();
        ~FEUITheme();

        // Theme management
        void Initialize();
        void SetTheme(const FETheme& theme);
        const FETheme& GetCurrentTheme() const { return m_currentTheme; }
        const std::string& GetCurrentThemeName() const { return m_currentTheme.name; }

        // Color access
        const FEColor& GetPrimaryColor() const { return m_currentTheme.primaryColor; }
        const FEColor& GetAccentColor() const { return m_currentTheme.accentColor; }
        const FEColor& GetPanelBackground() const { return m_currentTheme.panelBackground; }
        const FEColor& GetPanelBorder() const { return m_currentTheme.panelBorder; }
        const FEColor& GetTextColor() const { return m_currentTheme.textColor; }
        const FEColor& GetButtonHover() const { return m_currentTheme.buttonHover; }
        const FEColor& GetButtonActive() const { return m_currentTheme.buttonActive; }
        const FEColor& GetWarningColor() const { return m_currentTheme.warningColor; }
        const FEColor& GetErrorColor() const { return m_currentTheme.errorColor; }
        const FEColor& GetSuccessColor() const { return m_currentTheme.successColor; }

        // Font management
        void SetFont(const std::string& fontName, int fontSize);
        const std::string& GetFontName() const { return m_currentTheme.fontName; }
        int GetFontSize() const { return m_currentTheme.fontSize; }

        // Theme persistence
        bool SaveTheme(const std::string& themePath);
        bool LoadTheme(const std::string& themePath);

        // Style sheet generation
        std::string GenerateStyleSheet() const;
        std::string GenerateCSSStyleSheet() const;

        // Predefined themes
        static FETheme CreateDarkTheme();
        static FETheme CreateLightTheme();
        static FETheme CreateForbiddenEngineTheme();

    private:
        FETheme m_currentTheme;
        FETheme m_defaultTheme;
    };

    typedef std::shared_ptr<FEUITheme> ThemePtr;
}
