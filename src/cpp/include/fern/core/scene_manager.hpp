#pragma once
#include <memory>
#include <stack>
#include <unordered_map>
#include <string>
#include <functional>
#include "widget_manager.hpp"

namespace Fern {
    
    class Scene {
    public:
        Scene(const std::string& name) : name_(name) {}
        virtual ~Scene() = default;
        
        // Scene lifecycle methods
        virtual void onCreate() {}
        virtual void onEnter() {}
        virtual void onExit() {}
        virtual void onDestroy() {}
        
        // Scene update and render
        virtual void update(float deltaTime) {}
        virtual void render() {
            // Default implementation renders all managed widgets
            WidgetManager::getInstance().renderAll();
        }
        
        const std::string& getName() const { return name_; }
        
    protected:
        std::string name_;
    };
    
    class SceneManager {
    public:
        static SceneManager& getInstance() {
            static SceneManager instance;
            return instance;
        }
        
        // Scene registration
        void registerScene(const std::string& name, std::function<std::unique_ptr<Scene>()> creator);
        
        // Scene navigation
        void pushScene(const std::string& name);
        void popScene();
        void replaceScene(const std::string& name);
        void clearScenes();
        
        // Scene management
        void update(float deltaTime);
        void render();
        
        // Utility
        Scene* getCurrentScene() const;
        bool hasScenes() const;
        
    private:
        SceneManager() = default;
        
        std::unordered_map<std::string, std::function<std::unique_ptr<Scene>()>> sceneCreators_;
        std::stack<std::unique_ptr<Scene>> sceneStack_;
        
        void transitionToScene(std::unique_ptr<Scene> newScene);
    };
    
    // Convenience macros for scene registration
    #define REGISTER_SCENE(name, sceneClass) \
        SceneManager::getInstance().registerScene(name, []() -> std::unique_ptr<Scene> { \
            return std::make_unique<sceneClass>(); \
        })
    
    // Helper functions
    inline void pushScene(const std::string& name) {
        SceneManager::getInstance().pushScene(name);
    }
    
    inline void popScene() {
        SceneManager::getInstance().popScene();
    }
    
    inline void replaceScene(const std::string& name) {
        SceneManager::getInstance().replaceScene(name);
    }
    
} // namespace Fern
