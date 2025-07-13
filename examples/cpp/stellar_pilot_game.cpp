#include <fern/fern.hpp>
#include <memory>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <ctime>

using namespace Fern;

// Game: "Stellar Pilot" - Navigate through space while managing ship systems
enum class GameState {
    Menu,
    Playing,
    GameOver
};

struct Obstacle {
    float x, y;
    float size;
    uint32_t color;
    float speed;
};

struct Star {
    float x, y;
    float brightness;
    float twinklePhase;
};

struct Ship {
    float x = 400.0f;
    float y = 300.0f;
    float fuel = 100.0f;
    float shield = 100.0f;
    float speed = 2.0f;
    bool thrusting = false;
};

// Game variables
GameState currentState = GameState::Menu;
Ship player;
std::vector<Obstacle> obstacles;
std::vector<Star> stars;
float gameTime = 0.0f;
int score = 0;
int level = 1;
float lastObstacleSpawn = 0.0f;
std::mt19937 rng(std::time(nullptr));

// UI Widgets (minimal set for clean gameplay)
std::shared_ptr<SliderWidget> thrustSlider;
std::shared_ptr<ProgressBarWidget> fuelBar;
std::shared_ptr<CircularIndicatorWidget> shieldIndicator;
std::shared_ptr<DropdownWidget> difficultyDropdown;

// Initialize background stars
void initStars() {
    stars.clear();
    std::uniform_real_distribution<float> xDist(0, 800);
    std::uniform_real_distribution<float> yDist(0, 600);
    std::uniform_real_distribution<float> brightDist(0.3f, 1.0f);
    std::uniform_real_distribution<float> phaseDist(0, 6.28f);
    
    for (int i = 0; i < 100; i++) {
        stars.push_back({
            xDist(rng), yDist(rng), 
            brightDist(rng), phaseDist(rng)
        });
    }
}

void setupGame() {
    WidgetManager::getInstance().clear();
    
    // Thrust control slider (positioned at bottom, won't overlap with game area)
    thrustSlider = Slider(
        SliderConfig(50, 520, 200, 25)
            .range(0.0f, 100.0f)
            .initialValue(50.0f)
            .style(SliderStyle()
                .trackColor(Colors::DarkGray)
                .fillColor(Colors::Orange)
                .thumbColor(Colors::White)
                .showValue(true)
                .textColor(Colors::White)),
        true
    );
    
    // Fuel bar (top left, clear of game action)
    fuelBar = ProgressBar(
        ProgressBarConfig(10, 10, 150, 15)
            .range(0.0f, 100.0f)
            .value(100.0f)
            .style(ProgressBarStyle()
                .backgroundColor(Colors::DarkGray)
                .fillColor(Colors::Green)
                .borderColor(Colors::White)
                .borderWidth(1)
                .showPercentage(false)),
        true
    );
    
    // Shield indicator (top right corner, out of the way)
    shieldIndicator = CircularIndicator(
        CircularIndicatorConfig(720, 40, 35)
            .range(0.0f, 100.0f)
            .value(100.0f)
            .style(CircularIndicatorStyle()
                .backgroundColor(Colors::DarkGray)
                .fillColor(Colors::Cyan)
                .thickness(4)
                .showPercentage(false)),
        true
    );
    
    // Difficulty dropdown (bottom right, for between-game adjustments)
    std::vector<DropdownItem> difficulties = {
        DropdownItem("Easy"),
        DropdownItem("Normal"),
        DropdownItem("Hard"),
        DropdownItem("Insane")
    };
    
    difficultyDropdown = Dropdown(
        DropdownConfig(600, 520, 120, 25)
            .placeholder("Normal")
            .items(difficulties)
            .selectedIndex(1)
            .style(DropdownStyle()
                .backgroundColor(Colors::DarkGray)
                .borderColor(Colors::Gray)
                .selectedColor(Colors::Purple)
                .textColor(Colors::White)
                .fontSize(1)),
        true
    );
    
    // Event handlers
    thrustSlider->onValueChanged.connect([](float value) {
        player.speed = 1.0f + (value / 100.0f) * 4.0f; // Speed range: 1-5
        player.fuel -= value * 0.001f; // Higher thrust = more fuel consumption
        player.thrusting = value > 10.0f;
    });
    
    difficultyDropdown->onSelectionChanged.connect([](int index) {
        // Difficulty affects obstacle spawn rate and size
        level = index + 1;
    });
    
    // Initialize game state
    initStars();
    player = Ship();
    obstacles.clear();
    gameTime = 0.0f;
    score = 0;
    lastObstacleSpawn = 0.0f;
}

void spawnObstacle() {
    std::uniform_real_distribution<float> yDist(50, 450);
    std::uniform_real_distribution<float> sizeDist(20, 60);
    std::uniform_real_distribution<float> speedDist(1.0f, 3.0f + level * 0.5f);
    std::uniform_int_distribution<int> colorChoice(0, 3);
    
    uint32_t colors[] = {Colors::Red, Colors::Orange, Colors::Purple, Colors::Magenta};
    
    obstacles.push_back({
        800.0f, // Start from right edge
        yDist(rng),
        sizeDist(rng),
        colors[colorChoice(rng)],
        speedDist(rng)
    });
}

void updateGame() {
    if (currentState != GameState::Playing) return;
    
    gameTime += 0.016f;
    
    // Handle input for ship movement
    const auto& input = Input::getState();
    
    if (input.keyPressed || input.mouseDown) {
        // Mouse or WASD movement
        if (input.lastKeyPressed == KeyCode::W || input.lastKeyPressed == KeyCode::ArrowUp) {
            player.y -= player.speed * 2.0f;
        }
        if (input.lastKeyPressed == KeyCode::S || input.lastKeyPressed == KeyCode::ArrowDown) {
            player.y += player.speed * 2.0f;
        }
        if (input.lastKeyPressed == KeyCode::A || input.lastKeyPressed == KeyCode::ArrowLeft) {
            player.x -= player.speed * 2.0f;
        }
        if (input.lastKeyPressed == KeyCode::D || input.lastKeyPressed == KeyCode::ArrowRight) {
            player.x += player.speed * 2.0f;
        }
        
        // Mouse movement
        if (input.mouseDown) {
            float dx = input.mouseX - player.x;
            float dy = input.mouseY - player.y;
            float distance = sqrt(dx*dx + dy*dy);
            if (distance > 5.0f) {
                player.x += (dx / distance) * player.speed;
                player.y += (dy / distance) * player.speed;
            }
        }
    }
    
    // Keep player in bounds (leave space for UI)
    player.x = std::max(20.0f, std::min(780.0f, player.x));
    player.y = std::max(80.0f, std::min(500.0f, player.y));
    
    // Spawn obstacles
    float spawnRate = 1.5f - (level * 0.1f);
    spawnRate = std::max(0.3f, spawnRate);
    
    if (gameTime - lastObstacleSpawn > spawnRate) {
        spawnObstacle();
        lastObstacleSpawn = gameTime;
    }
    
    // Update obstacles
    for (auto& obstacle : obstacles) {
        obstacle.x -= obstacle.speed * 60.0f * 0.016f; // Move left
    }
    
    // Remove off-screen obstacles and award points
    obstacles.erase(
        std::remove_if(obstacles.begin(), obstacles.end(), [](const Obstacle& obs) {
            if (obs.x < -obs.size) {
                score += 10;
                return true;
            }
            return false;
        }),
        obstacles.end()
    );
    
    // Check collisions
    for (const auto& obstacle : obstacles) {
        float dx = player.x - obstacle.x;
        float dy = player.y - obstacle.y;
        float distance = sqrt(dx*dx + dy*dy);
        
        if (distance < obstacle.size/2 + 15) { // Ship size is ~15
            player.shield -= 25.0f;
            if (player.shield <= 0.0f) {
                currentState = GameState::GameOver;
            }
        }
    }
    
    // Update stars (parallax effect)
    for (auto& star : stars) {
        star.x -= player.speed * 0.1f;
        if (star.x < 0) star.x = 800;
        star.twinklePhase += 0.02f;
    }
    
    // Gradual fuel consumption
    player.fuel -= 0.5f;
    if (player.fuel <= 0.0f) {
        currentState = GameState::GameOver;
    }
    
    // Shield regeneration when not hit
    player.shield = std::min(100.0f, player.shield + 0.1f);
    
    // Update UI widgets
    fuelBar->setValue(player.fuel);
    shieldIndicator->setValue(player.shield);
    
    // Level progression
    if (score > 0 && score % 200 == 0 && score > level * 200 - 200) {
        level++;
    }
}

void drawGame() {
    // Starfield background
    Draw::fill(0xFF0A0A15); // Deep space blue-black
    
    // Draw twinkling stars
    for (const auto& star : stars) {
        float brightness = star.brightness * (0.7f + 0.3f * sin(star.twinklePhase));
        uint32_t starColor = Colors::White;
        if (brightness > 0.8f) starColor = Colors::LightBlue;
        else if (brightness < 0.4f) starColor = Colors::Gray;
        
        Draw::circle(star.x, star.y, 1, starColor);
    }
    
    // Draw obstacles with glowing effect
    for (const auto& obstacle : obstacles) {
        // Glow effect
        Draw::circle(obstacle.x, obstacle.y, obstacle.size/2 + 5, 
                    (obstacle.color & 0x00FFFFFF) | 0x30000000);
        // Main obstacle
        Draw::circle(obstacle.x, obstacle.y, obstacle.size/2, obstacle.color);
        // Inner highlight
        Draw::circle(obstacle.x - obstacle.size/6, obstacle.y - obstacle.size/6, 
                    obstacle.size/4, Colors::White);
    }
    
    // Draw player ship with thrust effect
    uint32_t shipColor = player.shield > 50 ? Colors::White : 
                        player.shield > 25 ? Colors::Yellow : Colors::Red;
    
    // Ship body
    Draw::circle(player.x, player.y, 15, shipColor);
    Draw::circle(player.x, player.y, 12, Colors::LightBlue);
    Draw::circle(player.x, player.y, 8, Colors::Blue);
    
    // Thrust trail effect
    if (player.thrusting) {
        for (int i = 1; i <= 5; i++) {
            float alpha = (6 - i) / 6.0f;
            uint32_t thrustColor = (Colors::Orange & 0x00FFFFFF) | 
                                  (static_cast<uint32_t>(alpha * 255) << 24);
            Draw::circle(player.x - i * 8, player.y, 8 - i, thrustColor);
        }
    }
    
    // UI Text (positioned to avoid widget areas)
    Text(Point(10, 30), "FUEL", 1.0, Colors::Green, true);
    Text(Point(650, 15), "SHIELD", 1.0, Colors::Cyan, true);
    
    Text(Point(300, 15), "STELLAR PILOT", 2.0, Colors::White, true);
    Text(Point(300, 40), "Score: " + std::to_string(score), 1.2, Colors::Yellow, true);
    Text(Point(500, 40), "Level: " + std::to_string(level), 1.2, Colors::Orange, true);
    
    Text(Point(50, 500), "Thrust:", 1.0, Colors::Orange, true);
    Text(Point(600, 500), "Difficulty:", 1.0, Colors::Purple, true);
    
    // Instructions (bottom center, away from widgets)
    Text(Point(280, 570), "WASD/Arrow Keys or Mouse to move", 1.0, Colors::LightGray, true);
}

void drawMenu() {
    Draw::fill(0xFF0A0A15);
    
    // Title with glow effect
    Text(Point(250, 200), "STELLAR PILOT", 4.0, Colors::Cyan, true);
    Text(Point(252, 202), "STELLAR PILOT", 4.0, Colors::Blue, true); // Shadow effect
    
    Text(Point(200, 280), "Navigate through the asteroid field!", 1.8, Colors::White, true);
    Text(Point(150, 320), "• Use WASD/Arrow keys or mouse to move your ship", 1.2, Colors::LightGray, true);
    Text(Point(150, 340), "• Adjust thrust slider to control speed and fuel consumption", 1.2, Colors::LightGray, true);
    Text(Point(150, 360), "• Avoid asteroids to keep your shields up", 1.2, Colors::LightGray, true);
    Text(Point(150, 380), "• Don't run out of fuel!", 1.2, Colors::LightGray, true);
    
    Text(Point(300, 450), "Press SPACE to start", 2.0, Colors::Yellow, true);
    Text(Point(320, 480), "ESC to quit", 1.2, Colors::Gray, true);
}

void drawGameOver() {
    Draw::fill(0xFF150A0A); // Reddish tint
    
    Text(Point(280, 200), "MISSION FAILED", 3.0, Colors::Red, true);
    Text(Point(250, 260), "Final Score: " + std::to_string(score), 2.0, Colors::Yellow, true);
    Text(Point(280, 300), "Level Reached: " + std::to_string(level), 1.5, Colors::Orange, true);
    
    std::string message;
    if (score >= 500) message = "Excellent piloting!";
    else if (score >= 300) message = "Good job, pilot!";
    else if (score >= 100) message = "Not bad for a rookie!";
    else message = "Keep practicing!";
    
    Text(Point(320, 350), message, 1.5, Colors::Cyan, true);
    Text(Point(300, 420), "Press SPACE to try again", 1.5, Colors::White, true);
    Text(Point(330, 450), "ESC to quit", 1.2, Colors::Gray, true);
}

void handleInput() {
    const auto& input = Input::getState();
    
    if (input.keyPressed) {
        switch (input.lastKeyPressed) {
            case KeyCode::Space:
                if (currentState == GameState::Menu || currentState == GameState::GameOver) {
                    currentState = GameState::Playing;
                    setupGame(); // Reset game state
                }
                break;
                
            case KeyCode::Escape:
                if (currentState == GameState::Playing) {
                    currentState = GameState::GameOver;
                } else if (currentState == GameState::Menu || currentState == GameState::GameOver) {
                    // Could add proper exit handling here
                }
                break;
        }
    }
}

void draw() {
    switch (currentState) {
        case GameState::Menu:
            drawMenu();
            break;
            
        case GameState::Playing:
            updateGame();
            drawGame();
            break;
            
        case GameState::GameOver:
            drawGameOver();
            break;
    }
    
    handleInput();
}

int main() {
    Fern::initialize();
    setupGame();
    
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    
    return 0;
}
