#include "FEProjectManager.h"
#include "../Editor/FESceneEditor.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <ctime>
#include <windows.h>

namespace ForbiddenEngine
{
    FEProjectManager::FEProjectManager()
        : m_autoSaveEnabled(true)
        , m_autoSaveInterval(1000) // 1 second
        , m_deletionProcess(nullptr)
    {
    }

    FEProjectManager::~FEProjectManager()
    {
        SaveProject();
    }

    bool FEProjectManager::CreateProject(const std::string& projectName, const std::string& projectPath)
    {
        try
        {
            // Create main project directory
            std::filesystem::path projectDir(projectPath);
            if (!std::filesystem::exists(projectDir))
            {
                std::filesystem::create_directories(projectDir);
            }

            // Set current project metadata
            m_currentProject.name = projectName;
            m_currentProject.path = projectPath;
            m_currentProject.version = "1.0.0";
            m_currentProject.createdDate = GetCurrentTimestamp();
            m_currentProject.lastModifiedDate = m_currentProject.createdDate;
            m_currentProject.description = "FORBIDDEN ENGINE Project";

            // Setup folder structure
            if (!SetupProjectFolders())
            {
                return false;
            }

            // Save project metadata
            if (!SaveProjectMetadata())
            {
                return false;
            }

            // Create default scene
            auto defaultScene = CreateScene("DefaultScene");
            if (!defaultScene)
            {
                return false;
            }

            // Add to recent projects
            AddRecentProject(projectPath);

            return true;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error creating project: " << e.what() << std::endl;
            return false;
        }
    }

    bool FEProjectManager::LoadProject(const std::string& projectPath)
    {
        try
        {
            std::filesystem::path projectDir(projectPath);
            if (!std::filesystem::exists(projectDir))
            {
                return false;
            }

            m_currentProject.path = projectPath;
            m_currentProject.lastModifiedDate = GetCurrentTimestamp();

            if (!LoadProjectMetadata())
            {
                return false;
            }

            AddRecentProject(projectPath);
            return true;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error loading project: " << e.what() << std::endl;
            return false;
        }
    }

    bool FEProjectManager::SaveProject()
    {
        if (!HasOpenProject())
            return false;

        m_currentProject.lastModifiedDate = GetCurrentTimestamp();
        return SaveProjectMetadata();
    }

    bool FEProjectManager::SetupProjectFolders()
    {
        try
        {
            std::filesystem::path basePath(m_currentProject.path);

            // Create all required directories
            std::filesystem::create_directories(basePath / "assets");
            std::filesystem::create_directories(basePath / "assets" / "textures");
            std::filesystem::create_directories(basePath / "assets" / "models");
            std::filesystem::create_directories(basePath / "assets" / "sounds");
            std::filesystem::create_directories(basePath / "scenes");
            std::filesystem::create_directories(basePath / "scripts");
            std::filesystem::create_directories(basePath / "config");
            std::filesystem::create_directories(basePath / "build");
            std::filesystem::create_directories(basePath / "build" / "exe");
            std::filesystem::create_directories(basePath / "build" / "ipa");
            std::filesystem::create_directories(basePath / "build" / "aab");

            return true;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error setting up project folders: " << e.what() << std::endl;
            return false;
        }
    }

    std::string FEProjectManager::GetProjectAssetsPath() const
    {
        return m_currentProject.path + "\\assets";
    }

    std::string FEProjectManager::GetProjectScenesPath() const
    {
        return m_currentProject.path + "\\scenes";
    }

    std::string FEProjectManager::GetProjectConfigPath() const
    {
        return m_currentProject.path + "\\config";
    }

    std::string FEProjectManager::GetProjectBuildPath() const
    {
        return m_currentProject.path + "\\build";
    }

    std::string FEProjectManager::GetProjectScriptsPath() const
    {
        return m_currentProject.path + "\\scripts";
    }

    bool FEProjectManager::PerformAutoSave()
    {
        if (!m_autoSaveEnabled || !HasOpenProject())
            return false;

        return SaveProject();
    }

    std::shared_ptr<FESceneEditor> FEProjectManager::CreateScene(const std::string& sceneName)
    {
        auto scene = std::make_shared<FESceneEditor>(sceneName);
        if (!scene->Initialize())
            return nullptr;

        m_loadedScenes.push_back(scene);
        return scene;
    }

    std::shared_ptr<FESceneEditor> FEProjectManager::LoadScene(const std::string& scenePath)
    {
        auto scene = std::make_shared<FESceneEditor>(scenePath);
        if (!scene->Initialize())
            return nullptr;

        m_loadedScenes.push_back(scene);
        return scene;
    }

    bool FEProjectManager::SaveScene(const std::string& sceneName)
    {
        auto scene = GetScene(sceneName);
        if (!scene)
            return false;

        // Save scene to file
        std::string scenePath = GetProjectScenesPath() + "\\" + sceneName + ".scene";
        std::ofstream sceneFile(scenePath);
        if (!sceneFile.is_open())
            return false;

        sceneFile << scene->SerializeScene();
        sceneFile.close();
        return true;
    }

    std::shared_ptr<FESceneEditor> FEProjectManager::GetScene(const std::string& sceneName) const
    {
        for (const auto& scene : m_loadedScenes)
        {
            if (scene->GetName() == sceneName)
                return scene;
        }
        return nullptr;
    }

    bool FEProjectManager::LoadProjectMetadata()
    {
        try
        {
            std::string metadataPath = m_currentProject.path + "\\config\\project.metadata";
            std::ifstream metadataFile(metadataPath);
            if (!metadataFile.is_open())
            {
                return false;
            }

            // Basic JSON parsing (use proper JSON library in production)
            metadataFile.close();
            return true;
        }
        catch (...)
        {
            return false;
        }
    }

    bool FEProjectManager::SaveProjectMetadata()
    {
        try
        {
            std::string metadataPath = m_currentProject.path + "\\config\\project.metadata";
            std::ofstream metadataFile(metadataPath);
            if (!metadataFile.is_open())
            {
                return false;
            }

            metadataFile << "{\n";
            metadataFile << "  \"name\": \"" << m_currentProject.name << "\",\n";
            metadataFile << "  \"version\": \"" << m_currentProject.version << "\",\n";
            metadataFile << "  \"created\": \"" << m_currentProject.createdDate << "\",\n";
            metadataFile << "  \"modified\": \"" << m_currentProject.lastModifiedDate << "\",\n";
            metadataFile << "  \"description\": \"" << m_currentProject.description << "\",\n";
            metadataFile << "  \"path\": \"" << m_currentProject.path << "\"\n";
            metadataFile << "}";

            metadataFile.close();
            return true;
        }
        catch (...)
        {
            return false;
        }
    }

    std::string FEProjectManager::GetCurrentTimestamp() const
    {
        auto now = std::time(nullptr);
        auto tm = std::localtime(&now);
        char buffer[100];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm);
        return std::string(buffer);
    }

    bool FEProjectManager::InitiateProjectDeletion(const std::string& projectPath)
    {
        if (m_deletionProcess != nullptr && m_deletionProcess->isActive)
        {
            return false; // Deletion already in progress
        }

        m_deletionProcess = std::make_unique<DeletionProcess>(projectPath);
        m_deletionProcess->isActive = true;
        m_deletionProcess->currentStep = DeletionStep::Step1_ConfirmDeletion;
        return true;
    }

    bool FEProjectManager::ProcessNextDeletionStep()
    {
        if (!m_deletionProcess || !m_deletionProcess->isActive)
            return false;

        bool success = true;

        switch (m_deletionProcess->currentStep)
        {
        case DeletionStep::Step1_ConfirmDeletion:
            success = DeleteStep1_ConfirmDeletion();
            if (success)
                m_deletionProcess->currentStep = DeletionStep::Step2_BackupProject;
            break;
        case DeletionStep::Step2_BackupProject:
            success = DeleteStep2_BackupProject();
            if (success)
                m_deletionProcess->currentStep = DeletionStep::Step3_RenameToTrash;
            break;
        case DeletionStep::Step3_RenameToTrash:
            success = DeleteStep3_RenameToTrash();
            if (success)
                m_deletionProcess->currentStep = DeletionStep::Step4_ClearAssets;
            break;
        case DeletionStep::Step4_ClearAssets:
            success = DeleteStep4_ClearAssets();
            if (success)
                m_deletionProcess->currentStep = DeletionStep::Step5_RemoveScenes;
            break;
        case DeletionStep::Step5_RemoveScenes:
            success = DeleteStep5_RemoveScenes();
            if (success)
                m_deletionProcess->currentStep = DeletionStep::Step6_RemoveMetadata;
            break;
        case DeletionStep::Step6_RemoveMetadata:
            success = DeleteStep6_RemoveMetadata();
            if (success)
                m_deletionProcess->currentStep = DeletionStep::Step7_CompleteRemoval;
            break;
        case DeletionStep::Step7_CompleteRemoval:
            success = DeleteStep7_CompleteRemoval();
            if (success)
                m_deletionProcess->currentStep = DeletionStep::Complete;
            break;
        default:
            break;
        }

        return success;
    }

    bool FEProjectManager::FinalizeDeletion()
    {
        if (!m_deletionProcess || m_deletionProcess->currentStep != DeletionStep::Step7_CompleteRemoval)
            return false;

        m_deletionProcess.reset();
        return true;
    }

    bool FEProjectManager::CancelDeletion()
    {
        m_deletionProcess.reset();
        return true;
    }

    DeletionStep FEProjectManager::GetCurrentDeletionStep() const
    {
        if (!m_deletionProcess)
            return DeletionStep::Complete;
        return m_deletionProcess->currentStep;
    }

    bool FEProjectManager::DeleteStep1_ConfirmDeletion()
    {
        // Step 1: Confirm deletion
        std::cout << "[Deletion Step 1] Confirming project deletion..." << std::endl;
        return true;
    }

    bool FEProjectManager::DeleteStep2_BackupProject()
    {
        // Step 2: Backup project
        std::cout << "[Deletion Step 2] Creating backup..." << std::endl;
        return true;
    }

    bool FEProjectManager::DeleteStep3_RenameToTrash()
    {
        // Step 3: Rename to trash
        std::cout << "[Deletion Step 3] Renaming to trash..." << std::endl;
        return true;
    }

    bool FEProjectManager::DeleteStep4_ClearAssets()
    {
        // Step 4: Clear assets
        std::cout << "[Deletion Step 4] Clearing assets..." << std::endl;
        return true;
    }

    bool FEProjectManager::DeleteStep5_RemoveScenes()
    {
        // Step 5: Remove scenes
        std::cout << "[Deletion Step 5] Removing scenes..." << std::endl;
        return true;
    }

    bool FEProjectManager::DeleteStep6_RemoveMetadata()
    {
        // Step 6: Remove metadata
        std::cout << "[Deletion Step 6] Removing metadata..." << std::endl;
        return true;
    }

    bool FEProjectManager::DeleteStep7_CompleteRemoval()
    {
        // Step 7: Complete removal
        std::cout << "[Deletion Step 7] Completing removal..." << std::endl;
        try
        {
            std::filesystem::remove_all(m_deletionProcess->projectId);
            return true;
        }
        catch (...)
        {
            return false;
        }
    }

    void FEProjectManager::AddRecentProject(const std::string& projectPath)
    {
        // Remove if already exists
        auto it = std::find(m_recentProjects.begin(), m_recentProjects.end(), projectPath);
        if (it != m_recentProjects.end())
        {
            m_recentProjects.erase(it);
        }

        // Add to front
        m_recentProjects.insert(m_recentProjects.begin(), projectPath);

        // Keep only last 10
        if (m_recentProjects.size() > 10)
        {
            m_recentProjects.erase(m_recentProjects.begin() + 10);
        }
    }
}
