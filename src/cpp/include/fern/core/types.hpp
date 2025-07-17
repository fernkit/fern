#pragma once

#include <cstdint>
#include <functional>
#include <vector>
#include <string>

namespace Fern {
    /**
     * @brief 2D point with integer coordinates
     * 
     * Point represents a position in 2D space with x and y coordinates.
     * Commonly used for widget positioning and mouse coordinates.
     * 
     * @example Creating and using points:
     * @code
     * Point origin;              // (0, 0)
     * Point position(100, 50);   // (100, 50)
     * auto circle = Circle(25, position, Colors::Red);
     * @endcode
     */
    struct Point {
        int x = 0;  ///< X coordinate
        int y = 0;  ///< Y coordinate
        
        Point() = default;
        Point(int x, int y) : x(x), y(y) {}
    };
    
    /**
     * @brief Enumeration of keyboard key codes
     * 
     * KeyCode provides constants for keyboard keys used in input handling.
     * Values correspond to standard ASCII/virtual key codes.
     */
    enum class KeyCode {
        None = 0,  ///< No key
        // Letters
        A = 65, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
        // Numbers
        Num0 = 48, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
        // Special keys
        Space = 32,       ///< Space bar
        Enter = 13,       ///< Enter/Return key
        Backspace = 8,    ///< Backspace key
        Delete = 46,      ///< Delete key
        Tab = 9,          ///< Tab key
        Escape = 27,      ///< Escape key
        ArrowLeft = 37,   ///< Left arrow key
        ArrowUp = 38,     ///< Up arrow key
        ArrowRight = 39,  ///< Right arrow key
        ArrowDown = 40,   ///< Down arrow key
        Shift = 16,       ///< Shift key
        Ctrl = 17,        ///< Control key
        Alt = 18          ///< Alt key
    };

    /**
     * @brief Complete input state for mouse and keyboard
     * 
     * InputState contains all current input information including mouse position,
     * button states, keyboard input, and text input. It's passed to widgets
     * for input handling.
     * 
     * @example Checking input in widget:
     * @code
     * bool MyWidget::handleInput(const InputState& input) {
     *     if (input.mouseClicked && isPointInside(input.mouseX, input.mouseY)) {
     *         // Handle click
     *         return true;
     *     }
     *     return false;
     * }
     * @endcode
     */
    struct InputState {
        // Mouse input
        int mouseX = 0;          ///< Current mouse X position
        int mouseY = 0;          ///< Current mouse Y position
        bool mouseDown = false;  ///< True if mouse button is currently pressed
        bool mouseClicked = false; ///< True if mouse was clicked this frame
        
        // Keyboard input
        KeyCode lastKeyPressed = KeyCode::None;   ///< Last key that was pressed
        KeyCode lastKeyReleased = KeyCode::None;  ///< Last key that was released
        bool keyPressed = false;   ///< True if any key was pressed this frame
        bool keyReleased = false;  ///< True if any key was released this frame
        
        // Text input (for typing)
        std::string textInput = "";  ///< Text entered this frame (for text fields)
        bool hasTextInput = false;   ///< True if text was entered this frame
        
        /**
         * @brief Check if a specific key is currently held down
         * 
         * @param key Key to check
         * @return true if key is currently pressed
         */
        bool isKeyDown(KeyCode key) const;
        
        /**
         * @brief Check if a specific key was just pressed this frame
         * 
         * @param key Key to check
         * @return true if key was just pressed
         */
        bool isKeyJustPressed(KeyCode key) const;
        
        /**
         * @brief Check if a specific key was just released this frame
         * 
         * @param key Key to check
         * @return true if key was just released
         */
        bool isKeyJustReleased(KeyCode key) const;
        
    private:
        mutable std::vector<KeyCode> pressedKeys_;      ///< Currently pressed keys
        mutable std::vector<KeyCode> justPressedKeys_;  ///< Keys pressed this frame
        mutable std::vector<KeyCode> justReleasedKeys_; ///< Keys released this frame
        
        friend class Input;  ///< Input class can access private members
    };
}