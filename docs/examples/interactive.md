# Interactive Examples

## Overview

This guide demonstrates advanced interactive features of the Fern UI Framework. These examples show how to create engaging, responsive applications with complex user interactions, real-time updates, and sophisticated UI patterns.

## Table of Contents

1. [Interactive Dashboard](#interactive-dashboard)
2. [Drawing Application](#drawing-application)
3. [Game-like Interface](#game-like-interface)
4. [Data Visualization](#data-visualization)
5. [Form Builder](#form-builder)
6. [Real-time Chat Interface](#real-time-chat-interface)
7. [Image Editor](#image-editor)
8. [Audio Visualizer](#audio-visualizer)
9. [3D-like Effects](#3d-like-effects)
10. [Complete Interactive App](#complete-interactive-app)

## Interactive Dashboard

A comprehensive dashboard with multiple interactive widgets:

```cpp
#include <fern/fern.hpp>
#include <chrono>
#include <vector>
#include <random>

class InteractiveDashboard {
private:
    struct Widget {
        int x, y, width, height;
        std::string title;
        uint32_t color;
        bool active;
        
        Widget(int x, int y, int w, int h, const std::string& t, uint32_t c)
            : x(x), y(y), width(w), height(h), title(t), color(c), active(false) {}
        
        bool contains(int mx, int my) const {
            return mx >= x && mx <= x + width && my >= y && my <= y + height;
        }
    };
    
    std::vector<Widget> widgets;
    std::vector<float> chartData;
    float time = 0.0f;
    int selectedWidget = -1;
    bool isDragging = false;
    int dragOffsetX = 0, dragOffsetY = 0;
    
public:
    InteractiveDashboard() {
        // Initialize widgets
        widgets.emplace_back(50, 50, 200, 150, "CPU Usage", Fern::Colors::BLUE);
        widgets.emplace_back(270, 50, 200, 150, "Memory", Fern::Colors::GREEN);
        widgets.emplace_back(490, 50, 200, 150, "Network", Fern::Colors::RED);
        widgets.emplace_back(50, 220, 200, 150, "Temperature", Fern::Colors::YELLOW);
        widgets.emplace_back(270, 220, 420, 150, "Performance Chart", Fern::Colors::CYAN);
        
        // Initialize chart data
        for (int i = 0; i < 50; i++) {
            chartData.push_back(0.5f + 0.3f * std::sin(i * 0.1f));
        }
    }
    
    void update(float deltaTime) {
        time += deltaTime;
        
        // Update chart data
        if (static_cast<int>(time * 10) % 5 == 0) {
            chartData.erase(chartData.begin());
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> dis(0.2, 0.8);
            chartData.push_back(dis(gen));
        }
    }
    
    void draw() {
        Fern::Canvas::fill(0xFF1a1a1a);
        
        // Draw title
        Fern::Canvas::drawText("Interactive Dashboard", 10, 10, Fern::Colors::WHITE);
        
        // Draw widgets
        for (size_t i = 0; i < widgets.size(); i++) {
            const auto& widget = widgets[i];
            
            // Widget background
            uint32_t bgColor = widget.active ? 0xFF333333 : 0xFF2a2a2a;
            if (selectedWidget == static_cast<int>(i)) {
                bgColor = 0xFF404040;
            }
            
            Fern::Canvas::drawRect(widget.x, widget.y, widget.width, widget.height, bgColor);
            
            // Widget border
            uint32_t borderColor = widget.active ? widget.color : 0xFF666666;
            drawBorder(widget.x, widget.y, widget.width, widget.height, borderColor);
            
            // Widget title
            Fern::Canvas::drawText(widget.title, widget.x + 10, widget.y + 10, Fern::Colors::WHITE);
            
            // Widget content
            drawWidgetContent(widget, i);
        }
        
        // Draw instructions
        Fern::Canvas::drawText("Click widgets to select, drag to move", 10, 550, Fern::Colors::GRAY);
        Fern::Canvas::drawText("Hover over widgets to activate", 10, 570, Fern::Colors::GRAY);
    }
    
    void handleMouseMove(int x, int y) {
        if (isDragging && selectedWidget >= 0) {
            // Move selected widget
            widgets[selectedWidget].x = x - dragOffsetX;
            widgets[selectedWidget].y = y - dragOffsetY;
            return;
        }
        
        // Check hover states
        for (auto& widget : widgets) {
            widget.active = widget.contains(x, y);
        }
    }
    
    void handleClick(int x, int y, bool down) {
        if (down) {
            // Find clicked widget
            for (size_t i = 0; i < widgets.size(); i++) {
                if (widgets[i].contains(x, y)) {
                    selectedWidget = i;
                    isDragging = true;
                    dragOffsetX = x - widgets[i].x;
                    dragOffsetY = y - widgets[i].y;
                    return;
                }
            }
            selectedWidget = -1;
        } else {
            isDragging = false;
        }
    }
    
private:
    void drawBorder(int x, int y, int width, int height, uint32_t color) {
        Fern::Canvas::drawRect(x, y, width, 2, color);
        Fern::Canvas::drawRect(x, y + height - 2, width, 2, color);
        Fern::Canvas::drawRect(x, y, 2, height, color);
        Fern::Canvas::drawRect(x + width - 2, y, 2, height, color);
    }
    
    void drawWidgetContent(const Widget& widget, int index) {
        int contentY = widget.y + 35;
        
        switch (index) {
            case 0: // CPU Usage
                drawProgressBar(widget.x + 10, contentY, widget.width - 20, 20, 
                               0.7f + 0.2f * std::sin(time * 2), Fern::Colors::BLUE);
                Fern::Canvas::drawText("70%", widget.x + 10, contentY + 30, Fern::Colors::WHITE);
                break;
                
            case 1: // Memory
                drawProgressBar(widget.x + 10, contentY, widget.width - 20, 20, 
                               0.45f, Fern::Colors::GREEN);
                Fern::Canvas::drawText("45%", widget.x + 10, contentY + 30, Fern::Colors::WHITE);
                break;
                
            case 2: // Network
                drawNetworkIndicator(widget.x + 10, contentY, widget.width - 20, 80);
                break;
                
            case 3: // Temperature
                drawTemperatureGauge(widget.x + widget.width/2, contentY + 40, 40);
                break;
                
            case 4: // Performance Chart
                drawChart(widget.x + 10, contentY, widget.width - 20, 100);
                break;
        }
    }
    
    void drawProgressBar(int x, int y, int width, int height, float progress, uint32_t color) {
        // Background
        Fern::Canvas::drawRect(x, y, width, height, 0xFF333333);
        
        // Progress
        int progressWidth = static_cast<int>(width * progress);
        Fern::Canvas::drawRect(x, y, progressWidth, height, color);
        
        // Border
        drawBorder(x, y, width, height, 0xFF666666);
    }
    
    void drawNetworkIndicator(int x, int y, int width, int height) {
        // Draw network activity bars
        for (int i = 0; i < 8; i++) {
            int barX = x + i * (width / 8);
            int barHeight = static_cast<int>(height * (0.3f + 0.7f * std::sin(time * 3 + i * 0.5f)));
            
            uint32_t color = (i % 2 == 0) ? Fern::Colors::RED : 0xFF800000;
            Fern::Canvas::drawRect(barX, y + height - barHeight, width / 10, barHeight, color);
        }
    }
    
    void drawTemperatureGauge(int centerX, int centerY, int radius) {
        // Draw temperature gauge
        Fern::Canvas::drawCircle(centerX, centerY, radius, 0xFF333333);
        
        // Temperature needle
        float temp = 65.0f + 10.0f * std::sin(time * 1.5f);
        float angle = (temp - 30.0f) / 70.0f * 3.14159f; // 30-100°C range
        
        int needleX = centerX + static_cast<int>(std::cos(angle) * radius * 0.8f);
        int needleY = centerY - static_cast<int>(std::sin(angle) * radius * 0.8f);
        
        Fern::Canvas::drawLine(centerX, centerY, needleX, needleY, 2, Fern::Colors::RED);
        
        // Temperature text
        std::string tempText = std::to_string(static_cast<int>(temp)) + "°C";
        Fern::Canvas::drawText(tempText, centerX - 15, centerY + radius + 10, Fern::Colors::WHITE);
    }
    
    void drawChart(int x, int y, int width, int height) {
        // Draw chart background
        Fern::Canvas::drawRect(x, y, width, height, 0xFF222222);
        
        // Draw chart lines
        for (size_t i = 1; i < chartData.size(); i++) {
            int x1 = x + (i - 1) * width / chartData.size();
            int y1 = y + height - static_cast<int>(chartData[i - 1] * height);
            int x2 = x + i * width / chartData.size();
            int y2 = y + height - static_cast<int>(chartData[i] * height);
            
            Fern::Canvas::drawLine(x1, y1, x2, y2, 2, Fern::Colors::CYAN);
        }
        
        // Draw chart border
        drawBorder(x, y, width, height, 0xFF666666);
    }
};
```

## Drawing Application

A pixel art drawing application with tools and palettes:

```cpp
#include <fern/fern.hpp>
#include <vector>

class DrawingApp {
private:
    struct Tool {
        std::string name;
        uint32_t color;
        int size;
        bool selected;
        
        Tool(const std::string& n, uint32_t c, int s = 1) 
            : name(n), color(c), size(s), selected(false) {}
    };
    
    std::vector<std::vector<uint32_t>> canvas;
    std::vector<Tool> tools;
    std::vector<uint32_t> palette;
    
    int canvasSize = 32;
    int pixelSize = 10;
    int canvasStartX = 50;
    int canvasStartY = 50;
    
    uint32_t currentColor = Fern::Colors::WHITE;
    int currentTool = 0;
    bool isDrawing = false;
    
public:
    DrawingApp() {
        // Initialize canvas
        canvas.resize(canvasSize, std::vector<uint32_t>(canvasSize, 0xFF000000));
        
        // Initialize tools
        tools.emplace_back("Brush", Fern::Colors::WHITE, 1);
        tools.emplace_back("Eraser", Fern::Colors::BLACK, 2);
        tools.emplace_back("Fill", Fern::Colors::BLUE, 1);
        tools[0].selected = true;
        
        // Initialize palette
        palette = {
            0xFF000000, 0xFFFFFFFF, 0xFFFF0000, 0xFF00FF00,
            0xFF0000FF, 0xFFFFFF00, 0xFFFF00FF, 0xFF00FFFF,
            0xFF800000, 0xFF008000, 0xFF000080, 0xFF808000,
            0xFF800080, 0xFF008080, 0xFF808080, 0xFFC0C0C0
        };
    }
    
    void draw() {
        Fern::Canvas::fill(0xFF1a1a1a);
        
        // Draw title
        Fern::Canvas::drawText("Drawing Application", 10, 10, Fern::Colors::WHITE);
        
        // Draw canvas
        drawCanvas();
        
        // Draw tools
        drawTools();
        
        // Draw palette
        drawPalette();
        
        // Draw instructions
        drawInstructions();
    }
    
    void handleMouseMove(int x, int y) {
        if (isDrawing) {
            drawPixel(x, y);
        }
    }
    
    void handleClick(int x, int y, bool down) {
        if (down) {
            // Check if clicking on canvas
            if (isOnCanvas(x, y)) {
                isDrawing = true;
                drawPixel(x, y);
            }
            // Check if clicking on tools
            else if (isOnTools(x, y)) {
                selectTool(x, y);
            }
            // Check if clicking on palette
            else if (isOnPalette(x, y)) {
                selectColor(x, y);
            }
        } else {
            isDrawing = false;
        }
    }
    
private:
    void drawCanvas() {
        // Draw canvas background
        int canvasPixelSize = canvasSize * pixelSize;
        Fern::Canvas::drawRect(canvasStartX - 2, canvasStartY - 2, 
                              canvasPixelSize + 4, canvasPixelSize + 4, 
                              Fern::Colors::WHITE);
        
        // Draw pixels
        for (int y = 0; y < canvasSize; y++) {
            for (int x = 0; x < canvasSize; x++) {
                int pixelX = canvasStartX + x * pixelSize;
                int pixelY = canvasStartY + y * pixelSize;
                
                Fern::Canvas::drawRect(pixelX, pixelY, pixelSize, pixelSize, 
                                      canvas[y][x]);
            }
        }
        
        // Draw grid
        for (int i = 0; i <= canvasSize; i++) {
            int lineX = canvasStartX + i * pixelSize;
            int lineY = canvasStartY + i * pixelSize;
            
            Fern::Canvas::drawLine(lineX, canvasStartY, lineX, 
                                  canvasStartY + canvasPixelSize, 1, 0xFF333333);
            Fern::Canvas::drawLine(canvasStartX, lineY, 
                                  canvasStartX + canvasPixelSize, lineY, 1, 0xFF333333);
        }
    }
    
    void drawTools() {
        int toolsStartX = canvasStartX + canvasSize * pixelSize + 20;
        int toolsStartY = canvasStartY;
        
        Fern::Canvas::drawText("Tools:", toolsStartX, toolsStartY - 20, Fern::Colors::WHITE);
        
        for (size_t i = 0; i < tools.size(); i++) {
            int toolY = toolsStartY + i * 40;
            
            // Tool background
            uint32_t bgColor = tools[i].selected ? 0xFF444444 : 0xFF222222;
            Fern::Canvas::drawRect(toolsStartX, toolY, 120, 35, bgColor);
            
            // Tool border
            uint32_t borderColor = tools[i].selected ? tools[i].color : 0xFF666666;
            drawBorder(toolsStartX, toolY, 120, 35, borderColor);
            
            // Tool name
            Fern::Canvas::drawText(tools[i].name, toolsStartX + 5, toolY + 10, Fern::Colors::WHITE);
            
            // Tool size indicator
            int sizeX = toolsStartX + 80;
            int sizeY = toolY + 17;
            Fern::Canvas::drawCircle(sizeX, sizeY, tools[i].size * 2, tools[i].color);
        }
    }
    
    void drawPalette() {
        int paletteStartX = canvasStartX;
        int paletteStartY = canvasStartY + canvasSize * pixelSize + 20;
        
        Fern::Canvas::drawText("Palette:", paletteStartX, paletteStartY - 20, Fern::Colors::WHITE);
        
        for (size_t i = 0; i < palette.size(); i++) {
            int col = i % 8;
            int row = i / 8;
            
            int colorX = paletteStartX + col * 25;
            int colorY = paletteStartY + row * 25;
            
            Fern::Canvas::drawRect(colorX, colorY, 20, 20, palette[i]);
            
            // Highlight current color
            if (palette[i] == currentColor) {
                drawBorder(colorX, colorY, 20, 20, Fern::Colors::WHITE);
            }
        }
        
        // Current color indicator
        Fern::Canvas::drawText("Current:", paletteStartX + 220, paletteStartY - 20, Fern::Colors::WHITE);
        Fern::Canvas::drawRect(paletteStartX + 220, paletteStartY, 40, 40, currentColor);
    }
    
    void drawInstructions() {
        int instructY = 500;
        Fern::Canvas::drawText("Click and drag to draw", 10, instructY, Fern::Colors::GRAY);
        Fern::Canvas::drawText("Click tools to select", 10, instructY + 20, Fern::Colors::GRAY);
        Fern::Canvas::drawText("Click palette to change color", 10, instructY + 40, Fern::Colors::GRAY);
    }
    
    void drawBorder(int x, int y, int width, int height, uint32_t color) {
        Fern::Canvas::drawRect(x, y, width, 2, color);
        Fern::Canvas::drawRect(x, y + height - 2, width, 2, color);
        Fern::Canvas::drawRect(x, y, 2, height, color);
        Fern::Canvas::drawRect(x + width - 2, y, 2, height, color);
    }
    
    bool isOnCanvas(int x, int y) {
        return x >= canvasStartX && x < canvasStartX + canvasSize * pixelSize &&
               y >= canvasStartY && y < canvasStartY + canvasSize * pixelSize;
    }
    
    bool isOnTools(int x, int y) {
        int toolsStartX = canvasStartX + canvasSize * pixelSize + 20;
        int toolsStartY = canvasStartY;
        
        return x >= toolsStartX && x < toolsStartX + 120 &&
               y >= toolsStartY && y < toolsStartY + tools.size() * 40;
    }
    
    bool isOnPalette(int x, int y) {
        int paletteStartX = canvasStartX;
        int paletteStartY = canvasStartY + canvasSize * pixelSize + 20;
        
        return x >= paletteStartX && x < paletteStartX + 200 &&
               y >= paletteStartY && y < paletteStartY + 50;
    }
    
    void drawPixel(int x, int y) {
        if (!isOnCanvas(x, y)) return;
        
        int pixelX = (x - canvasStartX) / pixelSize;
        int pixelY = (y - canvasStartY) / pixelSize;
        
        if (pixelX >= 0 && pixelX < canvasSize && pixelY >= 0 && pixelY < canvasSize) {
            if (currentTool == 0) { // Brush
                canvas[pixelY][pixelX] = currentColor;
            } else if (currentTool == 1) { // Eraser
                canvas[pixelY][pixelX] = 0xFF000000;
            } else if (currentTool == 2) { // Fill
                floodFill(pixelX, pixelY, canvas[pixelY][pixelX], currentColor);
            }
        }
    }
    
    void selectTool(int x, int y) {
        int toolsStartX = canvasStartX + canvasSize * pixelSize + 20;
        int toolsStartY = canvasStartY;
        
        int toolIndex = (y - toolsStartY) / 40;
        if (toolIndex >= 0 && toolIndex < static_cast<int>(tools.size())) {
            for (auto& tool : tools) {
                tool.selected = false;
            }
            tools[toolIndex].selected = true;
            currentTool = toolIndex;
        }
    }
    
    void selectColor(int x, int y) {
        int paletteStartX = canvasStartX;
        int paletteStartY = canvasStartY + canvasSize * pixelSize + 20;
        
        int col = (x - paletteStartX) / 25;
        int row = (y - paletteStartY) / 25;
        int colorIndex = row * 8 + col;
        
        if (colorIndex >= 0 && colorIndex < static_cast<int>(palette.size())) {
            currentColor = palette[colorIndex];
        }
    }
    
    void floodFill(int x, int y, uint32_t oldColor, uint32_t newColor) {
        if (x < 0 || x >= canvasSize || y < 0 || y >= canvasSize) return;
        if (canvas[y][x] != oldColor || oldColor == newColor) return;
        
        canvas[y][x] = newColor;
        
        floodFill(x + 1, y, oldColor, newColor);
        floodFill(x - 1, y, oldColor, newColor);
        floodFill(x, y + 1, oldColor, newColor);
        floodFill(x, y - 1, oldColor, newColor);
    }
};
```

## Game-like Interface

A simple game interface with score, health, and interactive elements:

```cpp
#include <fern/fern.hpp>
#include <vector>
#include <random>

class GameInterface {
private:
    struct Enemy {
        float x, y;
        float vx, vy;
        uint32_t color;
        bool alive;
        
        Enemy(float x, float y) : x(x), y(y), alive(true) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> speedDis(-50, 50);
            std::uniform_int_distribution<> colorDis(0, 2);
            
            vx = speedDis(gen);
            vy = speedDis(gen);
            
            uint32_t colors[] = {Fern::Colors::RED, Fern::Colors::YELLOW, Fern::Colors::MAGENTA};
            color = colors[colorDis(gen)];
        }
        
        void update(float deltaTime) {
            if (!alive) return;
            
            x += vx * deltaTime;
            y += vy * deltaTime;
            
            // Bounce off walls
            if (x <= 10 || x >= 790) vx = -vx;
            if (y <= 10 || y >= 590) vy = -vy;
            
            x = std::max(10.0f, std::min(790.0f, x));
            y = std::max(10.0f, std::min(590.0f, y));
        }
        
        void draw() {
            if (alive) {
                Fern::Canvas::drawCircle(static_cast<int>(x), static_cast<int>(y), 8, color);
            }
        }
        
        bool checkCollision(float px, float py, float radius) {
            if (!alive) return false;
            
            float dx = x - px;
            float dy = y - py;
            float distance = std::sqrt(dx * dx + dy * dy);
            
            return distance < radius + 8;
        }
    };
    
    struct Projectile {
        float x, y;
        float vx, vy;
        bool alive;
        
        Projectile(float x, float y, float vx, float vy) 
            : x(x), y(y), vx(vx), vy(vy), alive(true) {}
        
        void update(float deltaTime) {
            if (!alive) return;
            
            x += vx * deltaTime;
            y += vy * deltaTime;
            
            // Remove if off screen
            if (x < 0 || x > 800 || y < 0 || y > 600) {
                alive = false;
            }
        }
        
        void draw() {
            if (alive) {
                Fern::Canvas::drawCircle(static_cast<int>(x), static_cast<int>(y), 3, Fern::Colors::CYAN);
            }
        }
    };
    
    float playerX = 400;
    float playerY = 500;
    int health = 100;
    int score = 0;
    int level = 1;
    float gameTime = 0;
    
    std::vector<Enemy> enemies;
    std::vector<Projectile> projectiles;
    
    float enemySpawnTimer = 0;
    float powerupTimer = 0;
    
    bool gameOver = false;
    bool paused = false;
    
public:
    GameInterface() {
        spawnEnemies();
    }
    
    void update(float deltaTime) {
        if (gameOver || paused) return;
        
        gameTime += deltaTime;
        
        // Update enemies
        for (auto& enemy : enemies) {
            enemy.update(deltaTime);
        }
        
        // Update projectiles
        for (auto& projectile : projectiles) {
            projectile.update(deltaTime);
        }
        
        // Check collisions
        checkCollisions();
        
        // Spawn new enemies
        enemySpawnTimer += deltaTime;
        if (enemySpawnTimer > 2.0f) {
            spawnEnemies();
            enemySpawnTimer = 0;
        }
        
        // Clean up dead objects
        enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                     [](const Enemy& e) { return !e.alive; }), enemies.end());
        projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(),
                         [](const Projectile& p) { return !p.alive; }), projectiles.end());
        
        // Check game over
        if (health <= 0) {
            gameOver = true;
        }
        
        // Level progression
        if (score > level * 1000) {
            level++;
        }
    }
    
    void draw() {
        Fern::Canvas::fill(0xFF0a0a0a);
        
        if (gameOver) {
            drawGameOver();
            return;
        }
        
        // Draw game objects
        drawPlayer();
        
        for (auto& enemy : enemies) {
            enemy.draw();
        }
        
        for (auto& projectile : projectiles) {
            projectile.draw();
        }
        
        // Draw UI
        drawUI();
        
        if (paused) {
            drawPauseScreen();
        }
    }
    
    void handleMouseMove(int x, int y) {
        if (!gameOver && !paused) {
            playerX = x;
            playerY = y;
        }
    }
    
    void handleClick(int x, int y, bool down) {
        if (gameOver) {
            if (down) {
                resetGame();
            }
            return;
        }
        
        if (down && !paused) {
            // Shoot projectile
            float dx = x - playerX;
            float dy = y - playerY;
            float length = std::sqrt(dx * dx + dy * dy);
            
            if (length > 0) {
                float speed = 300.0f;
                projectiles.emplace_back(playerX, playerY, 
                                       dx / length * speed, dy / length * speed);
            }
        }
    }
    
    void handleKeyPress(Fern::KeyCode key) {
        if (key == Fern::KeyCode::Space) {
            paused = !paused;
        }
    }
    
private:
    void drawPlayer() {
        // Player body
        Fern::Canvas::drawCircle(static_cast<int>(playerX), static_cast<int>(playerY), 12, Fern::Colors::GREEN);
        
        // Player direction indicator
        auto& input = Fern::Input::getState();
        float dx = input.mouseX - playerX;
        float dy = input.mouseY - playerY;
        float length = std::sqrt(dx * dx + dy * dy);
        
        if (length > 0) {
            float aimX = playerX + (dx / length) * 20;
            float aimY = playerY + (dy / length) * 20;
            
            Fern::Canvas::drawLine(static_cast<int>(playerX), static_cast<int>(playerY),
                                  static_cast<int>(aimX), static_cast<int>(aimY), 2, Fern::Colors::WHITE);
        }
    }
    
    void drawUI() {
        // Health bar
        Fern::Canvas::drawText("Health:", 10, 10, Fern::Colors::WHITE);
        Fern::Canvas::drawRect(70, 10, 200, 20, 0xFF333333);
        
        int healthWidth = (health * 200) / 100;
        uint32_t healthColor = health > 50 ? Fern::Colors::GREEN : 
                              health > 25 ? Fern::Colors::YELLOW : Fern::Colors::RED;
        Fern::Canvas::drawRect(70, 10, healthWidth, 20, healthColor);
        
        // Score
        std::string scoreText = "Score: " + std::to_string(score);
        Fern::Canvas::drawText(scoreText, 10, 40, Fern::Colors::CYAN);
        
        // Level
        std::string levelText = "Level: " + std::to_string(level);
        Fern::Canvas::drawText(levelText, 10, 70, Fern::Colors::YELLOW);
        
        // Time
        std::string timeText = "Time: " + std::to_string(static_cast<int>(gameTime));
        Fern::Canvas::drawText(timeText, 10, 100, Fern::Colors::WHITE);
        
        // Enemy count
        std::string enemyText = "Enemies: " + std::to_string(enemies.size());
        Fern::Canvas::drawText(enemyText, 10, 130, Fern::Colors::RED);
        
        // Instructions
        Fern::Canvas::drawText("Move mouse to aim, click to shoot", 10, 560, Fern::Colors::GRAY);
        Fern::Canvas::drawText("Press SPACE to pause", 10, 580, Fern::Colors::GRAY);
    }
    
    void drawGameOver() {
        Fern::Canvas::fill(0xFF1a0000);
        
        // Game Over text
        Fern::Canvas::drawText("GAME OVER", 300, 200, Fern::Colors::RED);
        
        // Final stats
        std::string finalScore = "Final Score: " + std::to_string(score);
        Fern::Canvas::drawText(finalScore, 300, 250, Fern::Colors::WHITE);
        
        std::string finalLevel = "Level Reached: " + std::to_string(level);
        Fern::Canvas::drawText(finalLevel, 300, 280, Fern::Colors::WHITE);
        
        std::string finalTime = "Time Survived: " + std::to_string(static_cast<int>(gameTime)) + "s";
        Fern::Canvas::drawText(finalTime, 300, 310, Fern::Colors::WHITE);
        
        // Restart instruction
        Fern::Canvas::drawText("Click to restart", 320, 400, Fern::Colors::CYAN);
    }
    
    void drawPauseScreen() {
        // Semi-transparent overlay
        Fern::Canvas::drawRect(0, 0, 800, 600, 0x80000000);
        
        // Pause text
        Fern::Canvas::drawText("PAUSED", 350, 280, Fern::Colors::WHITE);
        Fern::Canvas::drawText("Press SPACE to continue", 300, 320, Fern::Colors::GRAY);
    }
    
    void spawnEnemies() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> xDis(20, 780);
        std::uniform_real_distribution<> yDis(20, 200);
        
        int enemyCount = 2 + level;
        for (int i = 0; i < enemyCount; i++) {
            enemies.emplace_back(xDis(gen), yDis(gen));
        }
    }
    
    void checkCollisions() {
        // Projectile vs Enemy collisions
        for (auto& projectile : projectiles) {
            if (!projectile.alive) continue;
            
            for (auto& enemy : enemies) {
                if (!enemy.alive) continue;
                
                if (enemy.checkCollision(projectile.x, projectile.y, 3)) {
                    enemy.alive = false;
                    projectile.alive = false;
                    score += 100;
                    break;
                }
            }
        }
        
        // Player vs Enemy collisions
        for (auto& enemy : enemies) {
            if (!enemy.alive) continue;
            
            if (enemy.checkCollision(playerX, playerY, 12)) {
                enemy.alive = false;
                health -= 10;
                if (health < 0) health = 0;
            }
        }
    }
    
    void resetGame() {
        playerX = 400;
        playerY = 500;
        health = 100;
        score = 0;
        level = 1;
        gameTime = 0;
        gameOver = false;
        paused = false;
        
        enemies.clear();
        projectiles.clear();
        
        spawnEnemies();
    }
};
```

## Complete Interactive App

Here's the main function that ties everything together:

```cpp
int main() {
    Fern::initialize(800, 600);
    
    // Choose which example to run
    int currentExample = 0; // 0 = Dashboard, 1 = Drawing, 2 = Game
    
    InteractiveDashboard dashboard;
    DrawingApp drawingApp;
    GameInterface gameInterface;
    
    auto lastTime = std::chrono::steady_clock::now();
    
    // Set up input handling
    auto renderer = Fern::createRenderer();
    
    renderer->setMouseCallback([&](int x, int y) {
        switch (currentExample) {
            case 0: dashboard.handleMouseMove(x, y); break;
            case 1: drawingApp.handleMouseMove(x, y); break;
            case 2: gameInterface.handleMouseMove(x, y); break;
        }
    });
    
    renderer->setClickCallback([&](bool down) {
        auto& input = Fern::Input::getState();
        switch (currentExample) {
            case 0: dashboard.handleClick(input.mouseX, input.mouseY, down); break;
            case 1: drawingApp.handleClick(input.mouseX, input.mouseY, down); break;
            case 2: gameInterface.handleClick(input.mouseX, input.mouseY, down); break;
        }
    });
    
    renderer->setKeyCallback([&](Fern::KeyCode key, bool pressed) {
        if (pressed) {
            if (key == Fern::KeyCode::Num1) currentExample = 0;
            else if (key == Fern::KeyCode::Num2) currentExample = 1;
            else if (key == Fern::KeyCode::Num3) currentExample = 2;
            else if (currentExample == 2) gameInterface.handleKeyPress(key);
        }
    });
    
    // Set up rendering
    Fern::setDrawCallback([&]() {
        // Calculate delta time
        auto currentTime = std::chrono::steady_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;
        
        // Update and draw current example
        switch (currentExample) {
            case 0:
                dashboard.update(deltaTime);
                dashboard.draw();
                break;
            case 1:
                drawingApp.draw();
                break;
            case 2:
                gameInterface.update(deltaTime);
                gameInterface.draw();
                break;
        }
        
        // Draw example selector
        Fern::Canvas::drawText("Press 1-3 to switch examples", 10, 10, 0xFF888888);
        
        std::string currentText = "Current: ";
        if (currentExample == 0) currentText += "Dashboard";
        else if (currentExample == 1) currentText += "Drawing App";
        else if (currentExample == 2) currentText += "Game Interface";
        
        Fern::Canvas::drawText(currentText, 200, 10, 0xFF888888);
    });
    
    Fern::startRenderLoop();
    return 0;
}
```

## Key Interactive Concepts

### 1. **State Management**
- Tracking multiple object states
- Managing UI element states
- Handling user interactions

### 2. **Real-time Updates**
- Delta time calculations
- Smooth animations
- Continuous data updates

### 3. **Complex Input Handling**
- Mouse tracking and dragging
- Multi-tool selection
- Contextual interactions

### 4. **Visual Feedback**
- Hover effects
- Selection indicators
- Status updates

### 5. **Game-like Features**
- Collision detection
- Score systems
- Health and status bars

## Performance Tips

1. **Optimize Drawing**: Only redraw changed areas when possible
2. **Efficient Collision Detection**: Use spatial partitioning for many objects
3. **Memory Management**: Remove dead objects regularly
4. **State Caching**: Cache expensive calculations
5. **Frame Rate Control**: Limit updates to maintain smooth performance

## Related Documentation

- [Basic Examples](basic.md)
- [Best Practices](best-practices.md)
- [Widget Documentation](../widgets/)
- [Input Handling](../input/)
- [Performance Guide](../development/performance.md)

---

*These interactive examples demonstrate the power and flexibility of the Fern UI Framework for creating engaging, responsive applications.*
