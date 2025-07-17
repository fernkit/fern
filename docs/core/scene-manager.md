# Scene Manager

The Scene Manager in Fern provides a powerful system for organizing your application into discrete scenes or screens. This is particularly useful for games, multi-screen applications, or any application that needs to switch between different UI states.

## Table of Contents
- [Overview](#overview)
- [Basic Concepts](#basic-concepts)
- [Getting Started](#getting-started)
- [Scene Lifecycle](#scene-lifecycle)
- [Scene Navigation](#scene-navigation)
- [Advanced Features](#advanced-features)
- [Examples](#examples)
- [Best Practices](#best-practices)
- [API Reference](#api-reference)

## Overview

The Scene Manager allows you to:
- **Organize UI into logical screens**: Menu, Game, Settings, etc.
- **Navigate between scenes**: Push, pop, and replace scenes
- **Manage scene lifecycle**: Automatic creation, initialization, and cleanup
- **Maintain scene history**: Stack-based navigation with automatic state management
- **Overlay scenes**: Pause screens, dialogs, and modals

## Basic Concepts

### Scene
A Scene represents a distinct screen or state in your application. Each scene:
- Has a unique name for identification
- Manages its own UI widgets
- Handles its own update and render logic
- Responds to lifecycle events

### Scene Stack
Scenes are managed in a stack structure:
- **Push**: Add a new scene on top (previous scene pauses)
- **Pop**: Remove current scene (previous scene resumes)
- **Replace**: Replace current scene entirely
- **Clear**: Remove all scenes

### Scene Lifecycle
Each scene goes through these states:
1. **Create**: Scene object is instantiated
2. **Enter**: Scene becomes active
3. **Update/Render**: Scene is running
4. **Exit**: Scene becomes inactive
5. **Destroy**: Scene is cleaned up

## Getting Started

### 1. Include Required Headers
```cpp
#include <fern/fern.hpp>
using namespace Fern;
```

### 2. Create Your Scene Class
```cpp
class MenuScene : public Scene {
public:
    MenuScene() : Scene("MenuScene") {}
    
    void onCreate() override {
        // Initialize scene data
    }
    
    void onEnter() override {
        // Set up UI widgets
        setupUI();
    }
    
    void onExit() override {
        // Clean up widgets
        WidgetManager::getInstance().clear();
    }
    
    void render() override {
        Draw::fill(Colors::DarkBlue);
        Scene::render(); // Renders all widgets
    }
    
private:
    void setupUI() {
        auto button = Button(ButtonConfig(0, 0, 200, 50, "Start Game"));
        button->onClick.connect([]() {
            SceneManager::getInstance().pushScene("GameScene");
        });
        addWidget(button);
    }
};
```

### 3. Register and Start Scenes
```cpp
void setupScenes() {
    // Register scenes
    REGISTER_SCENE("MenuScene", MenuScene);
    REGISTER_SCENE("GameScene", GameScene);
    
    // Start with menu
    SceneManager::getInstance().pushScene("MenuScene");
}

int main() {
    Fern::initialize();
    setupScenes();
    
    Fern::setDrawCallback([]() {
        SceneManager::getInstance().render();
    });
    
    Fern::startRenderLoop();
    return 0;
}
```

## Scene Lifecycle

### onCreate()
Called once when the scene is first created.
```cpp
void onCreate() override {
    // Initialize non-UI data
    score = 0;
    level = 1;
    gameTime = 0.0f;
    
    // Load resources
    loadTextures();
    loadSounds();
}
```

### onEnter()
Called every time the scene becomes active.
```cpp
void onEnter() override {
    // Set up UI for this scene
    setupUI();
    
    // Reset temporary state
    isPaused = false;
    
    // Start background music
    playBackgroundMusic();
}
```

### onExit()
Called when the scene becomes inactive.
```cpp
void onExit() override {
    // Clean up UI widgets
    WidgetManager::getInstance().clear();
    
    // Pause audio
    pauseBackgroundMusic();
    
    // Save state if needed
    saveGameState();
}
```

### onDestroy()
Called when the scene is permanently removed.
```cpp
void onDestroy() override {
    // Free resources
    unloadTextures();
    unloadSounds();
    
    // Final cleanup
    clearGameData();
}
```

### update(deltaTime)
Called every frame while the scene is active.
```cpp
void update(float deltaTime) override {
    // Update game logic
    updatePlayer(deltaTime);
    updateEnemies(deltaTime);
    updatePhysics(deltaTime);
    
    // Update UI
    updateScoreDisplay();
}
```

### render()
Called every frame to draw the scene.
```cpp
void render() override {
    // Clear background
    Draw::fill(Colors::Black);
    
    // Render game objects
    renderBackground();
    renderPlayer();
    renderEnemies();
    
    // Render UI widgets
    Scene::render(); // Calls WidgetManager to render all widgets
}
```

## Scene Navigation

### Push Scene
Adds a new scene on top of the current one:
```cpp
// Current scene pauses, new scene becomes active
SceneManager::getInstance().pushScene("SettingsScene");

// Or use the convenience function
pushScene("SettingsScene");
```

### Pop Scene
Removes the current scene and returns to the previous one:
```cpp
// Current scene is destroyed, previous scene resumes
SceneManager::getInstance().popScene();

// Or use the convenience function
popScene();
```

### Replace Scene
Replaces the current scene entirely:
```cpp
// Current scene is destroyed, new scene becomes active
SceneManager::getInstance().replaceScene("GameOverScene");

// Or use the convenience function
replaceScene("GameOverScene");
```

### Clear All Scenes
Removes all scenes from the stack:
```cpp
SceneManager::getInstance().clearScenes();
```

## Advanced Features

### Scene Communication
Pass data between scenes using static variables or singletons:

```cpp
class GameData {
public:
    static GameData& getInstance() {
        static GameData instance;
        return instance;
    }
    
    int score = 0;
    int level = 1;
    std::string playerName;
};

// In one scene
GameData::getInstance().score = 1000;
pushScene("GameOverScene");

// In another scene
int finalScore = GameData::getInstance().score;
```

### Conditional Scene Navigation
```cpp
void onGameEnd() {
    if (GameData::getInstance().score > highScore) {
        pushScene("HighScoreScene");
    } else {
        pushScene("GameOverScene");
    }
}
```

### Scene Overlays
Create overlay scenes that don't replace the background:

```cpp
class PauseScene : public Scene {
public:
    void render() override {
        // Semi-transparent overlay
        Draw::fill(0x80000000); // 50% transparent black
        Scene::render();
    }
};

// Usage
pushScene("PauseScene"); // Game scene stays in background
```

### Dynamic Scene Registration
Register scenes at runtime:
```cpp
SceneManager::getInstance().registerScene("DynamicScene", []() -> std::unique_ptr<Scene> {
    return std::make_unique<MyDynamicScene>();
});
```

## Examples

### Example 1: Simple Menu System
```cpp
class MainMenuScene : public Scene {
public:
    MainMenuScene() : Scene("MainMenu") {}
    
    void onEnter() override {
        auto startBtn = Button(ButtonConfig(0, 0, 200, 50, "Start Game"));
        startBtn->onClick.connect([]() { pushScene("Game"); });
        
        auto settingsBtn = Button(ButtonConfig(0, 0, 200, 50, "Settings"));
        settingsBtn->onClick.connect([]() { pushScene("Settings"); });
        
        auto exitBtn = Button(ButtonConfig(0, 0, 200, 50, "Exit"));
        exitBtn->onClick.connect([]() { exit(0); });
        
        auto layout = Column({startBtn, SizedBox(0, 20), settingsBtn, SizedBox(0, 20), exitBtn});
        auto center = Center(layout);
        addWidget(center);
    }
    
    void onExit() override {
        WidgetManager::getInstance().clear();
    }
    
    void render() override {
        Draw::fill(Colors::DarkBlue);
        Scene::render();
    }
};
```

### Example 2: Game with Pause System
```cpp
class GameScene : public Scene {
private:
    bool isPaused = false;
    
public:
    void update(float deltaTime) override {
        if (!isPaused) {
            updateGameLogic(deltaTime);
        }
    }
    
    void setupUI() {
        auto pauseBtn = Button(ButtonConfig(0, 0, 100, 30, "Pause"));
        pauseBtn->onClick.connect([this]() {
            isPaused = true;
            pushScene("Pause");
        });
        addWidget(pauseBtn);
    }
};

class PauseScene : public Scene {
public:
    void onEnter() override {
        auto resumeBtn = Button(ButtonConfig(0, 0, 150, 40, "Resume"));
        resumeBtn->onClick.connect([]() { popScene(); });
        
        auto menuBtn = Button(ButtonConfig(0, 0, 150, 40, "Main Menu"));
        menuBtn->onClick.connect([]() { 
            clearScenes();
            pushScene("MainMenu");
        });
        
        auto layout = Column({resumeBtn, SizedBox(0, 20), menuBtn});
        addWidget(Center(layout));
    }
    
    void render() override {
        Draw::fill(0x80000000); // Semi-transparent overlay
        Scene::render();
    }
};
```

### Example 3: Modal Dialog System
```cpp
class ConfirmDialogScene : public Scene {
private:
    std::function<void()> onConfirm_;
    std::function<void()> onCancel_;
    
public:
    ConfirmDialogScene(const std::string& message, 
                      std::function<void()> onConfirm,
                      std::function<void()> onCancel = nullptr) 
        : Scene("ConfirmDialog"), onConfirm_(onConfirm), onCancel_(onCancel) {}
    
    void onEnter() override {
        auto messageText = Text(Point(0, 0), message_, 2, Colors::White);
        
        auto confirmBtn = Button(ButtonConfig(0, 0, 100, 40, "OK"));
        confirmBtn->onClick.connect([this]() {
            if (onConfirm_) onConfirm_();
            popScene();
        });
        
        auto cancelBtn = Button(ButtonConfig(0, 0, 100, 40, "Cancel"));
        cancelBtn->onClick.connect([this]() {
            if (onCancel_) onCancel_();
            popScene();
        });
        
        auto buttons = Row({confirmBtn, SizedBox(20, 0), cancelBtn});
        auto dialog = Column({messageText, SizedBox(0, 30), buttons});
        addWidget(Center(dialog));
    }
};

// Usage
void showConfirmDialog(const std::string& message, std::function<void()> onConfirm) {
    SceneManager::getInstance().registerScene("ConfirmDialog", [=]() -> std::unique_ptr<Scene> {
        return std::make_unique<ConfirmDialogScene>(message, onConfirm);
    });
    pushScene("ConfirmDialog");
}
```

## Best Practices

### 1. Clean Resource Management
```cpp
class GameScene : public Scene {
public:
    void onCreate() override {
        // Load resources once
        loadGameAssets();
    }
    
    void onEnter() override {
        // Set up UI (can be called multiple times)
        setupUI();
    }
    
    void onExit() override {
        // Clean up UI every time
        WidgetManager::getInstance().clear();
    }
    
    void onDestroy() override {
        // Free resources once
        unloadGameAssets();
    }
};
```

### 2. Consistent Navigation Patterns
```cpp
// Always provide a way back
void setupBackButton() {
    auto backBtn = Button(ButtonConfig(10, 10, 80, 30, "Back"));
    backBtn->onClick.connect([]() { popScene(); });
    addWidget(backBtn);
}

// Use replace for main navigation, push for overlays
void goToMainMenu() { replaceScene("MainMenu"); }    // Replace
void showPauseMenu() { pushScene("PauseMenu"); }     // Overlay
```

### 3. Error Handling
```cpp
void safeSceneTransition(const std::string& sceneName) {
    if (SceneManager::getInstance().getCurrentScene()) {
        pushScene(sceneName);
    } else {
        std::cerr << "No current scene to transition from!" << std::endl;
    }
}
```

### 4. Scene State Management
```cpp
class GameScene : public Scene {
private:
    struct GameState {
        int score;
        int level;
        float time;
    } state_;
    
public:
    void onExit() override {
        saveState(); // Preserve state when leaving
        WidgetManager::getInstance().clear();
    }
    
    void onEnter() override {
        loadState(); // Restore state when returning
        setupUI();
    }
};
```

## API Reference

### Scene Class

#### Constructor
```cpp
Scene(const std::string& name)
```

#### Virtual Methods
```cpp
virtual void onCreate()                  // Called once when scene is created
virtual void onEnter()                   // Called when scene becomes active
virtual void onExit()                    // Called when scene becomes inactive
virtual void onDestroy()                 // Called when scene is destroyed
virtual void update(float deltaTime)     // Called every frame while active
virtual void render()                    // Called every frame to draw
```

#### Methods
```cpp
const std::string& getName() const      // Get scene name
```

### SceneManager Class

#### Singleton Access
```cpp
static SceneManager& getInstance()
```

#### Scene Registration
```cpp
void registerScene(const std::string& name, std::function<std::unique_ptr<Scene>()> creator)
```

#### Navigation
```cpp
void pushScene(const std::string& name)     // Add scene to stack
void popScene()                             // Remove current scene
void replaceScene(const std::string& name)  // Replace current scene
void clearScenes()                          // Remove all scenes
```

#### Management
```cpp
void update(float deltaTime)                // Update current scene
void render()                              // Render current scene
Scene* getCurrentScene() const             // Get current scene
bool hasScenes() const                     // Check if any scenes exist
```

### Convenience Functions
```cpp
void pushScene(const std::string& name)     // Global helper
void popScene()                             // Global helper
void replaceScene(const std::string& name)  // Global helper
```

### Macros
```cpp
REGISTER_SCENE(name, sceneClass)            // Easy scene registration
```

## Common Patterns

### Loading Screen
```cpp
class LoadingScene : public Scene {
public:
    void onCreate() override {
        // Start loading in background thread
        std::thread([this]() {
            loadAllAssets();
            // Switch to main game when done
            replaceScene("MainGame");
        }).detach();
    }
    
    void render() override {
        Draw::fill(Colors::Black);
        // Show loading spinner/progress
        Scene::render();
    }
};
```

### Transition Effects
```cpp
class FadeTransitionScene : public Scene {
private:
    float fadeAmount = 0.0f;
    std::string targetScene;
    
public:
    void update(float deltaTime) override {
        fadeAmount += deltaTime * 2.0f; // 0.5 second fade
        if (fadeAmount >= 1.0f) {
            replaceScene(targetScene);
        }
    }
    
    void render() override {
        uint8_t alpha = static_cast<uint8_t>(fadeAmount * 255);
        Draw::fill(Colors::Black | (alpha << 24));
    }
};
```

---

The Scene Manager provides a robust foundation for organizing complex applications. By following these patterns and best practices, you can create smooth, maintainable navigation systems for your Fern applications.
