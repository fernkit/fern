/**
 * @file fern.hpp
 * @brief Main header file for the Fern UI Framework
 * 
 * This file includes all the core components of the Fern framework, providing
 * a single header for easy integration. Fern is a lightweight, cross-platform
 * UI framework designed for creating interactive applications with pixel-perfect
 * rendering and modern C++ design.
 * 
 * @author Fern Development Team
 * @version 1.0
 */

#pragma once

#include "core/canvas.hpp"
#include "core/input.hpp"
#include "core/scene_manager.hpp"
#include "graphics/primitives.hpp"
#include "graphics/colors.hpp"
#include "text/font.hpp"
#include "ui/widgets/text_widget.hpp"
#include "ui/widgets/circle_widget.hpp"
#include "ui/widgets/line_widget.hpp"
#include "ui/widgets/button_widget.hpp"
#include "ui/widgets/text_input_widget.hpp"
#include "ui/widgets/slider_widget.hpp"
#include "ui/widgets/radio_button_widget.hpp"
#include "ui/widgets/dropdown_widget.hpp"
#include "ui/widgets/circular_indicator_widget.hpp"
#include "ui/widgets/progress_bar_widget.hpp"
#include "core/widget_manager.hpp"
#include "ui/layout/layout.hpp"
#include "ui/containers/container.hpp"

/**
 * @namespace Fern
 * @brief Main namespace for the Fern UI Framework
 * 
 * All Fern classes, functions, and constants are contained within this namespace.
 * This prevents naming conflicts and provides clear organization of the API.
 */
namespace Fern {
    /**
     * @brief Initialize Fern with a custom pixel buffer
     * 
     * Initializes the Fern framework with a user-provided pixel buffer.
     * This allows for custom rendering targets and integration with existing graphics systems.
     * 
     * @param pixelBuffer Pointer to RGBA pixel buffer (32-bit per pixel)
     * @param width Width of the canvas in pixels
     * @param height Height of the canvas in pixels
     * 
     * @warning The pixel buffer must remain valid for the lifetime of the application
     * @note Each pixel is represented as a 32-bit RGBA value (0xAABBGGRR)
     */
    void initialize(uint32_t* pixelBuffer, int width, int height);

    /**
     * @brief Initialize Fern with automatic platform-specific rendering
     * 
     * Initializes Fern with platform-specific default rendering. The framework
     * will create its own render target and manage the display automatically.
     * This is the recommended initialization method for most applications.
     */
    void initialize();
    
    /**
     * @brief Initialize Fern with specified dimensions
     * 
     * Initializes Fern with platform-specific rendering using custom dimensions.
     * The framework will create its own render target with the specified size.
     * 
     * @param width Desired canvas width in pixels
     * @param height Desired canvas height in pixels
     */
    void initialize(int width, int height); 

    /**
     * @brief Get the current canvas width
     * 
     * @return int Current canvas width in pixels
     */
    int getWidth();
    
    /**
     * @brief Get the current canvas height
     * 
     * @return int Current canvas height in pixels
     */
    int getHeight();
    
    /**
     * @brief Get the current canvas size as a Point
     * 
     * @return Point Canvas dimensions as Point(width, height)
     */
    Point getCanvasSize();

    /**
     * @brief Start the main render loop
     * 
     * Begins the main application loop, handling events and rendering.
     * This function will block until the application is closed.
     * Set up your draw callback before calling this function.
     * 
     * @see setDrawCallback()
     */
    void startRenderLoop();
    
    /**
     * @brief Set the draw callback function
     * 
     * Sets the function that will be called every frame to render the application.
     * This callback should contain all rendering logic for your application.
     * 
     * @param callback Function to call each frame for rendering
     * 
     * @example
     * @code
     * Fern::setDrawCallback([]() {
     *     Draw::fill(Colors::Black);
     *     // Render your UI here
     *     WidgetManager::getInstance().renderAll();
     * });
     * @endcode
     */
    void setDrawCallback(std::function<void()> callback);

    /**
     * @brief Set the window resize callback function
     * 
     * Sets the function that will be called when the window is resized.
     * Use this to update layouts, reposition widgets, or handle responsive design.
     * 
     * @param callback Function to call when window is resized, receives new width and height
     * 
     * @example
     * @code
     * Fern::setWindowResizeCallback([](int newWidth, int newHeight) {
     *     // Handle window resize
     *     WidgetManager::getInstance().handleWindowResize(newWidth, newHeight);
     * });
     * @endcode
     */
    void setWindowResizeCallback(std::function<void(int, int)> callback);
}