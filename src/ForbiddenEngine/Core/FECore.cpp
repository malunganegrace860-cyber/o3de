#include "FECore.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <ctime>
#include <windows.h>

namespace ForbiddenEngine
{
    // Global engine instance
    static FECore* g_engine = nullptr;

    FECore::FECore()
        : m_isInitialized(false)
        , m_version(FE_VERSION)
        , m_engineName(FE_ENGINE_NAME)
        , m_platform(FE_PLATFORM)
    {
    }

    FECore::~FECore()
    {
        if (m_isInitialized)
        {
            Shutdown();
        }
    }

    bool FECore::Initialize()
    {
        if (m_isInitialized)
        {
            LogWarning("Engine already initialized!");
            return true;
        }

        Log("========================================");
        Log("FORBIDDEN ENGINE v" + m_version);
        Log("Platform: " + m_platform);
        Log("========================================");

        // Setup directory structure
        if (!SetupPaths())
        {
            LogError("Failed to setup directory paths");
            return false;
        }

        // Load or create configuration
        if (!SetupDefaultConfiguration())
        {
            LogError("Failed to setup default configuration");
            return false;
        }

        // Load configuration file if exists
        LoadConfigurationFile();

        m_isInitialized = true;
        Log("Engine initialized successfully");

        // Call initialization callbacks
        for (auto& callback : m_initializeCallbacks)
        {
            callback();
        }

        return true;
    }

    bool FECore::Shutdown()
    {
        if (!m_isInitialized)
        {
            return true;
        }

        Log("\nShutting down FORBIDDEN ENGINE...");

        // Call shutdown callbacks
        for (auto& callback : m_shutdownCallbacks)
        {
            callback();
        }

        Log("Engine shutdown complete");
        m_isInitialized = false;
        return true;
    }

    bool FECore::SetupPaths()
    {
        try
        {
            // Get home directory
            const char* homeDrive = std::getenv("HOMEDRIVE");
            const char* homePath = std::getenv("HOMEPATH");
            
            if (!homeDrive || !homePath)
            {
                LogError("Could not determine home directory");
                return false;
            }

            std::string homeDir = std::string(homeDrive) + std::string(homePath);
            std::string feDir = homeDir + "\\" + FE_CONFIGURATION_DIR;

            // Create FE directory structure
            std::filesystem::path fePath(feDir);
            if (!std::filesystem::exists(fePath))
            {
                std::filesystem::create_directories(fePath / "projects");
                std::filesystem::create_directories(fePath / "cache");
                std::filesystem::create_directories(fePath / "logs");
                Log("Created FORBIDDEN ENGINE directory structure at: " + feDir);
            }
            else
            {
                // Ensure subdirectories exist
                std::filesystem::create_directories(fePath / "projects");
                std::filesystem::create_directories(fePath / "cache");
                std::filesystem::create_directories(fePath / "logs");
            }

            return true;
        }
        catch (const std::exception& e)
        {
            LogError(std::string("Path setup error: ") + e.what());
            return false;
        }
    }

    bool FECore::SetupDefaultConfiguration()
    {
        // Set default configuration values
        m_configuration["engine.name"] = m_engineName;
        m_configuration["engine.version"] = m_version;
        m_configuration["engine.platform"] = m_platform;
        m_configuration["ui.theme"] = "dark";
        m_configuration["ui.primaryColor"] = "#000000";
        m_configuration["ui.accentColor"] = "#FFFFFF";
        m_configuration["editor.autoSaveInterval"] = "1000";
        m_configuration["editor.defaultSceneWidth"] = "1920";
        m_configuration["editor.defaultSceneHeight"] = "1080";
        m_configuration["project.deletionSteps"] = "7";

        return true;
    }

    bool FECore::LoadConfigurationFile()
    {
        try
        {
            std::string configPath = GetConfigPath() + "\\fe_config.json";
            std::ifstream configFile(configPath);

            if (configFile.is_open())
            {
                Log("Loading configuration from: " + configPath);
                configFile.close();
                return true;
            }
            else
            {
                Log("Configuration file not found, using defaults");
                return true;
            }
        }
        catch (const std::exception& e)
        {
            LogWarning(std::string("Configuration load error: ") + e.what());
            return true; // Don't fail on config load
        }
    }

    std::string FECore::GetProjectsPath() const
    {
        const char* homeDrive = std::getenv("HOMEDRIVE");
        const char* homePath = std::getenv("HOMEPATH");
        return std::string(homeDrive) + std::string(homePath) + "\\" + FE_CONFIGURATION_DIR + "\\projects";
    }

    std::string FECore::GetConfigPath() const
    {
        const char* homeDrive = std::getenv("HOMEDRIVE");
        const char* homePath = std::getenv("HOMEPATH");
        return std::string(homeDrive) + std::string(homePath) + "\\" + FE_CONFIGURATION_DIR;
    }

    std::string FECore::GetCachePath() const
    {
        const char* homeDrive = std::getenv("HOMEDRIVE");
        const char* homePath = std::getenv("HOMEPATH");
        return std::string(homeDrive) + std::string(homePath) + "\\" + FE_CONFIGURATION_DIR + "\\cache";
    }

    std::string FECore::GetLogsPath() const
    {
        const char* homeDrive = std::getenv("HOMEDRIVE");
        const char* homePath = std::getenv("HOMEPATH");
        return std::string(homeDrive) + std::string(homePath) + "\\" + FE_CONFIGURATION_DIR + "\\logs";
    }

    void FECore::Log(const std::string& message)
    {
        // Get current time
        auto now = std::time(nullptr);
        auto tm = std::localtime(&now);
        char timeBuffer[100];
        std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", tm);

        std::string logMessage = "[" + std::string(timeBuffer) + "] " + message;
        std::cout << logMessage << std::endl;

        // Also write to log file
        try
        {
            std::string logPath = GetLogsPath() + "\\forbidden_engine.log";
            std::ofstream logFile(logPath, std::ios::app);
            if (logFile.is_open())
            {
                logFile << logMessage << std::endl;
                logFile.close();
            }
        }
        catch (...) { /* Ignore logging errors */ }
    }

    void FECore::LogWarning(const std::string& message)
    {
        Log("[WARNING] " + message);
    }

    void FECore::LogError(const std::string& message)
    {
        Log("[ERROR] " + message);
    }

    bool FECore::LoadConfiguration(const std::string& configPath)
    {
        try
        {
            std::ifstream configFile(configPath);
            if (!configFile.is_open())
            {
                LogError("Could not open configuration file: " + configPath);
                return false;
            }

            Log("Configuration loaded from: " + configPath);
            configFile.close();
            return true;
        }
        catch (const std::exception& e)
        {
            LogError(std::string("Configuration load error: ") + e.what());
            return false;
        }
    }

    bool FECore::SaveConfiguration(const std::string& configPath)
    {
        try
        {
            std::ofstream configFile(configPath);
            if (!configFile.is_open())
            {
                LogError("Could not open configuration file for writing: " + configPath);
                return false;
            }

            // Write JSON configuration
            configFile << "{\n";
            configFile << "  \"engine\": {\n";
            configFile << "    \"name\": \"" << m_engineName << "\",\n";
            configFile << "    \"version\": \"" << m_version << "\",\n";
            configFile << "    \"platform\": \"" << m_platform << "\"\n";
            configFile << "  },\n";
            configFile << "  \"ui\": {\n";
            configFile << "    \"theme\": \"dark\",\n";
            configFile << "    \"primaryColor\": \"#000000\",\n";
            configFile << "    \"accentColor\": \"#FFFFFF\"\n";
            configFile << "  }\n";
            configFile << "}\n";

            configFile.close();
            Log("Configuration saved to: " + configPath);
            return true;
        }
        catch (const std::exception& e)
        {
            LogError(std::string("Configuration save error: ") + e.what());
            return false;
        }
    }

    void FECore::RegisterInitializeCallback(EngineCallback callback)
    {
        m_initializeCallbacks.push_back(callback);
    }

    void FECore::RegisterShutdownCallback(EngineCallback callback)
    {
        m_shutdownCallbacks.push_back(callback);
    }

    FECore& GetEngine()
    {
        if (!g_engine)
        {
            g_engine = new FECore();
        }
        return *g_engine;
    }
}
