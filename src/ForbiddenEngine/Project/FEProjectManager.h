#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>

namespace ForbiddenEngine
{
    // Forward declarations
    class FESceneEditor;

    // Project metadata
    struct ProjectMetadata
    {
        std::string name;
        std::string path;
        std::string version;
        std::string createdDate;
        std::string lastModifiedDate;
        std::string description;
    };

    // Project deletion step
    enum class DeletionStep
    {
        Step1_ConfirmDeletion,
        Step2_BackupProject,
        Step3_RenameToTrash,
        Step4_ClearAssets,
        Step5_RemoveScenes,
        Step6_RemoveMetadata,
        Step7_CompleteRemoval,
        Complete
    };

    // Project deletion process tracker
    struct DeletionProcess
    {
        std::string projectId;
        DeletionStep currentStep;
        bool isActive;
        std::string backupPath;

        DeletionProcess(const std::string& id)
            : projectId(id), currentStep(DeletionStep::Step1_ConfirmDeletion), isActive(false), backupPath("")
        {
        }
    };

    // Project manager
    class FEProjectManager
    {
    public:
        FEProjectManager();
        ~FEProjectManager();

        // Project creation
        bool CreateProject(const std::string& projectName, const std::string& projectPath);
        bool LoadProject(const std::string& projectPath);
        bool SaveProject();

        // Project info
        const ProjectMetadata& GetCurrentProject() const { return m_currentProject; }
        bool HasOpenProject() const { return !m_currentProject.name.empty(); }
        const std::string& GetCurrentProjectPath() const { return m_currentProject.path; }
        const std::string& GetCurrentProjectName() const { return m_currentProject.name; }

        // Project folder structure
        std::string GetProjectAssetsPath() const;
        std::string GetProjectScenesPath() const;
        std::string GetProjectConfigPath() const;
        std::string GetProjectBuildPath() const;
        std::string GetProjectScriptsPath() const;

        // Auto-save
        void EnableAutoSave(bool enable) { m_autoSaveEnabled = enable; }
        bool IsAutoSaveEnabled() const { return m_autoSaveEnabled; }
        void SetAutoSaveInterval(int milliseconds) { m_autoSaveInterval = milliseconds; }
        int GetAutoSaveInterval() const { return m_autoSaveInterval; }
        bool PerformAutoSave();

        // Scene management
        std::shared_ptr<FESceneEditor> CreateScene(const std::string& sceneName);
        std::shared_ptr<FESceneEditor> LoadScene(const std::string& scenePath);
        bool SaveScene(const std::string& sceneName);
        const std::vector<std::shared_ptr<FESceneEditor>>& GetLoadedScenes() const { return m_loadedScenes; }
        std::shared_ptr<FESceneEditor> GetScene(const std::string& sceneName) const;

        // Project deletion (7-step process)
        bool InitiateProjectDeletion(const std::string& projectPath);
        bool ProcessNextDeletionStep();
        bool SkipDeletionStep();
        bool FinalizeDeletion();
        bool CancelDeletion();
        DeletionStep GetCurrentDeletionStep() const;
        bool IsDeletionInProgress() const { return m_deletionProcess != nullptr && m_deletionProcess->isActive; }

        // Recent projects
        void AddRecentProject(const std::string& projectPath);
        const std::vector<std::string>& GetRecentProjects() const { return m_recentProjects; }
        void ClearRecentProjects() { m_recentProjects.clear(); }

        // Project statistics
        int GetSceneCount() const { return static_cast<int>(m_loadedScenes.size()); }
        int GetProjectCount() const { return static_cast<int>(m_recentProjects.size()); }

    private:
        ProjectMetadata m_currentProject;
        std::vector<std::shared_ptr<FESceneEditor>> m_loadedScenes;
        std::vector<std::string> m_recentProjects;
        bool m_autoSaveEnabled;
        int m_autoSaveInterval; // milliseconds
        std::unique_ptr<DeletionProcess> m_deletionProcess;

        // Internal methods
        bool SetupProjectFolders();
        bool LoadProjectMetadata();
        bool SaveProjectMetadata();
        std::string GetCurrentTimestamp() const;

        // Deletion process steps
        bool DeleteStep1_ConfirmDeletion();
        bool DeleteStep2_BackupProject();
        bool DeleteStep3_RenameToTrash();
        bool DeleteStep4_ClearAssets();
        bool DeleteStep5_RemoveScenes();
        bool DeleteStep6_RemoveMetadata();
        bool DeleteStep7_CompleteRemoval();
    };

    typedef std::shared_ptr<FEProjectManager> ProjectManagerPtr;
}
