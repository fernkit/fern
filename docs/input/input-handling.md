# Input Handling

The Fern Input Handling system provides comprehensive support for processing user input including mouse, keyboard, and text input events. It offers both low-level input state access and high-level widget integration.

## Overview

The input system consists of:
- **InputState**: Current state of all input devices
- **Input**: Static class for managing and updating input state
- **KeyCode**: Enumeration of supported keys
- **Event handling**: Integration with widget event systems

## Input State

### InputState Structure
```cpp
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
    
    // Text input
    std::string textInput = "";
    bool hasTextInput = false;
    
    // Key state queries
    bool isKeyDown(KeyCode key) const;
    bool isKeyJustPressed(KeyCode key) const;
    bool isKeyJustReleased(KeyCode key) const;
};
```

### Accessing Input State
```cpp
#include "fern/fern.hpp"
using namespace Fern;

int main() {
    initWindow(800, 600, "Input Handling Example");
    
    while (isRunning()) {
        clear();
        
        // Get current input state
        const InputState& input = Input::getState();
        
        // Check mouse position
        printf("Mouse: (%d, %d)\n", input.mouseX, input.mouseY);
        
        // Check mouse button
        if (input.mouseClicked) {
            printf("Mouse clicked at (%d, %d)\n", input.mouseX, input.mouseY);
        }
        
        // Check keyboard
        if (input.keyPressed) {
            printf("Key pressed: %d\n", (int)input.lastKeyPressed);
        }
        
        updateWidgets();
        present();
    }
    
    return 0;
}
```

## Key Codes

### Available Keys
```cpp
enum class KeyCode {
    None = 0,
    
    // Letters (A-Z)
    A = 65, B, C, D, E, F, G, H, I, J, K, L, M, 
    N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    
    // Numbers (0-9)
    Num0 = 48, Num1, Num2, Num3, Num4, Num5, 
    Num6, Num7, Num8, Num9,
    
    // Special keys
    Space = 32,
    Enter = 13,
    Backspace = 8,
    Delete = 46,
    Tab = 9,
    Escape = 27,
    
    // Arrow keys
    ArrowLeft = 37,
    ArrowUp = 38,
    ArrowRight = 39,
    ArrowDown = 40,
    
    // Modifier keys
    Shift = 16,
    Ctrl = 17,
    Alt = 18
};
```

## Mouse Input

### Basic Mouse Handling
```cpp
#include "fern/fern.hpp"
using namespace Fern;

int main() {
    initWindow(800, 600, "Mouse Input Example");
    
    std::vector<Point> clickPoints;
    
    while (isRunning()) {
        clear();
        
        const InputState& input = Input::getState();
        
        // Track mouse clicks
        if (input.mouseClicked) {
            clickPoints.push_back(Point(input.mouseX, input.mouseY));
        }
        
        // Draw mouse cursor
        Draw::circle(input.mouseX, input.mouseY, 5, Colors::Red);
        
        // Draw click points
        for (const auto& point : clickPoints) {
            Draw::circle(point.x, point.y, 3, Colors::Blue);
        }
        
        // Clear on right click (if supported)
        if (input.mouseClicked && input.mouseX > 400) {
            clickPoints.clear();
        }
        
        present();
    }
    
    return 0;
}
```

### Mouse Movement Tracking
```cpp
class MouseTracker {
private:
    Point lastPosition_;
    std::vector<Point> trail_;
    
public:
    void update(const InputState& input) {
        Point currentPos(input.mouseX, input.mouseY);
        
        // Add to trail if mouse moved
        if (currentPos.x != lastPosition_.x || currentPos.y != lastPosition_.y) {
            trail_.push_back(currentPos);
            
            // Limit trail length
            if (trail_.size() > 50) {
                trail_.erase(trail_.begin());
            }
        }
        
        lastPosition_ = currentPos;
    }
    
    void render() {
        // Draw trail
        for (size_t i = 1; i < trail_.size(); i++) {
            uint32_t alpha = (255 * i) / trail_.size();
            uint32_t color = (alpha << 24) | 0x00FF0000; // Red with fade
            
            Draw::line(trail_[i-1].x, trail_[i-1].y, 
                      trail_[i].x, trail_[i].y, 2, color);
        }
    }
};
```

## Keyboard Input

### Basic Keyboard Handling
```cpp
#include "fern/fern.hpp"
using namespace Fern;

int main() {
    initWindow(800, 600, "Keyboard Input Example");
    
    std::string typedText = "";
    
    while (isRunning()) {
        clear();
        
        const InputState& input = Input::getState();
        
        // Handle text input
        if (input.hasTextInput) {
            typedText += input.textInput;
        }
        
        // Handle special keys
        if (input.isKeyJustPressed(KeyCode::Backspace)) {
            if (!typedText.empty()) {
                typedText.pop_back();
            }
        }
        
        if (input.isKeyJustPressed(KeyCode::Enter)) {
            typedText += "\n";
        }
        
        if (input.isKeyJustPressed(KeyCode::Escape)) {
            typedText.clear();
        }
        
        // Display typed text
        Font::renderText(getCanvas(), typedText, 50, 50, 16, Colors::Black);
        
        present();
    }
    
    return 0;
}
```

### Key State Queries
```cpp
void handleMovement(const InputState& input) {
    static int playerX = 400;
    static int playerY = 300;
    const int speed = 5;
    
    // Continuous movement (held keys)
    if (input.isKeyDown(KeyCode::ArrowLeft) || input.isKeyDown(KeyCode::A)) {
        playerX -= speed;
    }
    if (input.isKeyDown(KeyCode::ArrowRight) || input.isKeyDown(KeyCode::D)) {
        playerX += speed;
    }
    if (input.isKeyDown(KeyCode::ArrowUp) || input.isKeyDown(KeyCode::W)) {
        playerY -= speed;
    }
    if (input.isKeyDown(KeyCode::ArrowDown) || input.isKeyDown(KeyCode::S)) {
        playerY += speed;
    }
    
    // Action on key press (not held)
    if (input.isKeyJustPressed(KeyCode::Space)) {
        // Jump or shoot
        performAction();
    }
    
    // Draw player
    Draw::circle(playerX, playerY, 10, Colors::Blue);
}
```

### Modifier Keys
```cpp
void handleModifiers(const InputState& input) {
    bool shiftPressed = input.isKeyDown(KeyCode::Shift);
    bool ctrlPressed = input.isKeyDown(KeyCode::Ctrl);
    bool altPressed = input.isKeyDown(KeyCode::Alt);
    
    // Handle key combinations
    if (ctrlPressed && input.isKeyJustPressed(KeyCode::S)) {
        // Save action
        saveFile();
    }
    
    if (ctrlPressed && input.isKeyJustPressed(KeyCode::O)) {
        // Open action
        openFile();
    }
    
    if (altPressed && input.isKeyJustPressed(KeyCode::Enter)) {
        // Toggle fullscreen
        toggleFullscreen();
    }
    
    // Modify movement speed with shift
    int speed = shiftPressed ? 10 : 5;
    // Use speed for movement...
}
```

## Text Input

### Text Input Widget Integration
```cpp
class CustomTextInput : public Widget {
private:
    std::string text_;
    bool focused_;
    int cursorPos_;
    
public:
    CustomTextInput(int x, int y, int width, int height) 
        : focused_(false), cursorPos_(0) {
        x_ = x;
        y_ = y;
        width_ = width;
        height_ = height;
    }
    
    bool handleInput(const InputState& input) override {
        // Check if clicked
        if (input.mouseClicked) {
            bool wasClicked = (input.mouseX >= x_ && input.mouseX <= x_ + width_ &&
                              input.mouseY >= y_ && input.mouseY <= y_ + height_);
            focused_ = wasClicked;
            return wasClicked;
        }
        
        if (!focused_) return false;
        
        // Handle text input
        if (input.hasTextInput) {
            text_.insert(cursorPos_, input.textInput);
            cursorPos_ += input.textInput.length();
            return true;
        }
        
        // Handle special keys
        if (input.isKeyJustPressed(KeyCode::Backspace)) {
            if (cursorPos_ > 0) {
                text_.erase(cursorPos_ - 1, 1);
                cursorPos_--;
            }
            return true;
        }
        
        if (input.isKeyJustPressed(KeyCode::Delete)) {
            if (cursorPos_ < text_.length()) {
                text_.erase(cursorPos_, 1);
            }
            return true;
        }
        
        // Handle cursor movement
        if (input.isKeyJustPressed(KeyCode::ArrowLeft)) {
            cursorPos_ = std::max(0, cursorPos_ - 1);
            return true;
        }
        
        if (input.isKeyJustPressed(KeyCode::ArrowRight)) {
            cursorPos_ = std::min((int)text_.length(), cursorPos_ + 1);
            return true;
        }
        
        return false;
    }
    
    void render() override {
        // Draw input background
        uint32_t bgColor = focused_ ? Colors::White : Colors::LightGray;
        Draw::rect(x_, y_, width_, height_, bgColor);
        
        // Draw border
        uint32_t borderColor = focused_ ? Colors::Blue : Colors::Gray;
        Draw::roundedRectBorder(x_, y_, width_, height_, 4, 2, borderColor);
        
        // Draw text
        Font::renderText(getCanvas(), text_, x_ + 5, y_ + 5, 16, Colors::Black);
        
        // Draw cursor if focused
        if (focused_) {
            int cursorX = x_ + 5 + Font::getTextWidth(text_.substr(0, cursorPos_), 16);
            Draw::line(cursorX, y_ + 5, cursorX, y_ + height_ - 5, 1, Colors::Black);
        }
    }
    
    std::string getText() const { return text_; }
    void setText(const std::string& text) { 
        text_ = text; 
        cursorPos_ = text_.length();
    }
};
```

## Advanced Input Handling

### Input Buffering
```cpp
class InputBuffer {
private:
    std::queue<InputEvent> eventQueue_;
    
public:
    struct InputEvent {
        enum Type { MouseClick, KeyPress, KeyRelease, TextInput };
        Type type;
        int x, y;  // For mouse events
        KeyCode key;  // For key events
        std::string text;  // For text events
        uint32_t timestamp;
    };
    
    void addEvent(const InputEvent& event) {
        eventQueue_.push(event);
    }
    
    bool pollEvent(InputEvent& event) {
        if (eventQueue_.empty()) return false;
        
        event = eventQueue_.front();
        eventQueue_.pop();
        return true;
    }
    
    void processEvents() {
        InputEvent event;
        while (pollEvent(event)) {
            switch (event.type) {
                case InputEvent::MouseClick:
                    handleMouseClick(event.x, event.y);
                    break;
                case InputEvent::KeyPress:
                    handleKeyPress(event.key);
                    break;
                case InputEvent::TextInput:
                    handleTextInput(event.text);
                    break;
            }
        }
    }
};
```

### Input Validation
```cpp
class InputValidator {
public:
    static bool isValidEmail(const std::string& email) {
        return email.find('@') != std::string::npos && 
               email.find('.') != std::string::npos;
    }
    
    static bool isValidNumber(const std::string& text) {
        if (text.empty()) return false;
        
        for (char c : text) {
            if (!std::isdigit(c) && c != '.' && c != '-') {
                return false;
            }
        }
        return true;
    }
    
    static std::string filterNumericInput(const std::string& input) {
        std::string result;
        for (char c : input) {
            if (std::isdigit(c) || c == '.' || c == '-') {
                result += c;
            }
        }
        return result;
    }
    
    static std::string filterAlphanumeric(const std::string& input) {
        std::string result;
        for (char c : input) {
            if (std::isalnum(c) || c == ' ') {
                result += c;
            }
        }
        return result;
    }
};
```

### Input Gestures
```cpp
class GestureRecognizer {
private:
    std::vector<Point> mouseTrail_;
    uint32_t startTime_;
    
public:
    enum GestureType { None, Swipe, Circle, Tap, DoubleTap };
    
    void startGesture(const InputState& input) {
        mouseTrail_.clear();
        mouseTrail_.push_back(Point(input.mouseX, input.mouseY));
        startTime_ = getCurrentTime();
    }
    
    void updateGesture(const InputState& input) {
        if (input.mouseDown) {
            mouseTrail_.push_back(Point(input.mouseX, input.mouseY));
        }
    }
    
    GestureType recognizeGesture() {
        if (mouseTrail_.size() < 2) return None;
        
        uint32_t duration = getCurrentTime() - startTime_;
        
        // Quick tap
        if (duration < 200 && mouseTrail_.size() < 5) {
            return Tap;
        }
        
        // Swipe detection
        if (isSwipeGesture()) {
            return Swipe;
        }
        
        // Circle detection
        if (isCircleGesture()) {
            return Circle;
        }
        
        return None;
    }
    
private:
    bool isSwipeGesture() {
        // Simple swipe: mostly straight line with significant distance
        Point start = mouseTrail_.front();
        Point end = mouseTrail_.back();
        
        int dx = end.x - start.x;
        int dy = end.y - start.y;
        int distance = std::sqrt(dx*dx + dy*dy);
        
        return distance > 100; // Minimum swipe distance
    }
    
    bool isCircleGesture() {
        // Simple circle: path that returns near start point
        if (mouseTrail_.size() < 20) return false;
        
        Point start = mouseTrail_.front();
        Point end = mouseTrail_.back();
        
        int dx = end.x - start.x;
        int dy = end.y - start.y;
        int distance = std::sqrt(dx*dx + dy*dy);
        
        return distance < 50; // Close to start point
    }
};
```

## Performance Considerations

### Input Polling vs Events
```cpp
// Efficient input handling
void optimizedInputUpdate() {
    static InputState lastInput;
    const InputState& currentInput = Input::getState();
    
    // Only process changes
    if (currentInput.mouseX != lastInput.mouseX || 
        currentInput.mouseY != lastInput.mouseY) {
        handleMouseMove(currentInput);
    }
    
    if (currentInput.mouseClicked && !lastInput.mouseClicked) {
        handleMouseClick(currentInput);
    }
    
    lastInput = currentInput;
}
```

### Input Throttling
```cpp
class InputThrottler {
private:
    uint32_t lastInputTime_;
    uint32_t throttleDelay_;
    
public:
    InputThrottler(uint32_t delay = 50) : throttleDelay_(delay), lastInputTime_(0) {}
    
    bool shouldProcess() {
        uint32_t currentTime = getCurrentTime();
        if (currentTime - lastInputTime_ >= throttleDelay_) {
            lastInputTime_ = currentTime;
            return true;
        }
        return false;
    }
};
```

## Best Practices

### 1. Input Responsiveness
```cpp
// Separate input handling from rendering
void gameLoop() {
    while (isRunning()) {
        // Process input first
        handleInput();
        
        // Update game state
        updateGame();
        
        // Render
        render();
        
        // Frame timing
        limitFrameRate();
    }
}
```

### 2. Input Validation
```cpp
// Always validate input
void handleTextInput(const std::string& input) {
    // Filter unwanted characters
    std::string filtered = InputValidator::filterAlphanumeric(input);
    
    // Check length limits
    if (filtered.length() > MAX_INPUT_LENGTH) {
        filtered = filtered.substr(0, MAX_INPUT_LENGTH);
    }
    
    // Process validated input
    processValidatedInput(filtered);
}
```

### 3. Accessibility
```cpp
// Provide keyboard alternatives for mouse actions
void handleAccessibleInput(const InputState& input) {
    // Mouse click or Enter key
    if (input.mouseClicked || input.isKeyJustPressed(KeyCode::Enter)) {
        activateButton();
    }
    
    // Arrow keys for navigation
    if (input.isKeyJustPressed(KeyCode::ArrowUp)) {
        navigateUp();
    }
    if (input.isKeyJustPressed(KeyCode::ArrowDown)) {
        navigateDown();
    }
}
```

## Related Components

- [Event System](events.md) - High-level event handling
- [Signal/Slot System](signals.md) - Event communication patterns
- [Widget System](../widgets/) - Widget-level input handling
- [Text Input Widget](../widgets/text-input.md) - Specialized text input handling

## Tips

1. **Use appropriate input methods**: Mouse for pointing, keyboard for typing, combinations for shortcuts
2. **Provide visual feedback**: Show input states clearly to users
3. **Handle edge cases**: Empty input, invalid characters, rapid input
4. **Consider accessibility**: Provide keyboard alternatives for all mouse actions
5. **Optimize performance**: Don't process input that hasn't changed
6. **Test thoroughly**: Input handling is critical for user experience

The Input Handling system provides a solid foundation for creating responsive and accessible user interfaces in Fern applications.
