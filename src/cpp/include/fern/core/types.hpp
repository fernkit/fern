#pragma once

#include <cstdint>
#include <functional>
#include <vector>
#include <string>

namespace Fern {
    struct Point {
        int x = 0;
        int y = 0;
        
        Point() = default;
        Point(int x, int y) : x(x), y(y) {}
    };
    
    enum class KeyCode {
        None = 0,
        // Letters
        A = 65, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
        // Numbers
        Num0 = 48, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
        // Special keys
        Space = 32,
        Enter = 13,
        Backspace = 8,
        Delete = 46,
        Tab = 9,
        Escape = 27,
        ArrowLeft = 37,
        ArrowUp = 38,
        ArrowRight = 39,
        ArrowDown = 40,
        Shift = 16,
        Ctrl = 17,
        Alt = 18
    };

    struct InputState {
        // Mouse input
        int mouseX = 0;
        int mouseY = 0;
        bool mouseDown = false;
        bool mouseClicked = false;
        
        // Keyboard input
        KeyCode lastKeyPressed = KeyCode::None;
        KeyCode lastKeyReleased = KeyCode::None;
        bool keyPressed = false;
        bool keyReleased = false;
        
        // Text input (for typing)
        std::string textInput = "";
        bool hasTextInput = false;
        
        // Key state tracking
        bool isKeyDown(KeyCode key) const;
        bool isKeyJustPressed(KeyCode key) const;
        bool isKeyJustReleased(KeyCode key) const;
        
    private:
        mutable std::vector<KeyCode> pressedKeys_;
        mutable std::vector<KeyCode> justPressedKeys_;
        mutable std::vector<KeyCode> justReleasedKeys_;
        
        friend class Input;
    };
}