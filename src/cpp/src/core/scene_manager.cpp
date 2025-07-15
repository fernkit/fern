#include "../../include/fern/core/scene_manager.hpp"
#include <iostream>

namespace Fern {
    
    void SceneManager::registerScene(const std::string& name, std::function<std::unique_ptr<Scene>()> creator) {
        sceneCreators_[name] = creator;
        std::cout << "Scene registered: " << name << std::endl;
    }
    
    void SceneManager::pushScene(const std::string& name) {
        auto it = sceneCreators_.find(name);
        if (it == sceneCreators_.end()) {
            std::cerr << "Error: Scene '" << name << "' not found!" << std::endl;
            return;
        }
        
        // Pause current scene
        if (!sceneStack_.empty()) {
            sceneStack_.top()->onExit();
        }
        
        // Create and push new scene
        auto newScene = it->second();
        std::cout << "Pushing scene: " << name << std::endl;
        
        newScene->onCreate();
        sceneStack_.push(std::move(newScene));
        sceneStack_.top()->onEnter();
    }
    
    void SceneManager::popScene() {
        if (sceneStack_.empty()) {
            std::cerr << "Error: No scenes to pop!" << std::endl;
            return;
        }
        
        std::cout << "Popping scene: " << sceneStack_.top()->getName() << std::endl;
        
        // Exit and destroy current scene
        sceneStack_.top()->onExit();
        sceneStack_.top()->onDestroy();
        sceneStack_.pop();
        
        // Resume previous scene
        if (!sceneStack_.empty()) {
            sceneStack_.top()->onEnter();
        }
    }
    
    void SceneManager::replaceScene(const std::string& name) {
        auto it = sceneCreators_.find(name);
        if (it == sceneCreators_.end()) {
            std::cerr << "Error: Scene '" << name << "' not found!" << std::endl;
            return;
        }
        
        // Clear current scene
        if (!sceneStack_.empty()) {
            sceneStack_.top()->onExit();
            sceneStack_.top()->onDestroy();
            sceneStack_.pop();
        }
        
        // Create and set new scene
        auto newScene = it->second();
        std::cout << "Replacing with scene: " << name << std::endl;
        
        newScene->onCreate();
        sceneStack_.push(std::move(newScene));
        sceneStack_.top()->onEnter();
    }
    
    void SceneManager::clearScenes() {
        while (!sceneStack_.empty()) {
            sceneStack_.top()->onExit();
            sceneStack_.top()->onDestroy();
            sceneStack_.pop();
        }
        std::cout << "All scenes cleared" << std::endl;
    }
    
    void SceneManager::update(float deltaTime) {
        if (!sceneStack_.empty()) {
            sceneStack_.top()->update(deltaTime);
        }
    }
    
    void SceneManager::render() {
        if (!sceneStack_.empty()) {
            sceneStack_.top()->render();
        }
    }
    
    Scene* SceneManager::getCurrentScene() const {
        if (sceneStack_.empty()) {
            return nullptr;
        }
        return sceneStack_.top().get();
    }
    
    bool SceneManager::hasScenes() const {
        return !sceneStack_.empty();
    }
    
} // namespace Fern
