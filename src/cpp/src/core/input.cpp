#include "../../include/fern/core/input.hpp"
#include <algorithm>

namespace Fern {
    InputState Input::state_ = {};
    
    InputState& Input::getState() {
        return state_;
    }
    
    void Input::resetEvents() {
        state_.mouseClicked = false;
        state_.keyPressed = false;
        state_.keyReleased = false;
        state_.hasTextInput = false;
        state_.textInput.clear();
        state_.lastKeyPressed = KeyCode::None;
        state_.lastKeyReleased = KeyCode::None;
        
        // Clear just pressed/released keys but keep current pressed keys
        state_.justPressedKeys_.clear();
        state_.justReleasedKeys_.clear();
    }
    
    void Input::updateMousePosition(int x, int y) {
        state_.mouseX = x;
        state_.mouseY = y;
    }
    
    void Input::updateMouseButton(bool down) {
        if (!state_.mouseDown && down) {
            state_.mouseClicked = true;
        }
        state_.mouseDown = down;
    }
    
    void Input::updateKeyPress(KeyCode key) {
        state_.lastKeyPressed = key;
        state_.keyPressed = true;
        
        // Add to pressed keys if not already pressed
        if (std::find(state_.pressedKeys_.begin(), state_.pressedKeys_.end(), key) == state_.pressedKeys_.end()) {
            state_.pressedKeys_.push_back(key);
            state_.justPressedKeys_.push_back(key);
        }
    }
    
    void Input::updateKeyRelease(KeyCode key) {
        state_.lastKeyReleased = key;
        state_.keyReleased = true;
        
        // Remove from pressed keys
        auto it = std::find(state_.pressedKeys_.begin(), state_.pressedKeys_.end(), key);
        if (it != state_.pressedKeys_.end()) {
            state_.pressedKeys_.erase(it);
            state_.justReleasedKeys_.push_back(key);
        }
    }
    
    void Input::updateTextInput(const std::string& text) {
        state_.textInput = text;
        state_.hasTextInput = true;
    }
    
    // InputState method implementations
    bool InputState::isKeyDown(KeyCode key) const {
        return std::find(pressedKeys_.begin(), pressedKeys_.end(), key) != pressedKeys_.end();
    }
    
    bool InputState::isKeyJustPressed(KeyCode key) const {
        return std::find(justPressedKeys_.begin(), justPressedKeys_.end(), key) != justPressedKeys_.end();
    }
    
    bool InputState::isKeyJustReleased(KeyCode key) const {
        return std::find(justReleasedKeys_.begin(), justReleasedKeys_.end(), key) != justReleasedKeys_.end();
    }
}