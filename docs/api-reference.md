# API Reference

## Overview

This document provides a comprehensive reference for the Fern UI Framework API. All classes, functions, and constants are documented with their signatures, parameters, return values, and usage examples.

## Table of Contents

1. [Core API](#core-api)
2. [Widget Classes](#widget-classes)
3. [Layout System](#layout-system)
4. [Graphics API](#graphics-api)
5. [Input System](#input-system)
6. [Platform API](#platform-api)
7. [Utility Classes](#utility-classes)
8. [Constants and Enums](#constants-and-enums)

## Core API

### Fern Namespace

#### Initialization Functions

```cpp
namespace Fern {
    /**
     * @brief Initialize the Fern framework
     * @param width Initial window/canvas width
     * @param height Initial window/canvas height
     * @param title Window title (optional)
     */
    void initialize(int width, int height, const std::string& title = "Fern Application");
    
    /**
     * @brief Start the main render loop
     * @note On web platforms, this starts the Emscripten main loop
     */
    void startRenderLoop();
    
    /**
     * @brief Check if the application should close
     * @return bool True if application should exit
     */
    bool shouldClose();
    
    /**
     * @brief Poll for input events
     */
    void pollEvents();
    
    /**
     * @brief Render all widgets
     */
    void render();
    
    /**
     * @brief Shutdown the framework and cleanup resources
     */
    void shutdown();
}
```

#### Window Management

```cpp
namespace Fern {
    /**
     * @brief Get current window width
     * @return int Window width in pixels
     */
    int getWidth();
    
    /**
     * @brief Get current window height
     * @return int Window height in pixels
     */
    int getHeight();
    
    /**
     * @brief Set window size
     * @param width New width in pixels
     * @param height New height in pixels
     */
    void setSize(int width, int height);
    
    /**
     * @brief Set window title
     * @param title New window title
     */
    void setTitle(const std::string& title);
    
    /**
     * @brief Get platform name
     * @return std::string Platform identifier
     */
    std::string getPlatformName();
}
```

#### Callback Registration

```cpp
namespace Fern {
    /**
     * @brief Set the main draw callback
     * @param callback Function to call each frame
     */
    void setDrawCallback(std::function<void()> callback);
    
    /**
     * @brief Set mouse movement callback
     * @param callback Function called on mouse move (x, y)
     */
    void setMouseCallback(std::function<void(int, int)> callback);
    
    /**
     * @brief Set mouse click callback
     * @param callback Function called on mouse click (pressed)
     */
    void setClickCallback(std::function<void(bool)> callback);
    
    /**
     * @brief Set keyboard callback
     * @param callback Function called on key events (key, pressed)
     */
    void setKeyCallback(std::function<void(KeyCode, bool)> callback);
    
    /**
     * @brief Set text input callback
     * @param callback Function called on text input (text)
     */
    void setTextInputCallback(std::function<void(const std::string&)> callback);
    
    /**
     * @brief Set window resize callback
     * @param callback Function called on window resize (width, height)
     */
    void setResizeCallback(std::function<void(int, int)> callback);
}
```

#### Widget Management

```cpp
namespace Fern {
    /**
     * @brief Add widget to the global widget manager
     * @param widget Shared pointer to widget
     */
    void addWidget(std::shared_ptr<Widget> widget);
    
    /**
     * @brief Remove widget from the global widget manager
     * @param widget Shared pointer to widget
     */
    void removeWidget(std::shared_ptr<Widget> widget);
    
    /**
     * @brief Clear all widgets from the manager
     */
    void clearWidgets();
    
    /**
     * @brief Get number of managed widgets
     * @return size_t Number of widgets
     */
    size_t getWidgetCount();
}
```

## Widget Classes

### Base Widget Class

```cpp
class Widget {
public:
    /**
     * @brief Default constructor
     */
    Widget() = default;
    
    /**
     * @brief Virtual destructor
     */
    virtual ~Widget() = default;
    
    /**
     * @brief Render the widget (pure virtual)
     */
    virtual void render() = 0;
    
    /**
     * @brief Handle input events (pure virtual)
     * @param input Current input state
     * @return bool True if input was handled
     */
    virtual bool handleInput(const InputState& input) = 0;
    
    /**
     * @brief Set widget position
     * @param x X coordinate
     * @param y Y coordinate
     */
    virtual void setPosition(int x, int y);
    
    /**
     * @brief Get X position
     * @return int X coordinate
     */
    virtual int getX() const;
    
    /**
     * @brief Get Y position
     * @return int Y coordinate
     */
    virtual int getY() const;
    
    /**
     * @brief Resize widget
     * @param width New width
     * @param height New height
     */
    virtual void resize(int width, int height);
    
    /**
     * @brief Get widget width
     * @return int Width in pixels
     */
    virtual int getWidth() const;
    
    /**
     * @brief Get widget height
     * @return int Height in pixels
     */
    virtual int getHeight() const;
};
```

### ButtonWidget

```cpp
class ButtonStyle {
public:
    ButtonStyle();
    
    // Fluent interface methods
    ButtonStyle& normalColor(uint32_t color);
    ButtonStyle& hoverColor(uint32_t color);
    ButtonStyle& pressColor(uint32_t color);
    ButtonStyle& textColor(uint32_t color);
    ButtonStyle& textScale(int scale);
    ButtonStyle& borderRadius(int radius);
    ButtonStyle& borderWidth(int width);
    ButtonStyle& borderColor(uint32_t color);
    ButtonStyle& useBitmapFont();
    ButtonStyle& useTTFFont(const std::string& fontName = "");
    
    // Getters
    uint32_t getNormalColor() const;
    uint32_t getHoverColor() const;
    uint32_t getPressColor() const;
    uint32_t getTextColor() const;
    int getTextScale() const;
    int getBorderRadius() const;
    int getBorderWidth() const;
    uint32_t getBorderColor() const;
    FontType getFontType() const;
    const std::string& getTTFFontName() const;
};

class ButtonConfig {
public:
    /**
     * @brief Construct button configuration
     * @param x X position
     * @param y Y position
     * @param width Button width
     * @param height Button height
     * @param text Button text
     */
    ButtonConfig(int x, int y, int width, int height, const std::string& text);
    
    // Fluent interface
    ButtonConfig& style(const ButtonStyle& s);
    ButtonConfig& autoSize(bool enabled);
    ButtonConfig& minWidth(int width);
    ButtonConfig& padding(int p);
    
    // Getters
    int getX() const;
    int getY() const;
    int getWidth() const;
    int getHeight() const;
    const std::string& getText() const;
    const ButtonStyle& getStyle() const;
    bool getAutoSize() const;
    int getMinWidth() const;
    int getPadding() const;
};

class ButtonWidget : public Widget {
public:
    /**
     * @brief Construct button with configuration
     * @param config Button configuration
     */
    ButtonWidget(const ButtonConfig& config);
    
    /**
     * @brief Legacy constructor
     * @param position Button position
     * @param width Button width
     * @param height Button height
     * @param text Button text
     */
    ButtonWidget(Point position, int width, int height, const std::string& text);
    
    // Widget interface
    void render() override;
    bool handleInput(const InputState& input) override;
    
    // Button-specific methods
    void setText(const std::string& text);
    void setStyle(const ButtonStyle& style);
    void setAutoSize(bool enabled);
    
    // Getters
    const std::string& getText() const;
    const ButtonStyle& getStyle() const;
    bool isHovered() const;
    bool isPressed() const;
    
    // Events
    Signal<> onClick;
    Signal<bool> onHover;
    Signal<bool> onPress;
};

/**
 * @brief Factory function for creating buttons
 * @param config Button configuration
 * @param addToManager Whether to add to widget manager
 * @return std::shared_ptr<ButtonWidget> Created button
 */
std::shared_ptr<ButtonWidget> Button(const ButtonConfig& config, bool addToManager = true);

/**
 * @brief Legacy factory function
 */
std::shared_ptr<ButtonWidget> Button(Point position, int width, int height, 
                                    const std::string& text, bool addToManager = true);
```

### TextWidget

```cpp
class TextStyle {
public:
    TextStyle();
    
    // Fluent interface
    TextStyle& color(uint32_t c);
    TextStyle& backgroundColor(uint32_t c);
    TextStyle& fontSize(int size);
    TextStyle& padding(int p);
    TextStyle& alignment(int a);  // 0=left, 1=center, 2=right
    TextStyle& shadow(bool enabled, uint32_t shadowColor = 0xFF000000, int offset = 1);
    TextStyle& useBitmapFont();
    TextStyle& useTTFFont(const std::string& fontName = "");
    
    // Getters
    uint32_t getColor() const;
    uint32_t getBackgroundColor() const;
    int getFontSize() const;
    FontType getFontType() const;
    const std::string& getTTFFontName() const;
    bool hasBackground() const;
    int getPadding() const;
    int getAlignment() const;
    bool hasShadow() const;
    uint32_t getShadowColor() const;
    int getShadowOffset() const;
};

class TextConfig {
public:
    /**
     * @brief Construct text configuration
     * @param x X position
     * @param y Y position
     * @param text Text content
     */
    TextConfig(int x, int y, const std::string& text);
    
    // Fluent interface
    TextConfig& style(const TextStyle& s);
    TextConfig& text(const std::string& t);
    TextConfig& position(int x, int y);
    
    // Getters
    int getX() const;
    int getY() const;
    const std::string& getText() const;
    const TextStyle& getStyle() const;
};

class TextWidget : public Widget {
public:
    /**
     * @brief Construct with modern configuration
     * @param config Text configuration
     */
    TextWidget(const TextConfig& config);
    
    /**
     * @brief Legacy constructor
     * @param position Text position
     * @param text Text content
     * @param size Font size
     * @param color Text color
     * @param fontType Font type
     */
    TextWidget(Point position, const std::string& text, int size, uint32_t color, 
              FontType fontType = FontType::Bitmap);
    
    // Widget interface
    void render() override;
    bool handleInput(const InputState& input) override;
    
    // Text-specific methods
    void setText(const std::string& text);
    void setSize(int size);
    void setColor(uint32_t color);
    void setFontType(FontType type);
    void setConfig(const TextConfig& config);
    
    // Getters
    const std::string& getText() const;
    int getSize() const;
    uint32_t getColor() const;
    FontType getFontType() const;
    const TextConfig& getConfig() const;
};

/**
 * @brief Factory function for creating text widgets
 */
std::shared_ptr<TextWidget> Text(const TextConfig& config, bool addToManager = true);

/**
 * @brief Legacy factory function
 */
std::shared_ptr<TextWidget> Text(Point position, const std::string& text, 
                                int size, uint32_t color, bool addToManager = true,
                                FontType fontType = FontType::Bitmap);

/**
 * @brief Text preset configurations
 */
namespace TextPresets {
    TextConfig Title(int x, int y, const std::string& text);
    TextConfig Subtitle(int x, int y, const std::string& text);
    TextConfig Body(int x, int y, const std::string& text);
    TextConfig Caption(int x, int y, const std::string& text);
    TextConfig Button(int x, int y, const std::string& text);
    TextConfig Error(int x, int y, const std::string& text);
    TextConfig Success(int x, int y, const std::string& text);
}
```

### CircleWidget

```cpp
class CircleWidget : public Widget {
public:
    /**
     * @brief Construct a circle widget
     * @param center Center point of the circle
     * @param radius Circle radius in pixels
     * @param color Circle color in ARGB format
     * @param filled Whether to fill the circle (true) or just draw outline (false)
     */
    CircleWidget(Point center, int radius, uint32_t color, bool filled = true);
    
    // Widget interface
    void render() override;
    bool handleInput(const InputState& input) override;
    
    // Circle-specific methods
    void setCenter(Point center);
    void setRadius(int radius);
    void setColor(uint32_t color);
    void setFilled(bool filled);
    
    // Getters
    Point getCenter() const;
    int getRadius() const;
    uint32_t getColor() const;
    bool isFilled() const;
    bool isHovered() const;
    
    // Events
    Signal<> onClick;
    Signal<bool> onHover;
};

/**
 * @brief Factory function for creating circles
 */
std::shared_ptr<CircleWidget> Circle(Point center, int radius, uint32_t color, 
                                    bool filled = true, bool addToManager = true);
```

### LineWidget

```cpp
class LineWidget : public Widget {
public:
    /**
     * @brief Construct a line widget
     * @param start Starting point
     * @param end Ending point
     * @param thickness Line thickness in pixels
     * @param color Line color in ARGB format
     */
    LineWidget(Point start, Point end, int thickness, uint32_t color);
    
    // Widget interface
    void render() override;
    bool handleInput(const InputState& input) override;
    
    // Line-specific methods
    void setStart(Point start);
    void setEnd(Point end);
    void setThickness(int thickness);
    void setColor(uint32_t color);
    
    // Getters
    Point getStart() const;
    Point getEnd() const;
    int getThickness() const;
    uint32_t getColor() const;
};

/**
 * @brief Factory function for creating lines
 */
std::shared_ptr<LineWidget> Line(Point start, Point end, int thickness, uint32_t color, 
                                bool addToManager = true);
```

## Layout System

### Layout Base Classes

```cpp
class LayoutWidget : public Widget {
public:
    /**
     * @brief Add child widget to layout
     * @param child Widget to add
     */
    virtual void add(std::shared_ptr<Widget> child);
    
    /**
     * @brief Remove child widget from layout
     * @param child Widget to remove
     */
    virtual void remove(std::shared_ptr<Widget> child);
    
    /**
     * @brief Remove all child widgets
     */
    virtual void clear();
    
    /**
     * @brief Get number of children
     * @return size_t Number of child widgets
     */
    size_t getChildCount() const;
    
protected:
    /**
     * @brief Layout children (implemented by concrete layouts)
     */
    virtual void layoutChildren() = 0;
    
    std::vector<std::shared_ptr<Widget>> children_;
};
```

### Column Layout

```cpp
class ColumnWidget : public LayoutWidget {
public:
    /**
     * @brief Construct column layout
     * @param x X position
     * @param y Y position
     * @param width Column width
     * @param height Column height
     * @param spacing Spacing between children
     */
    ColumnWidget(int x, int y, int width, int height, int spacing = 0);
    
    /**
     * @brief Construct column with children
     * @param children Vector of child widgets
     * @param spacing Spacing between children
     */
    ColumnWidget(const std::vector<std::shared_ptr<Widget>>& children, int spacing = 0);
    
    // Widget interface
    void render() override;
    bool handleInput(const InputState& input) override;
    
    // Column-specific methods
    void setSpacing(int spacing);
    int getSpacing() const;
    
protected:
    void layoutChildren() override;
};

/**
 * @brief Factory function for column layouts
 */
std::shared_ptr<ColumnWidget> Column(const std::vector<std::shared_ptr<Widget>>& children, 
                                    int spacing = 0, bool addToManager = true);
```

### Row Layout

```cpp
class RowWidget : public LayoutWidget {
public:
    /**
     * @brief Construct row layout
     * @param x X position
     * @param y Y position
     * @param width Row width
     * @param height Row height
     * @param spacing Spacing between children
     */
    RowWidget(int x, int y, int width, int height, int spacing = 0);
    
    /**
     * @brief Construct row with children
     * @param children Vector of child widgets
     * @param spacing Spacing between children
     */
    RowWidget(const std::vector<std::shared_ptr<Widget>>& children, int spacing = 0);
    
    // Widget interface
    void render() override;
    bool handleInput(const InputState& input) override;
    
    // Row-specific methods
    void setSpacing(int spacing);
    int getSpacing() const;
    
protected:
    void layoutChildren() override;
};

/**
 * @brief Factory function for row layouts
 */
std::shared_ptr<RowWidget> Row(const std::vector<std::shared_ptr<Widget>>& children, 
                              int spacing = 0, bool addToManager = true);
```

### Center Layout

```cpp
class CenterWidget : public LayoutWidget {
public:
    /**
     * @brief Construct center layout
     * @param x X position
     * @param y Y position
     * @param width Container width
     * @param height Container height
     */
    CenterWidget(int x, int y, int width, int height);
    
    /**
     * @brief Construct center with child
     * @param child Widget to center
     * @param containerWidth Container width
     * @param containerHeight Container height
     */
    CenterWidget(std::shared_ptr<Widget> child, int containerWidth, int containerHeight);
    
    // Widget interface
    void render() override;
    bool handleInput(const InputState& input) override;
    
protected:
    void layoutChildren() override;
};

/**
 * @brief Factory function for center layouts
 */
std::shared_ptr<CenterWidget> Center(std::shared_ptr<Widget> child, 
                                    int containerWidth, int containerHeight, 
                                    bool addToManager = true);
```

### Utility Widgets

```cpp
class SizedBox : public Widget {
public:
    /**
     * @brief Construct sized box (spacing widget)
     * @param width Box width
     * @param height Box height
     */
    SizedBox(int width, int height);
    
    // Widget interface
    void render() override;
    bool handleInput(const InputState& input) override;
};

/**
 * @brief Factory function for sized boxes
 */
std::shared_ptr<SizedBox> SizedBox(int width, int height);

class ExpandedWidget : public LayoutWidget {
public:
    /**
     * @brief Construct expanded widget
     * @param child Child widget to expand
     * @param flex Flex factor for expansion
     */
    ExpandedWidget(std::shared_ptr<Widget> child, int flex = 1);
    
    // Widget interface
    void render() override;
    bool handleInput(const InputState& input) override;
    
    // Expanded-specific methods
    void setFlex(int flex);
    int getFlex() const;
    
protected:
    void layoutChildren() override;
};

/**
 * @brief Factory function for expanded widgets
 */
std::shared_ptr<ExpandedWidget> Expanded(std::shared_ptr<Widget> child, int flex = 1);
```

## Graphics API

### Canvas Class

```cpp
class Canvas {
public:
    /**
     * @brief Fill entire canvas with color
     * @param color Fill color in ARGB format
     */
    static void fill(uint32_t color);
    
    /**
     * @brief Draw filled rectangle
     * @param x X position
     * @param y Y position
     * @param width Rectangle width
     * @param height Rectangle height
     * @param color Rectangle color
     */
    static void drawRect(int x, int y, int width, int height, uint32_t color);
    
    /**
     * @brief Draw rectangle outline
     * @param x X position
     * @param y Y position
     * @param width Rectangle width
     * @param height Rectangle height
     * @param color Border color
     * @param thickness Border thickness
     */
    static void drawRectBorder(int x, int y, int width, int height, 
                              uint32_t color, int thickness = 1);
    
    /**
     * @brief Draw filled circle
     * @param centerX Center X coordinate
     * @param centerY Center Y coordinate
     * @param radius Circle radius
     * @param color Circle color
     */
    static void drawCircle(int centerX, int centerY, int radius, uint32_t color);
    
    /**
     * @brief Draw circle outline
     * @param centerX Center X coordinate
     * @param centerY Center Y coordinate
     * @param radius Circle radius
     * @param color Circle color
     * @param thickness Line thickness
     */
    static void drawCircleOutline(int centerX, int centerY, int radius, 
                                 uint32_t color, int thickness = 1);
    
    /**
     * @brief Draw line between two points
     * @param x1 Start X coordinate
     * @param y1 Start Y coordinate
     * @param x2 End X coordinate
     * @param y2 End Y coordinate
     * @param color Line color
     * @param thickness Line thickness
     */
    static void drawLine(int x1, int y1, int x2, int y2, uint32_t color, int thickness = 1);
    
    /**
     * @brief Draw text using bitmap font
     * @param text Text to draw
     * @param x X position
     * @param y Y position
     * @param color Text color
     * @param scale Text scale factor
     */
    static void drawText(const std::string& text, int x, int y, uint32_t color, int scale = 1);
    
    /**
     * @brief Draw text using TTF font
     * @param text Text to draw
     * @param x X position
     * @param y Y position
     * @param size Font size
     * @param color Text color
     * @param fontName Font name (optional)
     */
    static void drawTTFText(const std::string& text, int x, int y, int size, 
                           uint32_t color, const std::string& fontName = "");
    
    /**
     * @brief Draw rounded rectangle
     * @param x X position
     * @param y Y position
     * @param width Rectangle width
     * @param height Rectangle height
     * @param radius Corner radius
     * @param color Rectangle color
     */
    static void drawRoundedRect(int x, int y, int width, int height, 
                               int radius, uint32_t color);
    
    /**
     * @brief Get text dimensions
     * @param text Text to measure
     * @param scale Text scale (for bitmap font)
     * @return Point Width and height as Point(width, height)
     */
    static Point getTextSize(const std::string& text, int scale = 1);
    
    /**
     * @brief Get TTF text dimensions
     * @param text Text to measure
     * @param size Font size
     * @param fontName Font name (optional)
     * @return Point Width and height as Point(width, height)
     */
    static Point getTTFTextSize(const std::string& text, int size, 
                               const std::string& fontName = "");
};
```

### Colors Namespace

```cpp
namespace Colors {
    // Basic colors
    constexpr uint32_t Black       = 0xFF000000;
    constexpr uint32_t White       = 0xFFFFFFFF;
    constexpr uint32_t Red         = 0xFFFF0000;
    constexpr uint32_t Green       = 0xFF00FF00;
    constexpr uint32_t Blue        = 0xFF0000FF;
    constexpr uint32_t Yellow      = 0xFFFFFF00;
    constexpr uint32_t Cyan        = 0xFF00FFFF;
    constexpr uint32_t Magenta     = 0xFFFF00FF;
    
    // Gray scale
    constexpr uint32_t Gray        = 0xFF808080;
    constexpr uint32_t LightGray   = 0xFFC0C0C0;
    constexpr uint32_t DarkGray    = 0xFF404040;
    
    // Extended colors
    constexpr uint32_t Orange      = 0xFFFFA500;
    constexpr uint32_t Purple      = 0xFF800080;
    constexpr uint32_t Brown       = 0xFFA52A2A;
    constexpr uint32_t Pink        = 0xFFFFC0CB;
    
    // Material Design colors
    constexpr uint32_t MaterialBlue      = 0xFF2196F3;
    constexpr uint32_t MaterialRed       = 0xFFF44336;
    constexpr uint32_t MaterialGreen     = 0xFF4CAF50;
    constexpr uint32_t MaterialOrange    = 0xFFFF9800;
    constexpr uint32_t MaterialPurple    = 0xFF9C27B0;
    
    /**
     * @brief Create ARGB color from components
     * @param r Red component (0-255)
     * @param g Green component (0-255)
     * @param b Blue component (0-255)
     * @param a Alpha component (0-255, default 255)
     * @return uint32_t ARGB color value
     */
    uint32_t RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
    
    /**
     * @brief Create color from HSV values
     * @param h Hue (0-360)
     * @param s Saturation (0-100)
     * @param v Value (0-100)
     * @param a Alpha (0-255, default 255)
     * @return uint32_t ARGB color value
     */
    uint32_t HSV(float h, float s, float v, uint8_t a = 255);
    
    /**
     * @brief Mix two colors
     * @param color1 First color
     * @param color2 Second color
     * @param ratio Mix ratio (0.0-1.0)
     * @return uint32_t Mixed color
     */
    uint32_t mix(uint32_t color1, uint32_t color2, float ratio);
    
    /**
     * @brief Adjust color brightness
     * @param color Original color
     * @param factor Brightness factor (1.0 = no change)
     * @return uint32_t Adjusted color
     */
    uint32_t brighten(uint32_t color, float factor);
    
    /**
     * @brief Make color transparent
     * @param color Original color
     * @param alpha New alpha value (0-255)
     * @return uint32_t Color with new alpha
     */
    uint32_t transparent(uint32_t color, uint8_t alpha);
}
```

## Input System

### Input Structures

```cpp
enum class KeyCode {
    // Letters
    A = 65, B = 66, C = 67, D = 68, E = 69, F = 70, G = 71, H = 72,
    I = 73, J = 74, K = 75, L = 76, M = 77, N = 78, O = 79, P = 80,
    Q = 81, R = 82, S = 83, T = 84, U = 85, V = 86, W = 87, X = 88,
    Y = 89, Z = 90,
    
    // Numbers
    Num0 = 48, Num1 = 49, Num2 = 50, Num3 = 51, Num4 = 52,
    Num5 = 53, Num6 = 54, Num7 = 55, Num8 = 56, Num9 = 57,
    
    // Special keys
    Space = 32, Enter = 13, Tab = 9, Escape = 27, Backspace = 8,
    Delete = 127, Insert = 45, Home = 36, End = 35, PageUp = 33, PageDown = 34,
    
    // Arrow keys
    ArrowLeft = 37, ArrowUp = 38, ArrowRight = 39, ArrowDown = 40,
    
    // Function keys
    F1 = 112, F2 = 113, F3 = 114, F4 = 115, F5 = 116, F6 = 117,
    F7 = 118, F8 = 119, F9 = 120, F10 = 121, F11 = 122, F12 = 123,
    
    // Modifier keys
    Shift = 16, Ctrl = 17, Alt = 18, Meta = 91
};

struct InputState {
    // Mouse state
    int mouseX;                    ///< Current mouse X position
    int mouseY;                    ///< Current mouse Y position
    bool mouseLeftPressed;         ///< Left button just pressed this frame
    bool mouseLeftReleased;        ///< Left button just released this frame
    bool mouseLeftHeld;            ///< Left button currently held down
    bool mouseRightPressed;        ///< Right button just pressed this frame
    bool mouseRightReleased;       ///< Right button just released this frame
    bool mouseRightHeld;           ///< Right button currently held down
    
    // Keyboard state
    bool keyPressed[256];          ///< Keys just pressed this frame
    bool keyReleased[256];         ///< Keys just released this frame
    bool keyHeld[256];             ///< Keys currently held down
    
    // Text input
    std::string textInput;         ///< Text entered this frame
    
    // Utility methods
    /**
     * @brief Check if a specific key was just pressed
     * @param key Key code to check
     * @return bool True if key was pressed this frame
     */
    bool isKeyPressed(KeyCode key) const;
    
    /**
     * @brief Check if a specific key was just released
     * @param key Key code to check
     * @return bool True if key was released this frame
     */
    bool isKeyReleased(KeyCode key) const;
    
    /**
     * @brief Check if a specific key is currently held
     * @param key Key code to check
     * @return bool True if key is currently held
     */
    bool isKeyHeld(KeyCode key) const;
    
    /**
     * @brief Check if mouse is within a rectangle
     * @param x Rectangle X position
     * @param y Rectangle Y position
     * @param width Rectangle width
     * @param height Rectangle height
     * @return bool True if mouse is inside rectangle
     */
    bool isMouseInRect(int x, int y, int width, int height) const;
};
```

### Signal System

```cpp
template<typename... Args>
class Signal {
public:
    /**
     * @brief Connect a callback function to this signal
     * @param callback Function to call when signal is emitted
     */
    void connect(std::function<void(Args...)> callback);
    
    /**
     * @brief Emit the signal, calling all connected callbacks
     * @param args Arguments to pass to callbacks
     */
    void emit(Args... args);
    
    /**
     * @brief Disconnect all callbacks
     */
    void clear();
    
    /**
     * @brief Get number of connected callbacks
     * @return size_t Number of connections
     */
    size_t getConnectionCount() const;
};

// Common signal types
using VoidSignal = Signal<>;
using BoolSignal = Signal<bool>;
using IntSignal = Signal<int>;
using FloatSignal = Signal<float>;
using StringSignal = Signal<const std::string&>;
```

## Platform API

### Platform Detection

```cpp
namespace Fern::Platform {
    /**
     * @brief Get current platform name
     * @return std::string Platform identifier
     */
    std::string getName();
    
    /**
     * @brief Check if running on web platform
     * @return bool True if web platform
     */
    bool isWeb();
    
    /**
     * @brief Check if running on desktop platform
     * @return bool True if desktop platform
     */
    bool isDesktop();
    
    /**
     * @brief Check if running on mobile platform
     * @return bool True if mobile platform
     */
    bool isMobile();
    
    /**
     * @brief Get platform capabilities
     * @return PlatformCapabilities Structure with capability flags
     */
    struct PlatformCapabilities {
        bool hasFileSystem;
        bool hasNetworking;
        bool hasAudio;
        bool hasClipboard;
        bool supportsFullscreen;
        int maxTextureSize;
    };
    
    PlatformCapabilities getCapabilities();
}
```

## Utility Classes

### Point Structure

```cpp
struct Point {
    int x;  ///< X coordinate
    int y;  ///< Y coordinate
    
    /**
     * @brief Default constructor (0, 0)
     */
    Point();
    
    /**
     * @brief Construct point with coordinates
     * @param x X coordinate
     * @param y Y coordinate
     */
    Point(int x, int y);
    
    // Operators
    Point operator+(const Point& other) const;
    Point operator-(const Point& other) const;
    Point& operator+=(const Point& other);
    Point& operator-=(const Point& other);
    bool operator==(const Point& other) const;
    bool operator!=(const Point& other) const;
    
    /**
     * @brief Calculate distance to another point
     * @param other Other point
     * @return float Distance between points
     */
    float distanceTo(const Point& other) const;
    
    /**
     * @brief Calculate squared distance (faster than distanceTo)
     * @param other Other point
     * @return float Squared distance
     */
    float distanceSquaredTo(const Point& other) const;
};
```

### Font Types

```cpp
enum class FontType {
    Bitmap,  ///< Bitmap font (pixelated, small sizes)
    TTF      ///< TrueType font (smooth, scalable)
};

namespace FontManager {
    /**
     * @brief Load TTF font from file
     * @param path Path to TTF font file
     * @param name Font name for reference
     * @return bool True if loaded successfully
     */
    bool loadTTFFont(const std::string& path, const std::string& name);
    
    /**
     * @brief Check if TTF font is available
     * @param name Font name
     * @return bool True if font is available
     */
    bool isTTFFontAvailable(const std::string& name);
    
    /**
     * @brief Get list of available TTF fonts
     * @return std::vector<std::string> List of font names
     */
    std::vector<std::string> getAvailableTTFFonts();
}
```

## Constants and Enums

### Build Information

```cpp
namespace Fern::Version {
    constexpr int MAJOR = 1;
    constexpr int MINOR = 0;
    constexpr int PATCH = 0;
    constexpr const char* STRING = "1.0.0";
    constexpr const char* BUILD_DATE = __DATE__;
    constexpr const char* BUILD_TIME = __TIME__;
}
```

### Configuration Constants

```cpp
namespace Fern::Config {
    constexpr int DEFAULT_WINDOW_WIDTH = 800;
    constexpr int DEFAULT_WINDOW_HEIGHT = 600;
    constexpr int MAX_WIDGETS = 10000;
    constexpr int MAX_TEXT_LENGTH = 1024;
    constexpr float PI = 3.14159265359f;
    constexpr float TAU = 6.28318530718f;
}
```

---

This API reference provides comprehensive documentation for all public APIs in the Fern UI Framework. For examples and tutorials, see the [Examples Documentation](examples/) and [Getting Started Guide](getting-started.md).

## Related Documentation

- [Getting Started Guide](getting-started.md)
- [Widget Documentation](widgets/)
- [Layout System](layout/)
- [Examples](examples/)
- [Platform Guides](platform/)
