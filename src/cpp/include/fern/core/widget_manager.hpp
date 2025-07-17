#pragma once
#include "../ui/widgets/widget.hpp"
#include "../core/input.hpp"
#include "responsive_widget.hpp"
#include <vector>
#include <memory>
#include <algorithm>

// Forward declarations
namespace Fern {
    int getWidth();
    int getHeight();
}

namespace Fern
{
    /**
     * @brief Manages all widgets in the application
     * 
     * The WidgetManager is responsible for maintaining a collection of widgets,
     * handling their input events, rendering them in the correct order, and
     * managing their lifecycle. It provides centralized widget management
     * with proper Z-order handling and responsive layout support.
     * 
     * Key features:
     * - Automatic widget lifecycle management
     * - Proper Z-order input handling (top widgets receive input first)
     * - Responsive layout support with window resize handling
     * - Singleton pattern for global access
     * - Memory management with shared_ptr
     * 
     * @example Basic usage:
     * @code
     * auto button = Button(ButtonConfig(100, 100, 120, 40, "Click Me"));
     * WidgetManager::getInstance().addWidget(button);
     * 
     * // In your main loop:
     * WidgetManager::getInstance().updateAll(inputState);
     * WidgetManager::getInstance().renderAll();
     * @endcode
     * 
     * @note This class is a singleton - use getInstance() to access it
     * @note Widgets are rendered in the order they were added (first added = bottom layer)
     * @note Input is handled in reverse order (last added = top layer receives input first)
     */
    class WidgetManager {
    public: 
        /**
         * @brief Get the singleton instance of WidgetManager
         * 
         * @return WidgetManager& Reference to the singleton instance
         */
        static WidgetManager& getInstance(){
            static WidgetManager instance;
            return instance;
        }

        /**
         * @brief Add a widget to the manager
         * 
         * Adds a widget to the collection. The widget will be rendered and
         * can receive input events. Widgets are rendered in the order they
         * are added (first added appears at the bottom).
         * 
         * @param widget Shared pointer to the widget to add
         * 
         * @example
         * @code
         * auto button = Button(ButtonConfig(0, 0, 100, 30, "Click"));
         * WidgetManager::getInstance().addWidget(button);
         * @endcode
         */
        void addWidget(std::shared_ptr<Widget> widget){
             widgets_.push_back(widget);
        }

        /**
         * @brief Remove a specific widget from the manager
         * 
         * Removes the specified widget from the collection. The widget
         * will no longer be rendered or receive input events.
         * 
         * @param widget Shared pointer to the widget to remove
         * 
         * @note If the widget is not found, this method does nothing
         */
        void removeWidget(std::shared_ptr<Widget> widget) {
            widgets_.erase(
                std::remove_if(widgets_.begin(), widgets_.end(),
                    [&widget](const auto& w) { return w == widget; }),
                widgets_.end()
            );
        }

        /**
         * @brief Update all widgets with input events
         * 
         * Processes input events for all widgets in reverse order (Z-order).
         * The last added widget (topmost) receives input first. Once a widget
         * handles the input, no widgets below it will receive the event.
         * 
         * @param input Current input state containing mouse and keyboard info
         * 
         * @note Call this every frame before rendering to handle user interaction
         * @note Input propagation stops at the first widget that handles the event
         */
        void updateAll(const InputState& input) {
            bool inputHandled = false;
            for (auto it = widgets_.rbegin(); it != widgets_.rend(); ++it) {
                if (!inputHandled) {
                    inputHandled = (*it)->handleInput(input);
                }
            }
        }

        /**
         * @brief Render all widgets
         * 
         * Renders all widgets in the order they were added (first added = bottom layer).
         * This creates the proper visual layering where newer widgets appear on top.
         * 
         * @note Call this every frame after updating to draw all widgets
         * @note Widgets are responsible for their own rendering logic
         */
         void renderAll() {
            for (auto& widget : widgets_) {
                widget->render();
            }
        }

        /**
         * @brief Remove all widgets from the manager
         * 
         * Clears the entire widget collection. All widgets will be removed
         * and will no longer be rendered or receive input events.
         * 
         * @note This is useful for scene transitions or cleanup
         * @note The widgets themselves are not destroyed if other references exist
         */
        void clear() {
            widgets_.clear();
        }

        /**
         * @brief Handle window resize events for responsive widgets
         * 
         * Notifies all widgets that implement ResponsiveWidget interface
         * about window size changes. This allows widgets to adjust their
         * layout and positioning automatically.
         * 
         * @param newWidth New window width in pixels
         * @param newHeight New window height in pixels
         * 
         * @note Only widgets that inherit from ResponsiveWidget will be notified
         */
        void onWindowResize(int newWidth, int newHeight) {
            // Notify all widgets about the resize
            for (auto& widget : widgets_) {
                // Check if widget has responsive capabilities
                auto responsiveWidget = dynamic_cast<ResponsiveWidget*>(widget.get());
                if (responsiveWidget) {
                    responsiveWidget->onWindowResize(newWidth, newHeight);
                }
            }
        }
        
        /**
         * @brief Refresh layout for all responsive widgets
         * 
         * Triggers a layout refresh using the current canvas dimensions.
         * This is useful when you need to force a layout update without
         * an actual window resize event.
         */
        void refreshLayout() { 
            // Use external canvas dimensions
            onWindowResize(getWidth(), getHeight()); 
        }
        
        /**
         * @brief Get the current canvas width
         * 
         * @return int Current canvas width in pixels
         * @deprecated Use Fern::getWidth() instead for accurate dimensions
         */
        int getCanvasWidth() const { return 800; }  // Will be updated by external calls
        
        /**
         * @brief Get the current canvas height
         * 
         * @return int Current canvas height in pixels  
         * @deprecated Use Fern::getHeight() instead for accurate dimensions
         */
        int getCanvasHeight() const { return 600; } // Will be updated by external calls

    private:
        /**
         * @brief Private constructor for singleton pattern
         */
        WidgetManager() = default;
        
        std::vector<std::shared_ptr<Widget>> widgets_;  ///< Collection of managed widgets
    };

    /**
     * @brief Convenience function to add a widget
     * 
     * Global helper function that adds a widget to the default WidgetManager instance.
     * 
     * @param widget Shared pointer to the widget to add
     * 
     * @example
     * @code
     * auto button = Button(ButtonConfig(0, 0, 100, 30, "Click"));
     * addWidget(button);  // Equivalent to WidgetManager::getInstance().addWidget(button)
     * @endcode
     */
    inline void addWidget(std::shared_ptr<Widget> widget) {
        WidgetManager::getInstance().addWidget(widget);
    }
    
    /**
     * @brief Convenience function to remove a widget
     * 
     * Global helper function that removes a widget from the default WidgetManager instance.
     * 
     * @param widget Shared pointer to the widget to remove
     */
    inline void removeWidget(std::shared_ptr<Widget> widget) {
        WidgetManager::getInstance().removeWidget(widget);
    }
} // namespace Fern
