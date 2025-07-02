#pragma once

#include "types.hpp"

namespace Fern {
    class Input {
    public:
        static InputState& getState();
        static void resetEvents();
        
        // Mouse input methods
        static void updateMousePosition(int x, int y);
        static void updateMouseButton(bool down);
        
        // Keyboard input methods
        static void updateKeyPress(KeyCode key);
        static void updateKeyRelease(KeyCode key);
        static void updateTextInput(const std::string& text);
        
    private:
        static InputState state_;
    };
}