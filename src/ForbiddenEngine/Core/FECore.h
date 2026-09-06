#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <functional>

namespace ForbiddenEngine
{
    // Version information
    constexpr const char* FE_VERSION = "0.1.0-alpha";
    constexpr const char* FE_ENGINE_NAME = "FORBIDDEN ENGINE";
    constexpr const char* FE_PLATFORM = "Windows";
    constexpr const char* FE_CONFIGURATION_DIR = ".ForbiddenEngine";

    // Core engine initialization and shutdown
    class FECore
    {
    public:
        FECore();
        ~FECore();

        // Initialization
        bool Initialize();
        bool Shutdown();
        bool IsInitialized() const { return m_isInitialized; }

        // Engine info
        const std::string& GetVersion() const { return m_version; }
        const std::string& GetEngineName() const { return m_engineName; }
        const std::string& GetPlatform() const { return m_platform; }

        // Configuration management
        bool LoadConfiguration(const std::string& configPath);
        bool SaveConfiguration(const std::string& configPath);
        const std::map<std::string, std::string>& GetConfig() const { return m_configuration; }

        // Project paths
        std::string GetProjectsPath() const;
        std::string GetConfigPath() const;
        std::string GetCachePath() const;
        std::string GetLogsPath() const;

        // Logging system
        void Log(const std::string& message);
        void LogWarning(const std::string& message);
        void LogError(const std::string& message);

        // Callback system for engine events
        typedef std::function<void()> EngineCallback;
        void RegisterInitializeCallback(EngineCallback callback);
        void RegisterShutdownCallback(EngineCallback callback);

    private:
        bool m_isInitialized;
        std::string m_version;
        std::string m_engineName;
        std::string m_platform;
        std::map<std::string, std::string> m_configuration;
        std::vector<EngineCallback> m_initializeCallbacks;
        std::vector<EngineCallback> m_shutdownCallbacks;

        // Internal setup methods
        bool SetupPaths();
        bool SetupDefaultConfiguration();
        bool LoadConfigurationFile();
    };

    // Global engine instance accessor
    FECore& GetEngine();
}
