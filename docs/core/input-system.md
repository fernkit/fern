# Input System Guide

User interaction is the soul of any application - it's how thoughts and intentions flow from users into digital experiences. Fern's input system bridges this gap with a clean, efficient architecture that captures mouse movements, clicks, keyboard presses, and text input, transforming raw hardware events into meaningful application responses.

This guide will teach you not just how to handle input in Fern, but the fundamental principles of event-driven programming, the challenges of input buffering and timing, and how to create responsive, intuitive user interfaces.

## Understanding Digital Input

### The Nature of Input Events

Input in digital systems fundamentally differs from physical interaction:

1. **Discrete Events**: Unlike physical actions (which are continuous), digital input consists of discrete events - a key is either pressed or not, a mouse button is either down or up
2. **Temporal Considerations**: Events happen at specific moments in time, but your application processes them in frames
3. **Buffering and Timing**: Multiple events can occur between frames, requiring careful buffering and state management
4. **Platform Abstraction**: Different operating systems handle input differently, requiring abstraction layers

Fern's input system handles these complexities while providing a simple, unified interface for your application logic.

## Input Philosophy in Fern

Fern's input system follows several key principles:

- **Centralized State**: All input is managed through a single, global state object accessible anywhere in your application
- **Frame-Based Processing**: Input events are processed once per frame, ensuring consistent timing and performance
- **Event vs State**: Distinguishes between momentary events (clicks, key presses) and continuous states (mouse position, held keys)
- **Widget Integration**: Seamlessly integrates with the widget system for automatic input routing and handling
- **Platform Agnostic**: Abstracts platform-specific input handling behind a clean, consistent API

## The Input Flow

Understanding how input flows through Fern helps you design responsive applications:

```cpp
// 1. Platform Event (OS/Browser generates input)
//    ↓
// 2. Platform Handler (converts to Fern events)
//    ↓
// 3. Input System (updates global state)
//    ↓
// 4. Frame Processing (widgets check input state)
//    ↓
// 5. Application Response (your code responds to input)

#include <fern/fern.hpp>

using namespace Fern;

int main() {
    setupFern();
    
    while (shouldKeepRunning()) {
        // 1. Platform events are automatically captured
        
        // 2. processInput() updates the global input state
        processInput();
        
        // 3. Widgets and your code can now read the current state
        const InputState& input = Input::getState();
        
        if (input.mouseClicked) {
            std::cout << "Mouse clicked at: " << input.mouseX << ", " << input.mouseY << std::endl;
        }
        
        render();
    }
    
    return 0;
}
```

## The InputState Structure

At the heart of Fern's input system is the `InputState` structure, which contains all input information for the current frame:

```cpp
struct InputState {
    // Mouse input
    int mouseX = 0;          // Current mouse X position
    int mouseY = 0;          // Current mouse Y position  
    bool mouseDown = false;  // True if mouse button is currently pressed
    bool mouseClicked = false; // True if mouse was clicked THIS FRAME
    
    // Keyboard input
    KeyCode lastKeyPressed = KeyCode::None;   // Last key that was pressed
    KeyCode lastKeyReleased = KeyCode::None;  // Last key that was released
    bool keyPressed = false;   // True if ANY key was pressed this frame
    bool keyReleased = false;  // True if ANY key was released this frame
    
    // Text input (for typing)
    std::string textInput = "";  // Text entered this frame
    bool hasTextInput = false;   // True if text was entered this frame
    
    // Helper methods for specific key checking
    bool isKeyDown(KeyCode key) const;         // Is key currently held?
    bool isKeyJustPressed(KeyCode key) const;  // Was key just pressed?
    bool isKeyJustReleased(KeyCode key) const; // Was key just released?
};
```

### Understanding Event vs State

**State** represents continuous conditions:
- `mouseX`, `mouseY`: Always reflect current mouse position
- `mouseDown`: True as long as button is held
- `isKeyDown()`: True as long as key is held

**Events** represent momentary occurrences:
- `mouseClicked`: True only on the frame when button was pressed
- `keyPressed`: True only on the frame when any key was pressed
- `textInput`: Contains text only on the frame it was entered

## Basic Input Handling

### Mouse Input

```cpp
void handleMouseInput() {
    const InputState& input = Input::getState();
    
    // Current mouse position (always available)
    int mouseX = input.mouseX;
    int mouseY = input.mouseY;
    
    // Mouse button state
    if (input.mouseDown) {
        // Button is currently held down - good for dragging
        std::cout << "Dragging at: " << mouseX << ", " << mouseY << std::endl;
    }
    
    if (input.mouseClicked) {
        // Button was just clicked this frame - good for buttons
        std::cout << "Clicked at: " << mouseX << ", " << mouseY << std::endl;
    }
}
```

### Keyboard Input

```cpp
void handleKeyboardInput() {
    const InputState& input = Input::getState();
    
    // Check for any key activity
    if (input.keyPressed) {
        KeyCode key = input.lastKeyPressed;
        std::cout << "Key pressed: " << (int)key << std::endl;
    }
    
    // Check specific keys
    if (input.isKeyJustPressed(KeyCode::Space)) {
        std::cout << "Space bar pressed!" << std::endl;
    }
    
    if (input.isKeyDown(KeyCode::Shift)) {
        std::cout << "Shift is being held" << std::endl;
    }
    
    // Movement with WASD
    int deltaX = 0, deltaY = 0;
    if (input.isKeyDown(KeyCode::A)) deltaX = -1;
    if (input.isKeyDown(KeyCode::D)) deltaX = 1;
    if (input.isKeyDown(KeyCode::W)) deltaY = -1;
    if (input.isKeyDown(KeyCode::S)) deltaY = 1;
    
    if (deltaX != 0 || deltaY != 0) {
        movePlayer(deltaX, deltaY);
    }
}
```

### Text Input

```cpp
void handleTextInput() {
    const InputState& input = Input::getState();
    
    if (input.hasTextInput) {
        std::string text = input.textInput;
        std::cout << "User typed: " << text << std::endl;
        
        // Add to text field
        currentTextBuffer += text;
    }
}
```

## Advanced Input Patterns

### Click Detection with Bounds Checking

```cpp
class ClickableArea {
public:
    ClickableArea(int x, int y, int width, int height) 
        : x_(x), y_(y), width_(width), height_(height) {}
    
    bool checkClick(const InputState& input) {
        if (input.mouseClicked && contains(input.mouseX, input.mouseY)) {
            onClick();
            return true;
        }
        return false;
    }
    
    bool contains(int x, int y) const {
        return x >= x_ && x < x_ + width_ && 
               y >= y_ && y < y_ + height_;
    }
    
    virtual void onClick() {
        std::cout << "Area clicked!" << std::endl;
    }
    
private:
    int x_, y_, width_, height_;
};

// Usage
ClickableArea button(100, 100, 200, 50);

// In your main loop
const InputState& input = Input::getState();
button.checkClick(input);
```

### Keyboard Shortcuts and Combinations

```cpp
class KeyboardShortcuts {
public:
    void update(const InputState& input) {
        bool ctrl = input.isKeyDown(KeyCode::LControl) || input.isKeyDown(KeyCode::RControl);
        bool shift = input.isKeyDown(KeyCode::LShift) || input.isKeyDown(KeyCode::RShift);
        
        // Single key shortcuts
        if (input.isKeyJustPressed(KeyCode::Escape)) {
            showMenu();
        }
        
        // Ctrl combinations
        if (ctrl && input.isKeyJustPressed(KeyCode::S)) {
            saveFile();
        }
        
        if (ctrl && input.isKeyJustPressed(KeyCode::O)) {
            openFile();
        }
        
        // Ctrl+Shift combinations
        if (ctrl && shift && input.isKeyJustPressed(KeyCode::S)) {
            saveFileAs();
        }
        
        // Function keys
        if (input.isKeyJustPressed(KeyCode::F1)) {
            showHelp();
        }
    }
    
private:
    void showMenu() { std::cout << "Menu opened" << std::endl; }
    void saveFile() { std::cout << "File saved" << std::endl; }
    void openFile() { std::cout << "File opened" << std::endl; }
    void saveFileAs() { std::cout << "Save As dialog" << std::endl; }
    void showHelp() { std::cout << "Help displayed" << std::endl; }
};
```

### Input-Driven Animation

```cpp
class InputDrivenMovement {
public:
    InputDrivenMovement() : x_(100), y_(100), speed_(5) {}
    
    void update(const InputState& input) {
        // Smooth movement using held keys
        if (input.isKeyDown(KeyCode::Left) || input.isKeyDown(KeyCode::A)) {
            x_ -= speed_;
        }
        if (input.isKeyDown(KeyCode::Right) || input.isKeyDown(KeyCode::D)) {
            x_ += speed_;
        }
        if (input.isKeyDown(KeyCode::Up) || input.isKeyDown(KeyCode::W)) {
            y_ -= speed_;
        }
        if (input.isKeyDown(KeyCode::Down) || input.isKeyDown(KeyCode::S)) {
            y_ += speed_;
        }
        
        // Speed boost with shift
        if (input.isKeyDown(KeyCode::LShift)) {
            speed_ = 10;
        } else {
            speed_ = 5;
        }
        
        // Teleport to mouse on click
        if (input.mouseClicked) {
            x_ = input.mouseX;
            y_ = input.mouseY;
        }
    }
    
    void render() {
        // Draw player at current position
        auto player = Circle(Point(x_, y_), 10, Colors::Blue);
    }
    
private:
    int x_, y_;
    int speed_;
};
```

### Mouse Hover and Drag Operations

```cpp
class DragDropHandler {
public:
    DragDropHandler() : isDragging_(false), startX_(0), startY_(0) {}
    
    void update(const InputState& input) {
        // Start drag
        if (input.mouseClicked && !isDragging_) {
            startX_ = input.mouseX;
            startY_ = input.mouseY;
            isDragging_ = true;
            onDragStart(startX_, startY_);
        }
        
        // Continue drag
        if (isDragging_ && input.mouseDown) {
            int deltaX = input.mouseX - startX_;
            int deltaY = input.mouseY - startY_;
            onDragUpdate(input.mouseX, input.mouseY, deltaX, deltaY);
        }
        
        // End drag
        if (isDragging_ && !input.mouseDown) {
            int endX = input.mouseX;
            int endY = input.mouseY;
            isDragging_ = false;
            onDragEnd(endX, endY);
        }
    }
    
    virtual void onDragStart(int x, int y) {
        std::cout << "Drag started at: " << x << ", " << y << std::endl;
    }
    
    virtual void onDragUpdate(int currentX, int currentY, int deltaX, int deltaY) {
        std::cout << "Dragging to: " << currentX << ", " << currentY 
                  << " (delta: " << deltaX << ", " << deltaY << ")" << std::endl;
    }
    
    virtual void onDragEnd(int x, int y) {
        std::cout << "Drag ended at: " << x << ", " << y << std::endl;
    }
    
private:
    bool isDragging_;
    int startX_, startY_;
};
```

## Input in Widget Systems

### Widget Input Handling

Widgets automatically receive input through their `handleInput` method:

```cpp
class CustomWidget : public Widget {
public:
    bool handleInput(const InputState& input) override {
        // Check if input is within widget bounds
        if (contains(input.mouseX, input.mouseY)) {
            if (input.mouseClicked) {
                // Handle click
                onClick();
                return true; // Consume the input
            }
            
            if (input.isKeyJustPressed(KeyCode::Enter)) {
                // Handle enter key when widget has focus
                onActivate();
                return true;
            }
        }
        
        return false; // Don't consume input
    }
    
private:
    bool contains(int x, int y) const {
        return x >= getX() && x < getX() + getWidth() &&
               y >= getY() && y < getY() + getHeight();
    }
    
    void onClick() { /* Handle click */ }
    void onActivate() { /* Handle activation */ }
};
```

### Input Propagation and Consumption

```cpp
// Widgets are processed in reverse order (top to bottom)
// First widget that returns true "consumes" the input

class InputManager {
public:
    void processWidgetInput(const InputState& input) {
        // Process widgets from top to bottom
        for (auto it = widgets_.rbegin(); it != widgets_.rend(); ++it) {
            if ((*it)->handleInput(input)) {
                // Input was consumed, stop propagation
                break;
            }
        }
    }
    
private:
    std::vector<std::shared_ptr<Widget>> widgets_;
};
```

## Common Input Patterns

### Menu Navigation

```cpp
class MenuNavigator {
public:
    MenuNavigator(const std::vector<std::string>& items) 
        : items_(items), selectedIndex_(0) {}
    
    void update(const InputState& input) {
        // Navigate up/down
        if (input.isKeyJustPressed(KeyCode::Up) || input.isKeyJustPressed(KeyCode::W)) {
            selectedIndex_ = (selectedIndex_ - 1 + items_.size()) % items_.size();
        }
        
        if (input.isKeyJustPressed(KeyCode::Down) || input.isKeyJustPressed(KeyCode::S)) {
            selectedIndex_ = (selectedIndex_ + 1) % items_.size();
        }
        
        // Select item
        if (input.isKeyJustPressed(KeyCode::Enter) || input.isKeyJustPressed(KeyCode::Space)) {
            selectItem(selectedIndex_);
        }
        
        // Mouse selection
        if (input.mouseClicked) {
            int itemIndex = getItemAtPosition(input.mouseX, input.mouseY);
            if (itemIndex >= 0) {
                selectedIndex_ = itemIndex;
                selectItem(selectedIndex_);
            }
        }
    }
    
private:
    std::vector<std::string> items_;
    int selectedIndex_;
    
    int getItemAtPosition(int x, int y) {
        // Calculate which menu item is at this position
        // Return -1 if none
        return -1; // Placeholder
    }
    
    void selectItem(int index) {
        std::cout << "Selected: " << items_[index] << std::endl;
    }
};
```

### Text Input Field

```cpp
class TextInputField {
public:
    TextInputField() : cursorPosition_(0), hasFocus_(false) {}
    
    void update(const InputState& input) {
        if (!hasFocus_) return;
        
        // Handle text input
        if (input.hasTextInput) {
            text_.insert(cursorPosition_, input.textInput);
            cursorPosition_ += input.textInput.length();
        }
        
        // Handle cursor movement
        if (input.isKeyJustPressed(KeyCode::Left) && cursorPosition_ > 0) {
            cursorPosition_--;
        }
        
        if (input.isKeyJustPressed(KeyCode::Right) && cursorPosition_ < text_.length()) {
            cursorPosition_++;
        }
        
        // Handle backspace
        if (input.isKeyJustPressed(KeyCode::Backspace) && cursorPosition_ > 0) {
            text_.erase(cursorPosition_ - 1, 1);
            cursorPosition_--;
        }
        
        // Handle delete
        if (input.isKeyJustPressed(KeyCode::Delete) && cursorPosition_ < text_.length()) {
            text_.erase(cursorPosition_, 1);
        }
        
        // Home/End keys
        if (input.isKeyJustPressed(KeyCode::Home)) {
            cursorPosition_ = 0;
        }
        
        if (input.isKeyJustPressed(KeyCode::End)) {
            cursorPosition_ = text_.length();
        }
    }
    
    void setFocus(bool focus) { hasFocus_ = focus; }
    const std::string& getText() const { return text_; }
    
private:
    std::string text_;
    size_t cursorPosition_;
    bool hasFocus_;
};
```

### Game Controls

```cpp
class GameController {
public:
    struct ControlState {
        bool moveLeft = false;
        bool moveRight = false;
        bool moveUp = false;
        bool moveDown = false;
        bool jump = false;
        bool attack = false;
        bool defend = false;
        Point aimDirection{0, 0};
    };
    
    ControlState getControls(const InputState& input) {
        ControlState controls;
        
        // Movement (support both WASD and arrow keys)
        controls.moveLeft = input.isKeyDown(KeyCode::A) || input.isKeyDown(KeyCode::Left);
        controls.moveRight = input.isKeyDown(KeyCode::D) || input.isKeyDown(KeyCode::Right);
        controls.moveUp = input.isKeyDown(KeyCode::W) || input.isKeyDown(KeyCode::Up);
        controls.moveDown = input.isKeyDown(KeyCode::S) || input.isKeyDown(KeyCode::Down);
        
        // Actions
        controls.jump = input.isKeyJustPressed(KeyCode::Space);
        controls.attack = input.mouseClicked;
        controls.defend = input.isKeyDown(KeyCode::LShift);
        
        // Aim direction (mouse position relative to player)
        controls.aimDirection = Point(input.mouseX - playerX_, input.mouseY - playerY_);
        
        return controls;
    }
    
private:
    int playerX_ = 400; // Player position for aim calculation
    int playerY_ = 300;
};
```

## Performance and Best Practices

### Input Processing Efficiency

```cpp
// Efficient input handling
class EfficientInputHandler {
public:
    void update(const InputState& input) {
        // Cache commonly used values
        bool anyKeyPressed = input.keyPressed;
        bool mouseInBounds = isMouseInBounds(input.mouseX, input.mouseY);
        
        // Early exit if no relevant input
        if (!anyKeyPressed && !input.mouseClicked && !mouseInBounds) {
            return;
        }
        
        // Process only relevant input
        if (mouseInBounds && input.mouseClicked) {
            handleMouseClick(input.mouseX, input.mouseY);
        }
        
        if (anyKeyPressed) {
            handleKeyPress(input.lastKeyPressed);
        }
    }
    
private:
    bool isMouseInBounds(int x, int y) {
        // Quick bounds check
        return x >= 0 && x < screenWidth_ && y >= 0 && y < screenHeight_;
    }
    
    void handleMouseClick(int x, int y) { /* Handle click */ }
    void handleKeyPress(KeyCode key) { /* Handle key */ }
    
    int screenWidth_ = 800;
    int screenHeight_ = 600;
};
```

### Input State Management

```cpp
// Good: Single source of truth
void processGameInput() {
    const InputState& input = Input::getState(); // Get once, use everywhere
    
    player_.update(input);
    ui_.update(input);
    camera_.update(input);
}

// Avoid: Multiple input state retrievals
void inefficientProcessing() {
    player_.update(Input::getState());    // First retrieval
    ui_.update(Input::getState());        // Second retrieval  
    camera_.update(Input::getState());    // Third retrieval
}
```

### Input Validation and Sanitization

```cpp
class SafeInputHandler {
public:
    void update(const InputState& input) {
        // Validate mouse coordinates
        int safeMouseX = std::clamp(input.mouseX, 0, maxX_);
        int safeMouseY = std::clamp(input.mouseY, 0, maxY_);
        
        // Validate text input
        if (input.hasTextInput) {
            std::string safeText = sanitizeText(input.textInput);
            if (!safeText.empty()) {
                processTextInput(safeText);
            }
        }
        
        // Validate key codes
        if (input.keyPressed && isValidKey(input.lastKeyPressed)) {
            processKeyPress(input.lastKeyPressed);
        }
    }
    
private:
    std::string sanitizeText(const std::string& text) {
        std::string result;
        for (char c : text) {
            if (std::isprint(c) || c == '\n' || c == '\t') {
                result += c;
            }
        }
        return result;
    }
    
    bool isValidKey(KeyCode key) {
        return key != KeyCode::None && (int)key < 256;
    }
    
    void processTextInput(const std::string& text) { /* Safe processing */ }
    void processKeyPress(KeyCode key) { /* Safe processing */ }
    
    int maxX_ = 800;
    int maxY_ = 600;
};
```

## Input System Architecture

### Event Flow Architecture

Understanding how input flows through Fern helps you debug and optimize your applications:

```cpp
// 1. Platform Layer (OS/Browser events)
void onPlatformMouseMove(int x, int y) {
    Input::updateMousePosition(x, y);
}

void onPlatformMouseClick(bool down) {
    Input::updateMouseButton(down);
}

void onPlatformKeyPress(int keyCode) {
    Input::updateKeyPress(static_cast<KeyCode>(keyCode));
}

// 2. Input System (central state management)
class Input {
private:
    static InputState state_;
    
public:
    static void updateMousePosition(int x, int y) {
        state_.mouseX = x;
        state_.mouseY = y;
    }
    
    static void updateMouseButton(bool down) {
        state_.mouseDown = down;
        if (down) {
            state_.mouseClicked = true; // Set event flag
        }
    }
    
    static void resetEvents() {
        // Clear frame-specific events
        state_.mouseClicked = false;
        state_.keyPressed = false;
        state_.keyReleased = false;
        state_.hasTextInput = false;
        state_.textInput.clear();
    }
};

// 3. Application Layer (your code)
void gameLoop() {
    while (shouldKeepRunning()) {
        Input::resetEvents();     // Clear previous frame events
        processInput();           // Platform updates input state
        
        // Your code uses input state
        const InputState& input = Input::getState();
        handleGameInput(input);
        
        render();
    }
}
```

### Custom Input Extensions

You can extend the input system for specialized needs:

```cpp
class ExtendedInputManager {
public:
    struct ExtendedInput {
        // Standard input
        const InputState& base;
        
        // Extended features
        bool doubleClick = false;
        bool rightClick = false;
        float mouseWheelDelta = 0.0f;
        std::chrono::milliseconds clickDuration{0};
        
        ExtendedInput(const InputState& input) : base(input) {}
    };
    
    ExtendedInput getExtendedInput() {
        const InputState& base = Input::getState();
        ExtendedInput extended(base);
        
        // Detect double clicks
        extended.doubleClick = detectDoubleClick(base);
        
        // Add right click support (if platform provides it)
        extended.rightClick = rightClickState_;
        
        // Add mouse wheel
        extended.mouseWheelDelta = mouseWheelDelta_;
        
        // Calculate click duration
        extended.clickDuration = calculateClickDuration(base);
        
        return extended;
    }
    
private:
    bool detectDoubleClick(const InputState& input) {
        // Double click detection logic
        return false; // Placeholder
    }
    
    std::chrono::milliseconds calculateClickDuration(const InputState& input) {
        // Click duration calculation
        return std::chrono::milliseconds(0); // Placeholder
    }
    
    bool rightClickState_ = false;
    float mouseWheelDelta_ = 0.0f;
};
```

## Debugging Input Issues

### Input State Visualization

```cpp
class InputDebugger {
public:
    void displayInputState(const InputState& input) {
        std::cout << "=== INPUT DEBUG ===" << std::endl;
        std::cout << "Mouse: (" << input.mouseX << ", " << input.mouseY << ")" << std::endl;
        std::cout << "Mouse Down: " << (input.mouseDown ? "YES" : "NO") << std::endl;
        std::cout << "Mouse Clicked: " << (input.mouseClicked ? "YES" : "NO") << std::endl;
        
        if (input.keyPressed) {
            std::cout << "Key Pressed: " << (int)input.lastKeyPressed << std::endl;
        }
        
        if (input.hasTextInput) {
            std::cout << "Text Input: '" << input.textInput << "'" << std::endl;
        }
        
        std::cout << "===================" << std::endl;
    }
    
    void logInputHistory(const InputState& input) {
        // Log input for later analysis
        inputHistory_.push_back({
            std::chrono::steady_clock::now(),
            input.mouseX, input.mouseY,
            input.mouseClicked, input.keyPressed,
            input.lastKeyPressed
        });
        
        // Keep only recent history
        if (inputHistory_.size() > 100) {
            inputHistory_.erase(inputHistory_.begin());
        }
    }
    
private:
    struct InputFrame {
        std::chrono::steady_clock::time_point timestamp;
        int mouseX, mouseY;
        bool mouseClicked, keyPressed;
        KeyCode lastKey;
    };
    
    std::vector<InputFrame> inputHistory_;
};
```

## Troubleshooting Common Issues

### Input Not Responding

**Check these common problems:**

1. **Input not being processed**:
```cpp
// Make sure you're calling processInput() each frame
while (shouldKeepRunning()) {
    processInput(); // This updates the input state
    
    const InputState& input = Input::getState();
    // ... handle input
    
    render();
}
```

2. **Events being consumed by other widgets**:
```cpp
// Make sure widgets return false if they don't handle input
bool MyWidget::handleInput(const InputState& input) {
    if (contains(input.mouseX, input.mouseY) && input.mouseClicked) {
        onClick();
        return true; // Consume input
    }
    return false; // Don't consume input - let others handle it
}
```

3. **Frame timing issues**:
```cpp
// Events only last one frame - check immediately
const InputState& input = Input::getState();

// Good: Check in same frame
if (input.mouseClicked) {
    handleClick();
}

// Bad: Check in later frame
// (mouseClicked will be false by then)
```

### Performance Issues

```cpp
// Efficient: Check bounds before expensive operations
bool MyWidget::handleInput(const InputState& input) {
    // Quick bounds check first
    if (!contains(input.mouseX, input.mouseY)) {
        return false; // Exit early
    }
    
    // More expensive processing only if mouse is in bounds
    if (input.mouseClicked) {
        performExpensiveClickHandling();
        return true;
    }
    
    return false;
}
```

### Key Mapping Issues

```cpp
// Handle different key mappings gracefully
bool isMovementKey(KeyCode key) {
    return key == KeyCode::W || key == KeyCode::A || 
           key == KeyCode::S || key == KeyCode::D ||
           key == KeyCode::Up || key == KeyCode::Down ||
           key == KeyCode::Left || key == KeyCode::Right;
}

// Support multiple key bindings for same action
bool isJumpKey(const InputState& input) {
    return input.isKeyJustPressed(KeyCode::Space) ||
           input.isKeyJustPressed(KeyCode::Up) ||
           input.isKeyJustPressed(KeyCode::W);
}
```

## Integration Examples

### Input with Animation System

```cpp
class AnimatedButton {
public:
    void update(const InputState& input) {
        bool isHovered = contains(input.mouseX, input.mouseY);
        bool wasClicked = isHovered && input.mouseClicked;
        
        // Update animation state based on input
        if (wasClicked) {
            playClickAnimation();
        } else if (isHovered) {
            playHoverAnimation();
        } else {
            playIdleAnimation();
        }
    }
    
private:
    void playClickAnimation() { /* Animate click */ }
    void playHoverAnimation() { /* Animate hover */ }
    void playIdleAnimation() { /* Animate idle */ }
    
    bool contains(int x, int y) { return false; } // Placeholder
};
```

### Input with State Management

```cpp
class StatefulInputHandler {
public:
    enum class State { Menu, Game, Paused, Settings };
    
    void update(const InputState& input) {
        switch (currentState_) {
            case State::Menu:
                handleMenuInput(input);
                break;
            case State::Game:
                handleGameInput(input);
                break;
            case State::Paused:
                handlePausedInput(input);
                break;
            case State::Settings:
                handleSettingsInput(input);
                break;
        }
        
        // Global shortcuts work in any state
        handleGlobalInput(input);
    }
    
private:
    State currentState_ = State::Menu;
    
    void handleMenuInput(const InputState& input) {
        if (input.isKeyJustPressed(KeyCode::Enter)) {
            currentState_ = State::Game;
        }
    }
    
    void handleGameInput(const InputState& input) {
        if (input.isKeyJustPressed(KeyCode::Escape)) {
            currentState_ = State::Paused;
        }
        // ... game-specific input
    }
    
    void handlePausedInput(const InputState& input) {
        if (input.isKeyJustPressed(KeyCode::Escape)) {
            currentState_ = State::Game;
        }
    }
    
    void handleSettingsInput(const InputState& input) {
        if (input.isKeyJustPressed(KeyCode::Escape)) {
            currentState_ = State::Menu;
        }
    }
    
    void handleGlobalInput(const InputState& input) {
        if (input.isKeyJustPressed(KeyCode::F1)) {
            showHelp();
        }
    }
    
    void showHelp() { /* Show help */ }
};
```

## Summary

Fern's input system provides a clean, efficient foundation for creating responsive, interactive applications. By understanding the distinction between events and states, the flow of input through your application, and common patterns for handling user interaction, you can create intuitive interfaces that feel natural and responsive.

Key takeaways:

- **Centralized State**: All input flows through a single `InputState` accessible throughout your application
- **Event vs State**: Distinguish between momentary events (clicks, key presses) and continuous states (positions, held keys)
- **Frame-Based Processing**: Input is processed once per frame for consistent timing and performance
- **Widget Integration**: The input system seamlessly integrates with widgets for automatic routing
- **Performance Focused**: Efficient design allows for responsive applications even with complex input handling

The input system exemplifies Fern's philosophy: hide the complexity of platform-specific input handling behind a simple, intuitive API that lets you focus on creating great user experiences rather than wrestling with low-level input management.

Master input handling, and you'll have the foundation for creating applications that feel alive and responsive to user intent - the hallmark of truly engaging digital experiences.
