#pragma once

#include "Project.h"
#include <memory>
#include <string>

namespace WinMMM10 {

class ProjectManager {
public:
    ProjectManager();
    ~ProjectManager() = default;
    
    bool createProject(const std::string& filepath, const std::string& name);
    bool loadProject(const std::string& filepath);
    bool saveProject();
    bool saveProjectAs(const std::string& filepath);
    
    bool hasCurrentProject() const { return m_currentProject != nullptr; }
    Project* currentProject() { return m_currentProject.get(); }
    const Project* currentProject() const { return m_currentProject.get(); }
    
    void closeProject();
    
    bool hasUnsavedChanges() const { return m_hasUnsavedChanges; }
    void markChanged() { m_hasUnsavedChanges = true; }
    void markSaved() { m_hasUnsavedChanges = false; }

private:
    std::unique_ptr<Project> m_currentProject;
    bool m_hasUnsavedChanges{false};
};

} // namespace WinMMM10

