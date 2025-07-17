#pragma once

#include "../../core/types.hpp"
#include <functional>

namespace Fern {
    /**
     * @brief Base class for all UI widgets in the Fern framework
     * 
     * Widget provides the fundamental interface that all UI components must implement.
     * It defines basic positioning, sizing, rendering, and input handling capabilities
     * that are common to all widgets in the framework.
     * 
     * All concrete widgets inherit from this class and must implement:
     * - render(): Draw the widget to the screen
     * - handleInput(): Process user input events
     * 
     * @example Creating a custom widget:
     * @code
     * class MyWidget : public Widget {
     * public:
     *     void render() override {
     *         // Custom rendering logic
     *     }
     *     
     *     bool handleInput(const InputState& input) override {
     *         // Custom input handling
     *         return false; // or true if input was handled
     *     }
     * };
     * @endcode
     */
    class Widget {
    public:
        /**
         * @brief Default constructor
         */
        Widget() = default;
        
        /**
         * @brief Virtual destructor for proper cleanup
         */
        virtual ~Widget() = default;
        
        /**
         * @brief Render the widget to the screen
         * 
         * This pure virtual method must be implemented by all derived widgets
         * to define how they draw themselves to the canvas.
         */
        virtual void render() = 0;
        
        /**
         * @brief Handle user input events
         * 
         * This pure virtual method must be implemented by all derived widgets
         * to define how they respond to user input (mouse, keyboard, etc.).
         * 
         * @param input Current input state containing mouse/keyboard data
         * @return bool True if the widget handled the input, false otherwise
         */
        virtual bool handleInput(const InputState& input) = 0;
        
        /**
         * @brief Set the widget's position
         * @param x X coordinate in pixels
         * @param y Y coordinate in pixels
         */
        virtual void setPosition(int x, int y) { 
            x_ = x; 
            y_ = y; 
        }
        
        /**
         * @brief Get the widget's X position
         * @return int X coordinate in pixels
         */
        virtual int getX() const { return x_; }
        
        /**
         * @brief Get the widget's Y position
         * @return int Y coordinate in pixels
         */
        virtual int getY() const { return y_; }
        
        /**
         * @brief Resize the widget
         * @param width New width in pixels
         * @param height New height in pixels
         */
        virtual void resize(int width, int height) { 
            width_ = width; 
            height_ = height; 
        }
        
        /**
         * @brief Get the widget's width
         * @return int Width in pixels
         */
        virtual int getWidth() const { return width_; }
        
        /**
         * @brief Get the widget's height
         * @return int Height in pixels
         */
        virtual int getHeight() const { return height_; }
        
    protected:
        int x_ = 0;
        int y_ = 0;
        int width_ = 0;
        int height_ = 0;
    };
}