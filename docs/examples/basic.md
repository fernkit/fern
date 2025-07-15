# Basic Examples

## Overview

This guide provides fundamental examples to help you get started with the Fern UI Framework. These examples demonstrate core concepts and basic usage patterns that form the foundation of any Fern application.

## Table of Contents

1. [Hello World](#hello-world)
2. [Basic Shapes](#basic-shapes)
3. [Simple UI Elements](#simple-ui-elements)
4. [Color and Styling](#color-and-styling)
5. [Text Rendering](#text-rendering)
6. [Mouse Interaction](#mouse-interaction)
7. [Keyboard Input](#keyboard-input)
8. [Animation Basics](#animation-basics)
9. [Layout Fundamentals](#layout-fundamentals)
10. [Complete Basic App](#complete-basic-app)

## Hello World

The simplest possible Fern application:

```cpp
#include <fern/fern.hpp>

int main() {
    // Initialize Fern with 800x600 window
    Fern::initialize(800, 600);
    
    // Set up rendering callback
    Fern::setDrawCallback([]() {
        // Clear screen to dark gray
        Fern::Canvas::fill(0xFF2a2a2a);
        
        // Draw "Hello, World!" text
        Fern::Canvas::drawText("Hello, World!", 50, 50, Fern::Colors::WHITE);
    });
    
    // Start the main loop
    Fern::startRenderLoop();
    
    return 0;
}
```

### What This Does

1. **Initialize**: Creates a 800x600 pixel window
2. **Set Callback**: Defines what to draw each frame
3. **Clear Screen**: Fills the entire canvas with dark gray
4. **Draw Text**: Renders white text at position (50, 50)
5. **Start Loop**: Begins the main rendering loop

## Basic Shapes

Drawing fundamental shapes:

```cpp
#include <fern/fern.hpp>

int main() {
    Fern::initialize(800, 600);
    
    Fern::setDrawCallback([]() {
        // Clear to black
        Fern::Canvas::fill(0xFF000000);
        
        // Draw a rectangle
        Fern::Canvas::drawRect(50, 50, 200, 100, Fern::Colors::RED);
        
        // Draw a filled circle
        Fern::Canvas::drawCircle(400, 100, 50, Fern::Colors::BLUE);
        
        // Draw a line
        Fern::Canvas::drawLine(100, 200, 300, 250, 3, Fern::Colors::GREEN);
        
        // Draw a pixel
        Fern::Canvas::drawPixel(500, 300, Fern::Colors::YELLOW);
    });
    
    Fern::startRenderLoop();
    return 0;
}
```

### Shape Functions

- `drawRect(x, y, width, height, color)`: Rectangle
- `drawCircle(centerX, centerY, radius, color)`: Filled circle
- `drawLine(x1, y1, x2, y2, thickness, color)`: Line with thickness
- `drawPixel(x, y, color)`: Single pixel

## Simple UI Elements

Creating basic interactive elements:

```cpp
#include <fern/fern.hpp>

class SimpleUI {
private:
    bool buttonPressed = false;
    
public:
    void draw() {
        // Clear screen
        Fern::Canvas::fill(0xFF1a1a1a);
        
        // Draw title
        Fern::Canvas::drawText("Simple UI Demo", 10, 10, Fern::Colors::WHITE);
        
        // Draw button
        uint32_t buttonColor = buttonPressed ? Fern::Colors::DARK_GRAY : Fern::Colors::BLUE;
        Fern::Canvas::drawRect(50, 50, 150, 40, buttonColor);
        Fern::Canvas::drawText("Click Me", 70, 65, Fern::Colors::WHITE);
        
        // Draw status
        std::string status = buttonPressed ? "Button Pressed!" : "Button Released";
        Fern::Canvas::drawText(status, 50, 120, Fern::Colors::CYAN);
    }
    
    void handleClick(int x, int y) {
        // Check if click is inside button (50, 50, 150, 40)
        if (x >= 50 && x <= 200 && y >= 50 && y <= 90) {
            buttonPressed = !buttonPressed;
        }
    }
};

int main() {
    Fern::initialize(800, 600);
    
    SimpleUI ui;
    
    // Set up input handling
    auto renderer = Fern::createRenderer();
    renderer->setClickCallback([&ui](bool down) {
        if (down) {
            auto& input = Fern::Input::getState();
            ui.handleClick(input.mouseX, input.mouseY);
        }
    });
    
    // Set up rendering
    Fern::setDrawCallback([&ui]() {
        ui.draw();
    });
    
    Fern::startRenderLoop();
    return 0;
}
```

## Color and Styling

Working with colors and visual styles:

```cpp
#include <fern/fern.hpp>

int main() {
    Fern::initialize(800, 600);
    
    Fern::setDrawCallback([]() {
        // Clear with custom color
        Fern::Canvas::fill(0xFF0f0f0f);
        
        // Using predefined colors
        Fern::Canvas::drawRect(50, 50, 100, 100, Fern::Colors::RED);
        Fern::Canvas::drawRect(170, 50, 100, 100, Fern::Colors::GREEN);
        Fern::Canvas::drawRect(290, 50, 100, 100, Fern::Colors::BLUE);
        
        // Using custom colors (ARGB format)
        Fern::Canvas::drawRect(50, 170, 100, 100, 0xFFFF6B6B);  // Light red
        Fern::Canvas::drawRect(170, 170, 100, 100, 0xFF4ECDC4); // Teal
        Fern::Canvas::drawRect(290, 170, 100, 100, 0xFF45B7D1); // Light blue
        
        // Semi-transparent colors
        Fern::Canvas::drawRect(50, 290, 100, 100, 0x80FF0000);  // 50% red
        Fern::Canvas::drawRect(170, 290, 100, 100, 0x8000FF00); // 50% green
        Fern::Canvas::drawRect(290, 290, 100, 100, 0x800000FF); // 50% blue
        
        // Color labels
        Fern::Canvas::drawText("Primary Colors", 50, 30, Fern::Colors::WHITE);
        Fern::Canvas::drawText("Custom Colors", 50, 150, Fern::Colors::WHITE);
        Fern::Canvas::drawText("Transparent", 50, 270, Fern::Colors::WHITE);
    });
    
    Fern::startRenderLoop();
    return 0;
}
```

### Color Format

Colors in Fern use 32-bit ARGB format:
- `0xAARRGGBB` where AA=Alpha, RR=Red, GG=Green, BB=Blue
- `0xFF` = fully opaque, `0x80` = 50% transparent, `0x00` = fully transparent

## Text Rendering

Various text rendering techniques:

```cpp
#include <fern/fern.hpp>

int main() {
    Fern::initialize(800, 600);
    
    Fern::setDrawCallback([]() {
        Fern::Canvas::fill(0xFF1a1a1a);
        
        // Basic text
        Fern::Canvas::drawText("Basic Text", 10, 10, Fern::Colors::WHITE);
        
        // Colored text
        Fern::Canvas::drawText("Red Text", 10, 40, Fern::Colors::RED);
        Fern::Canvas::drawText("Blue Text", 10, 70, Fern::Colors::BLUE);
        Fern::Canvas::drawText("Green Text", 10, 100, Fern::Colors::GREEN);
        
        // Text with background
        Fern::Canvas::drawRect(10, 130, 200, 25, Fern::Colors::DARK_GRAY);
        Fern::Canvas::drawText("Text with Background", 15, 140, Fern::Colors::YELLOW);
        
        // Multi-line text simulation
        std::vector<std::string> lines = {
            "Line 1: This is the first line",
            "Line 2: This is the second line",
            "Line 3: This is the third line"
        };
        
        int y = 180;
        for (const auto& line : lines) {
            Fern::Canvas::drawText(line, 10, y, Fern::Colors::CYAN);
            y += 25;
        }
        
        // Formatted text
        int score = 12345;
        std::string scoreText = "Score: " + std::to_string(score);
        Fern::Canvas::drawText(scoreText, 10, 280, Fern::Colors::WHITE);
        
        // Right-aligned text simulation
        std::string rightText = "Right Aligned";
        // Note: In real implementation, you'd calculate text width
        int rightX = 800 - 150; // Approximate alignment
        Fern::Canvas::drawText(rightText, rightX, 310, Fern::Colors::WHITE);
    });
    
    Fern::startRenderLoop();
    return 0;
}
```

## Mouse Interaction

Handling mouse input and creating interactive elements:

```cpp
#include <fern/fern.hpp>

class MouseDemo {
private:
    int mouseX = 0;
    int mouseY = 0;
    bool mouseDown = false;
    std::vector<Point> clickPoints;
    
public:
    void draw() {
        Fern::Canvas::fill(0xFF1a1a1a);
        
        // Draw title
        Fern::Canvas::drawText("Mouse Interaction Demo", 10, 10, Fern::Colors::WHITE);
        
        // Draw mouse position
        std::string posText = "Mouse: " + std::to_string(mouseX) + ", " + std::to_string(mouseY);
        Fern::Canvas::drawText(posText, 10, 40, Fern::Colors::CYAN);
        
        // Draw mouse state
        std::string stateText = "Button: " + std::string(mouseDown ? "Pressed" : "Released");
        Fern::Canvas::drawText(stateText, 10, 70, Fern::Colors::YELLOW);
        
        // Draw crosshair at mouse position
        if (mouseX > 0 && mouseY > 0) {
            Fern::Canvas::drawLine(mouseX - 10, mouseY, mouseX + 10, mouseY, 1, Fern::Colors::RED);
            Fern::Canvas::drawLine(mouseX, mouseY - 10, mouseX, mouseY + 10, 1, Fern::Colors::RED);
        }
        
        // Draw click trail
        for (const auto& point : clickPoints) {
            Fern::Canvas::drawCircle(point.x, point.y, 5, Fern::Colors::GREEN);
        }
        
        // Draw instructions
        Fern::Canvas::drawText("Move mouse to see crosshair", 10, 550, Fern::Colors::GRAY);
        Fern::Canvas::drawText("Click to leave green dots", 10, 575, Fern::Colors::GRAY);
    }
    
    void updateMouse(int x, int y) {
        mouseX = x;
        mouseY = y;
    }
    
    void handleClick(bool down) {
        mouseDown = down;
        if (down) {
            clickPoints.push_back({mouseX, mouseY});
            
            // Limit trail length
            if (clickPoints.size() > 50) {
                clickPoints.erase(clickPoints.begin());
            }
        }
    }
};

int main() {
    Fern::initialize(800, 600);
    
    MouseDemo demo;
    
    // Set up input handling
    auto renderer = Fern::createRenderer();
    renderer->setMouseCallback([&demo](int x, int y) {
        demo.updateMouse(x, y);
    });
    
    renderer->setClickCallback([&demo](bool down) {
        demo.handleClick(down);
    });
    
    // Set up rendering
    Fern::setDrawCallback([&demo]() {
        demo.draw();
    });
    
    Fern::startRenderLoop();
    return 0;
}
```

## Keyboard Input

Handling keyboard input for interactive applications:

```cpp
#include <fern/fern.hpp>

class KeyboardDemo {
private:
    std::string inputText = "";
    std::vector<std::string> pressedKeys;
    bool showCursor = true;
    
public:
    void draw() {
        Fern::Canvas::fill(0xFF1a1a1a);
        
        // Draw title
        Fern::Canvas::drawText("Keyboard Input Demo", 10, 10, Fern::Colors::WHITE);
        
        // Draw input field
        Fern::Canvas::drawRect(10, 50, 500, 30, Fern::Colors::DARK_GRAY);
        Fern::Canvas::drawText("Type here: " + inputText, 15, 60, Fern::Colors::WHITE);
        
        // Draw cursor
        if (showCursor) {
            int cursorX = 15 + (inputText.length() * 8) + 80; // Approximate text width
            Fern::Canvas::drawLine(cursorX, 55, cursorX, 75, 1, Fern::Colors::WHITE);
        }
        
        // Draw pressed keys
        Fern::Canvas::drawText("Pressed keys:", 10, 100, Fern::Colors::CYAN);
        int y = 130;
        for (const auto& key : pressedKeys) {
            Fern::Canvas::drawText("- " + key, 10, y, Fern::Colors::YELLOW);
            y += 20;
        }
        
        // Draw instructions
        Fern::Canvas::drawText("Type to add text", 10, 400, Fern::Colors::GRAY);
        Fern::Canvas::drawText("Backspace to delete", 10, 420, Fern::Colors::GRAY);
        Fern::Canvas::drawText("Escape to clear", 10, 440, Fern::Colors::GRAY);
    }
    
    void handleKeyPress(Fern::KeyCode key) {
        std::string keyName = keyCodeToString(key);
        
        // Update pressed keys list
        pressedKeys.clear();
        pressedKeys.push_back(keyName);
        
        // Handle special keys
        if (key == Fern::KeyCode::Backspace) {
            if (!inputText.empty()) {
                inputText.pop_back();
            }
        } else if (key == Fern::KeyCode::Escape) {
            inputText.clear();
        } else if (key == Fern::KeyCode::Space) {
            inputText += " ";
        }
        // Add more key handling as needed
    }
    
    void handleTextInput(const std::string& text) {
        inputText += text;
    }
    
    void toggleCursor() {
        showCursor = !showCursor;
    }
    
private:
    std::string keyCodeToString(Fern::KeyCode key) {
        switch (key) {
            case Fern::KeyCode::Space: return "Space";
            case Fern::KeyCode::Enter: return "Enter";
            case Fern::KeyCode::Backspace: return "Backspace";
            case Fern::KeyCode::Escape: return "Escape";
            case Fern::KeyCode::A: return "A";
            case Fern::KeyCode::B: return "B";
            // Add more cases as needed
            default: return "Unknown";
        }
    }
};

int main() {
    Fern::initialize(800, 600);
    
    KeyboardDemo demo;
    
    // Set up input handling
    auto renderer = Fern::createRenderer();
    renderer->setKeyCallback([&demo](Fern::KeyCode key, bool pressed) {
        if (pressed) {
            demo.handleKeyPress(key);
        }
    });
    
    renderer->setTextInputCallback([&demo](const std::string& text) {
        demo.handleTextInput(text);
    });
    
    // Cursor blinking timer (simplified)
    int frameCount = 0;
    
    // Set up rendering
    Fern::setDrawCallback([&demo, &frameCount]() {
        demo.draw();
        
        // Blink cursor every 30 frames
        if (frameCount % 30 == 0) {
            demo.toggleCursor();
        }
        frameCount++;
    });
    
    Fern::startRenderLoop();
    return 0;
}
```

## Animation Basics

Simple animation techniques:

```cpp
#include <fern/fern.hpp>
#include <chrono>
#include <cmath>

class AnimationDemo {
private:
    float time = 0.0f;
    int bounceY = 100;
    int bounceDirection = 1;
    float rotationAngle = 0.0f;
    
public:
    void update(float deltaTime) {
        time += deltaTime;
        
        // Bouncing animation
        bounceY += bounceDirection * 2;
        if (bounceY <= 50 || bounceY >= 200) {
            bounceDirection *= -1;
        }
        
        // Rotation animation
        rotationAngle += deltaTime * 90.0f; // 90 degrees per second
        if (rotationAngle >= 360.0f) {
            rotationAngle -= 360.0f;
        }
    }
    
    void draw() {
        Fern::Canvas::fill(0xFF1a1a1a);
        
        // Draw title
        Fern::Canvas::drawText("Animation Demo", 10, 10, Fern::Colors::WHITE);
        
        // Bouncing ball
        Fern::Canvas::drawCircle(100, bounceY, 20, Fern::Colors::RED);
        Fern::Canvas::drawText("Bouncing Ball", 50, 250, Fern::Colors::WHITE);
        
        // Sine wave animation
        int centerX = 300;
        int centerY = 150;
        int waveX = centerX + static_cast<int>(std::sin(time * 2.0f) * 100);
        Fern::Canvas::drawCircle(waveX, centerY, 15, Fern::Colors::BLUE);
        Fern::Canvas::drawText("Sine Wave", 250, 250, Fern::Colors::WHITE);
        
        // Pulsing effect
        int pulseRadius = 20 + static_cast<int>(std::sin(time * 4.0f) * 10);
        Fern::Canvas::drawCircle(500, 150, pulseRadius, Fern::Colors::GREEN);
        Fern::Canvas::drawText("Pulsing Circle", 450, 250, Fern::Colors::WHITE);
        
        // Color animation
        uint8_t colorComponent = static_cast<uint8_t>(128 + std::sin(time * 3.0f) * 127);
        uint32_t animatedColor = (0xFF << 24) | (colorComponent << 16) | (0x80 << 8) | 0x80;
        Fern::Canvas::drawRect(50, 300, 100, 100, animatedColor);
        Fern::Canvas::drawText("Color Animation", 50, 420, Fern::Colors::WHITE);
        
        // Moving rectangle
        int moveX = 200 + static_cast<int>(std::sin(time * 1.5f) * 150);
        Fern::Canvas::drawRect(moveX, 300, 50, 50, Fern::Colors::YELLOW);
        Fern::Canvas::drawText("Moving Rectangle", 250, 420, Fern::Colors::WHITE);
    }
};

int main() {
    Fern::initialize(800, 600);
    
    AnimationDemo demo;
    auto lastTime = std::chrono::steady_clock::now();
    
    Fern::setDrawCallback([&demo, &lastTime]() {
        // Calculate delta time
        auto currentTime = std::chrono::steady_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;
        
        // Update and draw
        demo.update(deltaTime);
        demo.draw();
    });
    
    Fern::startRenderLoop();
    return 0;
}
```

## Layout Fundamentals

Basic layout concepts:

```cpp
#include <fern/fern.hpp>

class LayoutDemo {
public:
    void draw() {
        Fern::Canvas::fill(0xFF1a1a1a);
        
        // Draw title
        Fern::Canvas::drawText("Layout Demo", 10, 10, Fern::Colors::WHITE);
        
        // Grid layout
        drawGrid();
        
        // Centered elements
        drawCenteredElements();
        
        // Aligned elements
        drawAlignedElements();
    }
    
private:
    void drawGrid() {
        Fern::Canvas::drawText("Grid Layout:", 10, 50, Fern::Colors::CYAN);
        
        int startX = 10;
        int startY = 70;
        int cellSize = 50;
        int spacing = 10;
        
        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 4; col++) {
                int x = startX + col * (cellSize + spacing);
                int y = startY + row * (cellSize + spacing);
                
                uint32_t color = (row + col) % 2 == 0 ? Fern::Colors::BLUE : Fern::Colors::GREEN;
                Fern::Canvas::drawRect(x, y, cellSize, cellSize, color);
            }
        }
    }
    
    void drawCenteredElements() {
        Fern::Canvas::drawText("Centered Elements:", 10, 250, Fern::Colors::CYAN);
        
        int centerX = 400;
        int centerY = 350;
        
        // Large centered rectangle
        Fern::Canvas::drawRect(centerX - 100, centerY - 50, 200, 100, Fern::Colors::RED);
        
        // Small centered circle
        Fern::Canvas::drawCircle(centerX, centerY, 20, Fern::Colors::YELLOW);
        
        // Centered text (approximate)
        Fern::Canvas::drawText("Centered", centerX - 40, centerY - 5, Fern::Colors::BLACK);
    }
    
    void drawAlignedElements() {
        Fern::Canvas::drawText("Aligned Elements:", 10, 450, Fern::Colors::CYAN);
        
        int baseY = 480;
        
        // Left aligned
        Fern::Canvas::drawRect(10, baseY, 80, 30, Fern::Colors::BLUE);
        Fern::Canvas::drawText("Left", 15, baseY + 10, Fern::Colors::WHITE);
        
        // Center aligned
        Fern::Canvas::drawRect(360, baseY, 80, 30, Fern::Colors::GREEN);
        Fern::Canvas::drawText("Center", 370, baseY + 10, Fern::Colors::WHITE);
        
        // Right aligned
        Fern::Canvas::drawRect(710, baseY, 80, 30, Fern::Colors::RED);
        Fern::Canvas::drawText("Right", 720, baseY + 10, Fern::Colors::WHITE);
    }
};

int main() {
    Fern::initialize(800, 600);
    
    LayoutDemo demo;
    
    Fern::setDrawCallback([&demo]() {
        demo.draw();
    });
    
    Fern::startRenderLoop();
    return 0;
}
```

## Complete Basic App

A comprehensive example combining multiple concepts:

```cpp
#include <fern/fern.hpp>
#include <chrono>
#include <vector>
#include <string>

class BasicApp {
private:
    struct Button {
        int x, y, width, height;
        std::string text;
        uint32_t color;
        bool pressed;
        
        Button(int x, int y, int w, int h, const std::string& t, uint32_t c) 
            : x(x), y(y), width(w), height(h), text(t), color(c), pressed(false) {}
        
        bool contains(int mx, int my) const {
            return mx >= x && mx <= x + width && my >= y && my <= y + height;
        }
    };
    
    std::vector<Button> buttons;
    std::string status = "Ready";
    int counter = 0;
    bool showAnimation = false;
    float animationTime = 0.0f;
    
public:
    BasicApp() {
        // Initialize buttons
        buttons.emplace_back(50, 100, 100, 40, "Counter++", Fern::Colors::BLUE);
        buttons.emplace_back(170, 100, 100, 40, "Reset", Fern::Colors::RED);
        buttons.emplace_back(290, 100, 100, 40, "Animate", Fern::Colors::GREEN);
        buttons.emplace_back(410, 100, 100, 40, "Clear", Fern::Colors::YELLOW);
    }
    
    void update(float deltaTime) {
        if (showAnimation) {
            animationTime += deltaTime;
        }
    }
    
    void draw() {
        // Clear screen
        Fern::Canvas::fill(0xFF1a1a1a);
        
        // Draw title
        Fern::Canvas::drawText("Basic Fern Application", 10, 10, Fern::Colors::WHITE);
        
        // Draw counter
        std::string counterText = "Counter: " + std::to_string(counter);
        Fern::Canvas::drawText(counterText, 10, 40, Fern::Colors::CYAN);
        
        // Draw status
        Fern::Canvas::drawText("Status: " + status, 10, 70, Fern::Colors::YELLOW);
        
        // Draw buttons
        for (const auto& button : buttons) {
            uint32_t color = button.pressed ? Fern::Colors::DARK_GRAY : button.color;
            Fern::Canvas::drawRect(button.x, button.y, button.width, button.height, color);
            Fern::Canvas::drawText(button.text, button.x + 10, button.y + 15, Fern::Colors::WHITE);
        }
        
        // Draw animation if enabled
        if (showAnimation) {
            drawAnimation();
        }
        
        // Draw help text
        Fern::Canvas::drawText("Click buttons to interact", 10, 500, Fern::Colors::GRAY);
        Fern::Canvas::drawText("Press ESC to exit", 10, 520, Fern::Colors::GRAY);
    }
    
    void handleClick(int x, int y) {
        for (auto& button : buttons) {
            if (button.contains(x, y)) {
                button.pressed = true;
                handleButtonClick(button.text);
                
                // Reset button state after a short delay (simplified)
                // In a real app, you'd use a timer
                button.pressed = false;
                break;
            }
        }
    }
    
    void handleButtonClick(const std::string& buttonText) {
        if (buttonText == "Counter++") {
            counter++;
            status = "Counter incremented";
        } else if (buttonText == "Reset") {
            counter = 0;
            status = "Counter reset";
        } else if (buttonText == "Animate") {
            showAnimation = !showAnimation;
            animationTime = 0.0f;
            status = showAnimation ? "Animation started" : "Animation stopped";
        } else if (buttonText == "Clear") {
            counter = 0;
            showAnimation = false;
            animationTime = 0.0f;
            status = "Everything cleared";
        }
    }
    
private:
    void drawAnimation() {
        int centerX = 400;
        int centerY = 300;
        
        // Spinning circles
        for (int i = 0; i < 6; i++) {
            float angle = animationTime * 2.0f + i * 1.047f; // 60 degrees apart
            int x = centerX + static_cast<int>(std::cos(angle) * 100);
            int y = centerY + static_cast<int>(std::sin(angle) * 100);
            
            uint32_t color = (i % 3 == 0) ? Fern::Colors::RED : 
                           (i % 3 == 1) ? Fern::Colors::GREEN : Fern::Colors::BLUE;
            
            Fern::Canvas::drawCircle(x, y, 15, color);
        }
        
        // Center circle
        Fern::Canvas::drawCircle(centerX, centerY, 20, Fern::Colors::WHITE);
        
        // Animation label
        Fern::Canvas::drawText("Animation Running", centerX - 70, centerY + 50, Fern::Colors::WHITE);
    }
};

int main() {
    Fern::initialize(800, 600);
    
    BasicApp app;
    auto lastTime = std::chrono::steady_clock::now();
    
    // Set up input handling
    auto renderer = Fern::createRenderer();
    renderer->setClickCallback([&app](bool down) {
        if (down) {
            auto& input = Fern::Input::getState();
            app.handleClick(input.mouseX, input.mouseY);
        }
    });
    
    renderer->setKeyCallback([](Fern::KeyCode key, bool pressed) {
        if (key == Fern::KeyCode::Escape && pressed) {
            // Exit application
            exit(0);
        }
    });
    
    // Set up rendering
    Fern::setDrawCallback([&app, &lastTime]() {
        // Calculate delta time
        auto currentTime = std::chrono::steady_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;
        
        // Update and draw
        app.update(deltaTime);
        app.draw();
    });
    
    Fern::startRenderLoop();
    return 0;
}
```

## Key Concepts Learned

### 1. **Initialization**
- `Fern::initialize(width, height)` sets up the framework
- `Fern::startRenderLoop()` begins the main application loop

### 2. **Rendering**
- `Fern::setDrawCallback()` defines what to draw each frame
- `Fern::Canvas::fill()` clears the screen
- Drawing functions for shapes, text, and graphics

### 3. **Input Handling**
- Mouse callbacks for position and clicks
- Keyboard callbacks for key presses and text input
- Input state management

### 4. **Animation**
- Time-based animations using delta time
- Mathematical functions for smooth motion
- Frame-based state updates

### 5. **Interactive Elements**
- Button creation and click detection
- State management for UI elements
- Visual feedback for user interactions

## Next Steps

After mastering these basics, you can:

1. **Explore Advanced Widgets**: Learn about text inputs, sliders, and complex UI elements
2. **Study Layout Systems**: Understand how to create responsive layouts
3. **Dive into Graphics**: Learn about advanced drawing techniques and effects
4. **Master Event Systems**: Understand signals, slots, and complex event handling
5. **Build Complete Applications**: Create full-featured applications with multiple screens and complex interactions

## Related Documentation

- [Interactive Examples](interactive.md)
- [Widget Documentation](../widgets/)
- [Layout System](../layout/)
- [Input Handling](../input/)
- [Graphics & Styling](../graphics/)

---

*These basic examples provide the foundation for all Fern applications. Practice with these concepts before moving on to more advanced topics.*
