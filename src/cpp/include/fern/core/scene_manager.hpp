#pragma once
#include <memory>
#include <stack>
#include <unordered_map>
#include <string>
#include <functional>
#include "widget_manager.hpp"

namespace Fern {
    
    /**
     * @brief Base class for all scenes in the application
     * 
     * A Scene represents a distinct screen or state in your application (e.g., Menu, Game, Settings).
     * Each scene manages its own UI widgets and handles its own update and render logic.
     * 
     * The Scene class provides lifecycle methods that are automatically called by the SceneManager:
     * - onCreate(): Called once when the scene is first created
     * - onEnter(): Called every time the scene becomes active
     * - onExit(): Called when the scene becomes inactive
     * - onDestroy(): Called when the scene is permanently removed
     * 
     * @example Basic scene implementation:
     * @code
     * class MenuScene : public Scene {
     * public:
     *     MenuScene() : Scene("MenuScene") {}
     *     
     *     void onEnter() override {
     *         setupUI();
     *     }
     *     
     *     void onExit() override {
     *         WidgetManager::getInstance().clearAll();
     *     }
     *     
     *     void render() override {
     *         Draw::fill(Colors::DarkBlue);
     *         Scene::render(); // Renders all widgets
     *     }
     * };
     * @endcode
     */
    class Scene {
    public:
        /**
         * @brief Construct a new Scene with the given name
         * 
         * @param name Unique identifier for this scene
         */
        Scene(const std::string& name) : name_(name) {}
        
        /**
         * @brief Virtual destructor
         */
        virtual ~Scene() = default;
        
        /**
         * @brief Called once when the scene is first created
         * 
         * Use this method to initialize data, load resources, or perform
         * one-time setup that doesn't depend on UI widgets.
         * 
         * @note This is called before onEnter() and only once per scene instance
         */
        virtual void onCreate() {}
        
        /**
         * @brief Called every time the scene becomes active
         * 
         * Use this method to set up UI widgets, start animations, or
         * perform setup that should happen each time the scene is shown.
         * 
         * @note This can be called multiple times if the scene is pushed/popped
         */
        virtual void onEnter() {}
        
        /**
         * @brief Called when the scene becomes inactive
         * 
         * Use this method to clean up UI widgets, pause animations, or
         * save temporary state. The scene may become active again later.
         * 
         * @note Always clean up widgets here to prevent memory leaks
         */
        virtual void onExit() {}
        
        /**
         * @brief Called when the scene is permanently removed
         * 
         * Use this method to free resources, save permanent state, or
         * perform final cleanup. The scene will not be used again.
         * 
         * @note This is called after onExit() and only once per scene instance
         */
        virtual void onDestroy() {}
        
        /**
         * @brief Called every frame while the scene is active
         * 
         * Use this method to update game logic, animations, or any
         * time-dependent behavior.
         * 
         * @param deltaTime Time elapsed since the last update in seconds
         */
        virtual void update(float deltaTime) {}
        
        /**
         * @brief Called every frame to render the scene
         * 
         * Use this method to draw backgrounds, game objects, or custom graphics.
         * Call Scene::render() at the end to render all managed widgets.
         * 
         * @note The default implementation renders all widgets using WidgetManager
         */
        virtual void render() {
            // Default implementation renders all managed widgets
            WidgetManager::getInstance().renderAll();
        }
        
        /**
         * @brief Get the name of this scene
         * 
         * @return const std::string& The scene's unique identifier
         */
        const std::string& getName() const { return name_; }
        
    protected:
        std::string name_;  ///< Unique identifier for this scene
    };
    
    /**
     * @brief Manages scene navigation and lifecycle in a stack-based system
     * 
     * The SceneManager provides a powerful system for organizing your application
     * into discrete scenes or screens. It maintains a stack of scenes and handles
     * automatic lifecycle management, navigation, and state preservation.
     * 
     * Key features:
     * - Stack-based navigation (push/pop/replace)
     * - Automatic lifecycle management
     * - Scene registration and factory system
     * - Overlay support for modals and pause screens
     * 
     * @example Basic usage:
     * @code
     * // Register scenes
     * REGISTER_SCENE("Menu", MenuScene);
     * REGISTER_SCENE("Game", GameScene);
     * 
     * // Navigate between scenes
     * SceneManager::getInstance().pushScene("Menu");
     * SceneManager::getInstance().pushScene("Game");  // Menu pauses
     * SceneManager::getInstance().popScene();         // Back to Menu
     * @endcode
     * 
     * @note This class is a singleton - use getInstance() to access it
     */
    class SceneManager {
    public:
        /**
         * @brief Get the singleton instance of SceneManager
         * 
         * @return SceneManager& Reference to the singleton instance
         */
        static SceneManager& getInstance() {
            static SceneManager instance;
            return instance;
        }
        
        /**
         * @brief Register a scene factory function
         * 
         * Register a factory function that creates instances of a scene.
         * The factory is called each time the scene needs to be created.
         * 
         * @param name Unique name for the scene
         * @param creator Factory function that returns a new scene instance
         * 
         * @example Manual registration:
         * @code
         * SceneManager::getInstance().registerScene("Menu", []() -> std::unique_ptr<Scene> {
         *     return std::make_unique<MenuScene>();
         * });
         * @endcode
         * 
         * @see REGISTER_SCENE macro for easier registration
         */
        void registerScene(const std::string& name, std::function<std::unique_ptr<Scene>()> creator);
        
        /**
         * @brief Push a new scene onto the stack
         * 
         * Creates a new instance of the named scene and pushes it onto the stack.
         * The current scene (if any) will call onExit() and pause, while the
         * new scene will call onCreate() and onEnter().
         * 
         * @param name Name of the scene to push (must be registered)
         * 
         * @note The current scene remains in memory and can be returned to with popScene()
         */
        void pushScene(const std::string& name);
        
        /**
         * @brief Pop the current scene from the stack
         * 
         * Removes the current scene from the stack, calling onExit() and onDestroy().
         * If there was a previous scene, it will call onEnter() and resume.
         * 
         * @warning Does nothing if the stack is empty
         */
        void popScene();
        
        /**
         * @brief Replace the current scene with a new one
         * 
         * Removes the current scene (calling onExit() and onDestroy()) and
         * pushes a new scene onto the stack. This is useful for main navigation
         * where you don't want to return to the previous scene.
         * 
         * @param name Name of the scene to replace with (must be registered)
         */
        void replaceScene(const std::string& name);
        
        /**
         * @brief Remove all scenes from the stack
         * 
         * Calls onExit() and onDestroy() on all scenes in the stack,
         * leaving the application with no active scene.
         * 
         * @warning Use carefully - the application will have no UI after this call
         */
        void clearScenes();
        
        /**
         * @brief Update the current scene
         * 
         * Calls update() on the current scene (if any). This should be called
         * every frame from your main loop.
         * 
         * @param deltaTime Time elapsed since the last update in seconds
         */
        void update(float deltaTime);
        
        /**
         * @brief Render the current scene
         * 
         * Calls render() on the current scene (if any). This should be called
         * every frame from your draw callback.
         */
        void render();
        
        /**
         * @brief Get a pointer to the current scene
         * 
         * @return Scene* Pointer to the current scene, or nullptr if no scenes are active
         */
        Scene* getCurrentScene() const;
        
        /**
         * @brief Check if there are any scenes in the stack
         * 
         * @return true if there are scenes in the stack, false otherwise
         */
        bool hasScenes() const;
        
    private:
        /**
         * @brief Private constructor for singleton pattern
         */
        SceneManager() = default;
        
        std::unordered_map<std::string, std::function<std::unique_ptr<Scene>()>> sceneCreators_;  ///< Registered scene factories
        std::stack<std::unique_ptr<Scene>> sceneStack_;  ///< Stack of active scenes
        
        /**
         * @brief Internal method to handle scene transitions
         * 
         * @param newScene The scene to transition to
         */
        void transitionToScene(std::unique_ptr<Scene> newScene);
    };
    
    /**
     * @brief Convenience macro for scene registration
     * 
     * Registers a scene class with the SceneManager using the class constructor.
     * This is the recommended way to register scenes.
     * 
     * @param name String name for the scene
     * @param sceneClass Class name of the scene (must inherit from Scene)
     * 
     * @example
     * @code
     * REGISTER_SCENE("Menu", MenuScene);
     * REGISTER_SCENE("Game", GameScene);
     * REGISTER_SCENE("Settings", SettingsScene);
     * @endcode
     */
    #define REGISTER_SCENE(name, sceneClass) \
        SceneManager::getInstance().registerScene(name, []() -> std::unique_ptr<Scene> { \
            return std::make_unique<sceneClass>(); \
        })
    
    /**
     * @brief Convenience function to push a scene
     * 
     * @param name Name of the scene to push
     * @see SceneManager::pushScene()
     */
    inline void pushScene(const std::string& name) {
        SceneManager::getInstance().pushScene(name);
    }
    
    /**
     * @brief Convenience function to pop the current scene
     * 
     * @see SceneManager::popScene()
     */
    inline void popScene() {
        SceneManager::getInstance().popScene();
    }
    
    /**
     * @brief Convenience function to replace the current scene
     * 
     * @param name Name of the scene to replace with
     * @see SceneManager::replaceScene()
     */
    inline void replaceScene(const std::string& name) {
        SceneManager::getInstance().replaceScene(name);
    }
    
} // namespace Fern