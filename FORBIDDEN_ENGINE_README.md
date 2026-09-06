# FORBIDDEN ENGINE (FE)

**A high-performance, professional-grade game engine forked from Open 3D Engine (O3DE)**

> **Platform:** Windows PC (EXE) Only  
> **Status:** Phase 1 - Enhanced Nodes, Workspace & Editor  
> **Version:** 0.1.0-alpha

---

## 🎮 FORBIDDEN ENGINE Vision

FORBIDDEN ENGINE is a comprehensive game development platform built on the foundation of O3DE, enhanced with cutting-edge rendering, physics, animation, and tooling systems. It's designed for professional developers who demand complete control and extensibility.

### 📋 Development Roadmap

- **Phase 1** ✅ Enhanced Nodes, Workspace & Editor
- **Phase 2** Advanced Marketplace & Build Tools (IPA, EXE, AAB)
- **Phase 3** Physics System Enhancement
- **Phase 4** Advanced Physics Integration (Jolt, CoACD, Splash Splash, MeshOptimizer)
- **Phase 5** Animation System Enhancement (Ozz, Fabrik, OpenIK, Motion Matching, Wiggle Bones)
- **Phase 6** Advanced Rendering (PBRT, Basis Universal, KTX, XAtlas, AMD FidelityFX, Recast, Detour, MaterialXI, OpenPBR, Tracy Profiler, Slang)
- **Phase 7** Professional Rendering Tools (Google Filament, Intel OIDN, Volk, DirectXTex, OpenColorIO, FSR 3, LuxCoreRender, MoonRay)
- **Phase 8** Multi-Language Scripting (C++, C#, Lua)
- **Phase 9** Full Visual & Script Editor with Asset Store
- **Phase 10** FE Branding, Logo & Polish

---

## 🚀 Phase 1: Enhanced Nodes, Workspace & Editor

### What's Included

#### 1. **Enhanced Node System**
- Custom node architecture extending O3DE's ECS
- Visual node graph implementation
- Node connection validation system
- Dynamic property binding
- Node serialization/deserialization

#### 2. **Enhanced Workspace**
- Black UI with white accents (no comments on UI)
- Responsive layout fitting all screen resolutions
- Multi-panel workspace (Scene, Inspector, Assets, Console)
- Dockable windows
- Persistent workspace layout

#### 3. **Enhanced Editor**
- Visual scene editor with gizmo support
- Hierarchy/Scene tree
- Inspector panel for properties
- Asset browser
- Console with error detection
- Default scene with daylight and white box at fixed position
- Real-time rotation preview of all sides

#### 4. **FE Branding**
- Cybernetic FE logo (sharp, geometric, white accents)
- Splash screen on startup
- FE logo in UI header
- Professional branding throughout

#### 5. **Project Management**
- Home directory folder structure: `~/.ForbiddenEngine/projects/`
- Each project gets its own isolated folder
- Automatic project creation/deletion system (7-step deletion process)
- 1-second auto-save functionality

---

## 📦 Directory Structure

```
forbidden-engine/
├── CMakeLists.txt                 # Phase 1 build configuration
├── FORBIDDEN_ENGINE_README.md     # This file
├── src/
│   ├── ForbiddenEngine/
│   │   ├── Core/
│   │   │   ├── FECore.h
│   │   │   └── FECore.cpp
│   │   ├── Nodes/
│   │   │   ├── FENode.h
│   │   │   ├── FENode.cpp
│   │   │   ├── FENodeGraph.h
│   │   │   └── FENodeGraph.cpp
│   │   ├── Editor/
│   │   │   ├── FEEditor.h
│   │   │   ├── FEEditor.cpp
│   │   │   ├── FESceneEditor.h
│   │   │   ├── FESceneEditor.cpp
│   │   │   ├── FEInspector.h
│   │   │   └── FEInspector.cpp
│   │   ├── UI/
│   │   │   ├── FEUITheme.h
│   │   │   ├── FEUITheme.cpp
│   │   │   ├── FEWorkspace.h
│   │   │   └── FEWorkspace.cpp
│   │   ├── Project/
│   │   │   ├── FEProject.h
│   │   │   ├── FEProject.cpp
│   │   │   ├── FEProjectManager.h
│   │   │   └── FEProjectManager.cpp
│   │   └── Assets/
│   │       ├── FEAssetBrowser.h
│   │       └── FEAssetBrowser.cpp
├── assets/
│   ├── branding/
│   │   ├── fe_logo.svg
│   │   ├── fe_splash_screen.png
│   │   └── fe_logo_icon.ico
│   └── ui/
│       └── dark_theme.qss
├── config/
│   ├── fe_config.json
│   └── default_scene.json
└── docs/
    ├── PHASE_1_IMPLEMENTATION.md
    ├── NODE_SYSTEM.md
    ├── EDITOR_GUIDE.md
    └── PROJECT_STRUCTURE.md
```

---

## 🛠️ Building Phase 1

### Requirements
- Windows 10+ (64-bit)
- CMake 3.20+
- Visual Studio 2019+ or Clang
- Qt 6.2+ (for UI)

### Build Instructions

```bash
# Clone your FORBIDDEN ENGINE fork
git clone https://github.com/malunganegrace860-cyber/o3de.git forbidden-engine
cd forbidden-engine

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake .. -G "Visual Studio 16 2019" -A x64

# Build Phase 1
cmake --build . --config Release

# Run FORBIDDEN ENGINE
./bin/ForbiddenEngine.exe
```

---

## 💾 Project Folder System

### Folder Structure
```
~/.ForbiddenEngine/
├── projects/
│   ├── MyGameProject/
│   │   ├── project.feproj
│   │   ├── scenes/
│   │   ├── assets/
│   │   ├── scripts/
│   │   ├── config/
│   │   └── builds/
│   └── AnotherProject/
└── cache/
```

### Auto-Save
- 1-second auto-save interval
- Incremental saves to prevent data loss
- Auto-recovery on crash

---

## 🎨 UI/UX Features (Phase 1)

### Color Scheme
- **Primary:** Pure Black (#000000)
- **Accent:** Pure White (#FFFFFF)
- **Panels:** Dark Gray (#1a1a1a)
- **Borders:** White (#FFFFFF)

### Responsive Layout
- Adaptive to screen resolution
- All buttons fully responsive
- Dockable panels
- Customizable window layout

### Editor Components
1. **Scene Viewport** - 3D rendering with gizmo
2. **Hierarchy Panel** - Scene tree view
3. **Inspector** - Property editing
4. **Asset Browser** - File management
5. **Console** - Error detection & logging

---

## 🔧 Default Scene Setup

Every new project starts with:
- ✅ Daylight (sun/sky)
- ✅ Gizmo (position, rotation, scale)
- ✅ White box at fixed position (rotatable from all sides)
- ✅ Default camera
- ✅ Grid floor

---

## ✨ Key Phase 1 Features

| Feature | Status | Notes |
|---------|--------|-------|
| Enhanced Node System | ✅ Complete | Full visual node graph |
| Workspace UI | ✅ Complete | Black/white theme, responsive |
| Scene Editor | ✅ Complete | Gizmo, hierarchy, inspector |
| Project Manager | ✅ Complete | Auto-creation, 7-step deletion |
| Auto-Save | ✅ Complete | 1-second interval |
| Default Scene | ✅ Complete | Daylight, gizmo, white box |
| FE Branding | ✅ Complete | Logo, splash screen |
| Asset Browser | ✅ Complete | File browser integration |

---

## 📝 Configuration Files

### fe_config.json
```json
{
  "engine": {
    "name": "FORBIDDEN ENGINE",
    "version": "0.1.0-alpha",
    "platform": "Windows",
    "projectsPath": "~/.ForbiddenEngine/projects"
  },
  "ui": {
    "theme": "dark",
    "primaryColor": "#000000",
    "accentColor": "#FFFFFF"
  },
  "editor": {
    "autoSaveInterval": 1000,
    "defaultSceneSize": [1920, 1080]
  }
}
```

---

## 🚪 Next Steps

After Phase 1 is complete and tested, we proceed to:

1. **Phase 2** - Marketplace & Build Tools
2. **Phase 3-4** - Physics Systems
3. **Phase 5** - Animation Systems
4. **Phase 6-7** - Advanced Rendering
5. **Phase 8-10** - Scripting, Final Polish

---

## 📄 License

FORBIDDEN ENGINE is built on O3DE (Apache 2.0 License)

---

## 🎯 Getting Started

1. Build Phase 1 from source
2. Launch `ForbiddenEngine.exe`
3. Create your first project
4. Explore the enhanced node system
5. Design scenes with the visual editor

---

**FORBIDDEN ENGINE: Where Professional Developers Build Tomorrow's Games**

---

*Last Updated: Phase 1 Implementation*  
*Next Phase: Coming Soon*
