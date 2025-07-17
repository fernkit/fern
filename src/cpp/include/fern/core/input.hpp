/**
 * @file input.hpp
 * @brief Input management system for mouse and keyboard events
 * 
 * This file defines the Input class, which manages all input events from mouse
 * and keyboard. It maintains the current input state and provides methods for
 * updating input from platform-specific event handlers.
 */

#pragma once

#include "types.hpp"

namespace Fern {
    /**
     * @brief Central input management system
     * 
     * The Input class provides a centralized system for managing mouse and keyboard
     * input events. It maintains a global InputState that can be accessed by widgets
     * and other components for input handling.
     * 
     * The Input class is typically used by platform-specific event handlers to
     * update the input state, while widgets access the state for input processing.
     * 
     * @example Platform integration:
     * @code
     * // In platform-specific mouse event handler:
     * void onMouseMove(int x, int y) {
     *     Input::updateMousePosition(x, y);
     * }
     * 
     * void onMouseClick(bool down) {
     *     Input::updateMouseButton(down);
     * }
     * 
     * // In widget input handling:
     * bool MyWidget::handleInput(const InputState& input) {
     *     if (input.mouseClicked && contains(input.mouseX, input.mouseY)) {
     *         onClick();
     *         return true;
     *     }
     *     return false;
     * }
     * @endcode
     * 
     * @note This class is designed as a static interface for global input management
     */
    class Input {
    public:
        /**
         * @brief Get the current input state
         * 
         * Returns a reference to the current global input state containing
         * all mouse and keyboard information for this frame.
         * 
         * @return InputState& Reference to the current input state
         * 
         * @example
         * @code
         * const InputState& input = Input::getState();
         * if (input.mouseClicked) {
         *     handleMouseClick(input.mouseX, input.mouseY);
         * }
         * @endcode
         */
        static InputState& getState();
        
        /**
         * @brief Reset frame-specific input events
         * 
         * Clears events that should only last one frame (like clicks and key presses).
         * This should be called at the beginning of each frame by the main loop.
         * 
         * @note This preserves continuous states like mouse position and held keys
         */
        static void resetEvents();
        
        // Mouse input update methods
        
        /**
         * @brief Update the current mouse position
         * 
         * Updates the global mouse coordinates. This should be called from
         * platform-specific mouse move event handlers.
         * 
         * @param x New mouse X coordinate
         * @param y New mouse Y coordinate
         */
        static void updateMousePosition(int x, int y);
        
        /**
         * @brief Update mouse button state
         * 
         * Updates the mouse button press/release state. This should be called
         * from platform-specific mouse button event handlers.
         * 
         * @param down True if button is pressed, false if released
         * 
         * @note This automatically sets the mouseClicked flag for press events
         */
        static void updateMouseButton(bool down);
        
        // Keyboard input update methods
        
        /**
         * @brief Update keyboard key press
         * 
         * Records a key press event. This should be called from platform-specific
         * key press event handlers.
         * 
         * @param key The key that was pressed
         * 
         * @note This automatically updates the keyPressed flag and lastKeyPressed
         */
        static void updateKeyPress(KeyCode key);
        
        /**
         * @brief Update keyboard key release
         * 
         * Records a key release event. This should be called from platform-specific
         * key release event handlers.
         * 
         * @param key The key that was released
         * 
         * @note This automatically updates the keyReleased flag and lastKeyReleased
         */
        static void updateKeyRelease(KeyCode key);
        
        /**
         * @brief Update text input
         * 
         * Records text input for text fields and typing. This should be called
         * from platform-specific text input event handlers.
         * 
         * @param text The text that was entered (usually a single character)
         * 
         * @note This is separate from key presses and handles composed characters
         */
        static void updateTextInput(const std::string& text);
        
    private:
        static InputState state_;  ///< Global input state instance
    };
}