#include <fern/fern.hpp>
#include <memory>
#include <vector>
#include <cmath>
#include <random>
#include <sstream>
#include <iomanip>

using namespace Fern;

// Game state
enum class GameState {
    Playing,
    GameOver,
    Victory
};

// System health levels
struct SystemHealth {
    float power = 100.0f;
    float oxygen = 100.0f;
    float shields = 100.0f;
    float temperature = 50.0f;  // 0-100, 50 is optimal
    int crew_morale = 100;
    std::string selected_language = "English";
};

// Game variables
GameState gameState = GameState::Playing;
SystemHealth systems;
float gameTime = 0.0f;
int score = 0;
int alertLevel = 0;  // 0=green, 1=yellow, 2=red
std::mt19937 rng(std::time(nullptr));

// Widgets
std::shared_ptr<SliderWidget> powerSlider;
std::shared_ptr<SliderWidget> oxygenSlider;
std::shared_ptr<SliderWidget> temperatureSlider;
std::shared_ptr<ProgressBarWidget> shieldsProgress;
std::shared_ptr<CircularIndicatorWidget> powerIndicator;
std::shared_ptr<CircularIndicatorWidget> oxygenIndicator;
std::shared_ptr<RadioButtonGroup> emergencyModeGroup;
std::shared_ptr<DropdownWidget> languageDropdown;

// Game events
struct GameEvent {
    std::string message;
    uint32_t color;
    float timeRemaining;
};

std::vector<GameEvent> activeEvents;

void addEvent(const std::string& message, uint32_t color, float duration = 3.0f) {
    activeEvents.push_back({message, color, duration});
    if (activeEvents.size() > 5) {
        activeEvents.erase(activeEvents.begin());
    }
}

void updateAlertLevel() {
    int criticalSystems = 0;
    if (systems.power < 30.0f) criticalSystems++;
    if (systems.oxygen < 30.0f) criticalSystems++;
    if (systems.shields < 30.0f) criticalSystems++;
    if (systems.temperature < 20.0f || systems.temperature > 80.0f) criticalSystems++;
    
    if (criticalSystems >= 2) {
        alertLevel = 2; // Red alert
    } else if (criticalSystems >= 1) {
        alertLevel = 1; // Yellow alert
    } else {
        alertLevel = 0; // Green - all systems normal
    }
}

void generateRandomEvents() {
    static float lastEventTime = 0.0f;
    
    if (gameTime - lastEventTime > 3.0f + (rng() % 5)) {
        lastEventTime = gameTime;
        
        std::uniform_int_distribution<int> eventType(0, 6);
        
        switch (eventType(rng)) {
            case 0: // Asteroid field
                systems.shields -= 10.0f + (rng() % 15);
                addEvent("⚠ Asteroid field detected! Shields taking damage!", Colors::Orange);
                break;
                
            case 1: // Solar flare
                systems.power -= 8.0f + (rng() % 12);
                systems.temperature += 10.0f + (rng() % 15);
                addEvent("☀ Solar flare! Power systems affected!", Colors::Yellow);
                break;
                
            case 2: // Life support malfunction
                systems.oxygen -= 5.0f + (rng() % 10);
                addEvent("🔧 Life support malfunction detected!", Colors::Red);
                break;
                
            case 3: // Crew achievement
                systems.crew_morale += 5 + (rng() % 10);
                score += 50;
                addEvent("👥 Crew morale boost! Great teamwork!", Colors::Green);
                break;
                
            case 4: // Equipment upgrade found
                score += 100;
                addEvent("🔧 Equipment upgrade discovered! +100 points!", Colors::Cyan);
                break;
                
            case 5: // Space pirates
                systems.shields -= 15.0f + (rng() % 20);
                systems.power -= 5.0f + (rng() % 10);
                addEvent("💀 Space pirates attacking! Defensive systems active!", Colors::Red);
                break;
                
            case 6: // Repair drones activated
                systems.power += 3.0f + (rng() % 7);
                systems.oxygen += 2.0f + (rng() % 5);
                addEvent("🤖 Repair drones activated! Systems improving!", Colors::LightBlue);
                break;
        }
    }
}

void updateGameLogic() {
    if (gameState != GameState::Playing) return;
    
    gameTime += 0.016f; // 60 FPS
    
    // Generate random events
    generateRandomEvents();
    
    // Gradual system degradation
    systems.power -= 0.02f;
    systems.oxygen -= 0.015f;
    systems.shields -= 0.01f;
    
    // Temperature affects other systems
    if (systems.temperature > 70.0f) {
        systems.power -= 0.01f;
    } else if (systems.temperature < 30.0f) {
        systems.oxygen -= 0.01f;
    }
    
    // Clamp all values
    systems.power = std::max(0.0f, std::min(100.0f, systems.power));
    systems.oxygen = std::max(0.0f, std::min(100.0f, systems.oxygen));
    systems.shields = std::max(0.0f, std::min(100.0f, systems.shields));
    systems.temperature = std::max(0.0f, std::min(100.0f, systems.temperature));
    systems.crew_morale = std::max(0, std::min(100, systems.crew_morale));
    
    // Update widgets
    if (powerIndicator) powerIndicator->setValue(systems.power);
    if (oxygenIndicator) oxygenIndicator->setValue(systems.oxygen);
    if (shieldsProgress) shieldsProgress->setValue(systems.shields / 100.0f);
    
    // Update alert level
    updateAlertLevel();
    
    // Score calculation
    if (alertLevel == 0) score += 1; // Bonus for keeping all systems healthy
    
    // Check game over conditions
    if (systems.power <= 0.0f || systems.oxygen <= 0.0f) {
        gameState = GameState::GameOver;
        addEvent("💀 CRITICAL FAILURE! GAME OVER!", Colors::Red, 10.0f);
    }
    
    // Check victory condition (survive for 60 seconds with good score)
    if (gameTime > 60.0f && score > 1000) {
        gameState = GameState::Victory;
        addEvent("🎉 MISSION ACCOMPLISHED! You're a space hero!", Colors::Green, 10.0f);
    }
    
    // Update event timers
    for (auto& event : activeEvents) {
        event.timeRemaining -= 0.016f;
    }
    activeEvents.erase(
        std::remove_if(activeEvents.begin(), activeEvents.end(),
            [](const GameEvent& e) { return e.timeRemaining <= 0.0f; }),
        activeEvents.end()
    );
}

void setupGame() {
    WidgetManager::getInstance().clear();
    
    // Power control slider
    powerSlider = Slider(
        SliderConfig(50, 100, 200, 25)
            .range(0.0f, 100.0f)
            .initialValue(50.0f)
            .style(SliderStyle()
                .trackColor(Colors::DarkGray)
                .fillColor(Colors::Blue)
                .thumbColor(Colors::White)
                .showValue(true)
                .textColor(Colors::White)),
        true
    );
    
    // Oxygen control slider
    oxygenSlider = Slider(
        SliderConfig(50, 150, 200, 25)
            .range(0.0f, 100.0f)
            .initialValue(50.0f)
            .style(SliderStyle()
                .trackColor(Colors::DarkGray)
                .fillColor(Colors::Cyan)
                .thumbColor(Colors::White)
                .showValue(true)
                .textColor(Colors::White)),
        true
    );
    
    // Temperature control slider
    temperatureSlider = Slider(
        SliderConfig(50, 200, 200, 25)
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
    
    // Shields progress bar
    shieldsProgress = ProgressBar(
        ProgressBarConfig(350, 100, 250, 20)
            .range(0.0f, 1.0f)
            .value(1.0f)
            .style(ProgressBarStyle()
                .backgroundColor(Colors::DarkGray)
                .fillColor(Colors::Green)
                .borderColor(Colors::White)
                .borderWidth(1)
                .showPercentage(true)
                .textColor(Colors::White)
                .fontSize(1)
                .useBitmapFont()),
        true
    );
    
    // Power indicator (circular)
    powerIndicator = CircularIndicator(
        CircularIndicatorConfig(450, 150, 60)
            .range(0.0f, 100.0f)
            .value(100.0f)
            .style(CircularIndicatorStyle()
                .backgroundColor(Colors::DarkGray)
                .fillColor(Colors::Blue)
                .thickness(6)
                .showPercentage(true)
                .textColor(Colors::White)
                .fontSize(1)),
        true
    );
    
    // Oxygen indicator (circular)
    oxygenIndicator = CircularIndicator(
        CircularIndicatorConfig(530, 150, 60)
            .range(0.0f, 100.0f)
            .value(100.0f)
            .style(CircularIndicatorStyle()
                .backgroundColor(Colors::DarkGray)
                .fillColor(Colors::Cyan)
                .thickness(6)
                .showPercentage(true)
                .textColor(Colors::White)
                .fontSize(1)),
        true
    );
    
    // Emergency mode radio buttons
    emergencyModeGroup = std::make_shared<RadioButtonGroup>();
    
    auto normalMode = RadioButton(
        RadioButtonConfig(350, 250, "Normal", "mode")
            .selected(true)
            .style(RadioButtonStyle()
                .backgroundColor(Colors::DarkGray)
                .selectedColor(Colors::Green)
                .borderColor(Colors::Gray)
                .textColor(Colors::White)
                .fontSize(1)
                .radius(6)),
        true
    );
    normalMode->setGroup(emergencyModeGroup);
    
    auto emergencyMode = RadioButton(
        RadioButtonConfig(430, 250, "Emergency", "mode")
            .style(RadioButtonStyle()
                .backgroundColor(Colors::DarkGray)
                .selectedColor(Colors::Red)
                .borderColor(Colors::Gray)
                .textColor(Colors::White)
                .fontSize(1)
                .radius(6)),
        true
    );
    emergencyMode->setGroup(emergencyModeGroup);
    
    // Language dropdown
    std::vector<DropdownItem> languages = {
        DropdownItem("English"),
        DropdownItem("Español"),
        DropdownItem("Français"),
        DropdownItem("Deutsch"),
        DropdownItem("日本語")
    };
    
    languageDropdown = Dropdown(
        DropdownConfig(450, 300, 120, 25)
            .placeholder("Language")
            .items(languages)
            .selectedIndex(0)
            .style(DropdownStyle()
                .backgroundColor(Colors::DarkGray)
                .borderColor(Colors::Gray)
                .selectedColor(Colors::LightBlue)
                .textColor(Colors::White)
                .fontSize(1)
                .useBitmapFont()),
        true
    );
    
    // Setup event handlers
    powerSlider->onValueChanged.connect([](float value) {
        // Boost power but drain temperature control
        systems.power += (value - 50.0f) * 0.05f;
        systems.temperature += (value - 50.0f) * 0.02f;
    });
    
    oxygenSlider->onValueChanged.connect([](float value) {
        // Boost oxygen but drain power
        systems.oxygen += (value - 50.0f) * 0.04f;
        systems.power -= abs(value - 50.0f) * 0.01f;
    });
    
    temperatureSlider->onValueChanged.connect([](float value) {
        // Control temperature
        float target = value;
        if (systems.temperature < target) {
            systems.temperature += 0.3f;
            systems.power -= 0.1f; // Heating costs power
        } else if (systems.temperature > target) {
            systems.temperature -= 0.3f;
            systems.power -= 0.05f; // Cooling costs less power
        }
    });
    
    emergencyModeGroup->onSelectionChanged.connect([](std::shared_ptr<RadioButtonWidget> selected) {
        if (selected && selected->getText() == "Emergency") {
            // Emergency mode: boost all systems but drain faster
            systems.power += 20.0f;
            systems.oxygen += 15.0f;
            systems.shields += 25.0f;
            addEvent("🚨 EMERGENCY MODE ACTIVATED!", Colors::Red);
        } else {
            addEvent("✅ Normal operations resumed", Colors::Green);
        }
    });
    
    languageDropdown->onSelectionChanged.connect([](int index) {
        auto item = languageDropdown->getSelectedItem();
        if (item) {
            systems.selected_language = item->text;
            systems.crew_morale += 5; // Crew likes language diversity
            addEvent("🌍 Language changed to " + item->text, Colors::Cyan);
        }
    });
}

std::string formatTime(float seconds) {
    int mins = (int)seconds / 60;
    int secs = (int)seconds % 60;
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << mins << ":" << std::setw(2) << secs;
    return oss.str();
}

void draw() {
    // Background color based on alert level
    uint32_t bgColor = Colors::Black;
    if (alertLevel == 1) bgColor = 0xFF1A1A00; // Dark yellow tint
    else if (alertLevel == 2) bgColor = 0xFF1A0000; // Dark red tint
    
    Draw::fill(bgColor);
    
    // Title and status
    uint32_t titleColor = (alertLevel == 2) ? Colors::Red : 
                         (alertLevel == 1) ? Colors::Yellow : Colors::White;
    
    Text(Point(50, 20), "SPACE STATION CONTROL CENTER", 2.5, titleColor, true);
    
    if (gameState == GameState::Playing) {
        // Game stats
        Text(Point(500, 20), "Time: " + formatTime(gameTime), 1.5, Colors::White, true);
        Text(Point(500, 40), "Score: " + std::to_string(score), 1.5, Colors::Cyan, true);
        Text(Point(650, 20), "Alert: " + std::string(alertLevel == 0 ? "GREEN" : 
                                                     alertLevel == 1 ? "YELLOW" : "RED"), 
             1.5, alertLevel == 0 ? Colors::Green : alertLevel == 1 ? Colors::Yellow : Colors::Red, true);
        
        // Control sections
        Text(Point(50, 70), "POWER SYSTEMS:", 1.5, Colors::Blue, true);
        Text(Point(50, 105), "Power Output:", 1.2, Colors::LightGray, true);
        Text(Point(50, 155), "Oxygen Flow:", 1.2, Colors::LightGray, true);
        Text(Point(50, 205), "Temperature:", 1.2, Colors::LightGray, true);
        
        Text(Point(350, 70), "SYSTEM STATUS:", 1.5, Colors::Green, true);
        Text(Point(350, 105), "Shield Integrity:", 1.2, Colors::LightGray, true);
        Text(Point(430, 130), "PWR", 1.0, Colors::Blue, true);
        Text(Point(510, 130), "O2", 1.0, Colors::Cyan, true);
        
        Text(Point(350, 230), "OPERATIONS:", 1.5, Colors::Purple, true);
        Text(Point(350, 280), "Language:", 1.2, Colors::LightGray, true);
        
        // System health display
        Text(Point(620, 100), "HEALTH:", 1.3, Colors::White, true);
        Text(Point(620, 120), "Power: " + std::to_string((int)systems.power) + "%", 1.0, 
             systems.power > 50 ? Colors::Green : systems.power > 25 ? Colors::Yellow : Colors::Red, true);
        Text(Point(620, 135), "Oxygen: " + std::to_string((int)systems.oxygen) + "%", 1.0,
             systems.oxygen > 50 ? Colors::Green : systems.oxygen > 25 ? Colors::Yellow : Colors::Red, true);
        Text(Point(620, 150), "Shields: " + std::to_string((int)systems.shields) + "%", 1.0,
             systems.shields > 50 ? Colors::Green : systems.shields > 25 ? Colors::Yellow : Colors::Red, true);
        Text(Point(620, 165), "Temp: " + std::to_string((int)systems.temperature) + "°C", 1.0,
             (systems.temperature > 40 && systems.temperature < 60) ? Colors::Green : Colors::Yellow, true);
        Text(Point(620, 180), "Morale: " + std::to_string(systems.crew_morale) + "%", 1.0,
             systems.crew_morale > 70 ? Colors::Green : systems.crew_morale > 40 ? Colors::Yellow : Colors::Red, true);
        
        // Events display
        Text(Point(50, 260), "MISSION LOG:", 1.3, Colors::Yellow, true);
        int eventY = 280;
        for (const auto& event : activeEvents) {
            Text(Point(50, eventY), event.message, 1.0, event.color, true);
            eventY += 15;
        }
        
        // Instructions
        Text(Point(50, 380), "Instructions: Use sliders to control systems. Keep all levels healthy!", 1.1, Colors::LightGray, true);
        Text(Point(50, 395), "Emergency mode gives temporary boost but drains faster. Survive 60s with 1000+ score to win!", 1.1, Colors::LightGray, true);
    }
    else if (gameState == GameState::GameOver) {
        Text(Point(300, 200), "MISSION FAILED!", 3.0, Colors::Red, true);
        Text(Point(280, 240), "Final Score: " + std::to_string(score), 2.0, Colors::White, true);
        Text(Point(250, 270), "Press R to restart", 1.5, Colors::Cyan, true);
    }
    else if (gameState == GameState::Victory) {
        Text(Point(280, 200), "MISSION SUCCESS!", 3.0, Colors::Green, true);
        Text(Point(280, 240), "Final Score: " + std::to_string(score), 2.0, Colors::White, true);
        Text(Point(250, 270), "You're a space hero! Press R to play again", 1.5, Colors::Cyan, true);
    }
    
    updateGameLogic();
}

void handleInput() {
    const auto& input = Input::getState();
    
    // Handle keyboard input for game restart
    if (input.keyPressed && input.lastKeyPressed == KeyCode::R) {
        if (gameState != GameState::Playing) {
            // Reset game
            gameState = GameState::Playing;
            systems = SystemHealth();
            gameTime = 0.0f;
            score = 0;
            alertLevel = 0;
            activeEvents.clear();
            addEvent("New mission started! Good luck, Commander!", Colors::Green);
        }
    }
    
    // Emergency keys
    if (input.keyPressed) {
        switch (input.lastKeyPressed) {
            case KeyCode::P: // Emergency power boost
                systems.power += 15.0f;
                score -= 50; // Penalty for emergency use
                addEvent("⚡ Emergency power boost activated! -50 score", Colors::Yellow);
                break;
                
            case KeyCode::O: // Emergency oxygen boost
                systems.oxygen += 15.0f;
                score -= 50;
                addEvent("💨 Emergency oxygen boost activated! -50 score", Colors::Yellow);
                break;
                
            case KeyCode::S: // Emergency shield boost
                systems.shields += 20.0f;
                score -= 75;
                addEvent("🛡️ Emergency shields activated! -75 score", Colors::Yellow);
                break;
        }
    }
}

int main() {
    Fern::initialize();
    setupGame();
    
    // Add initial welcome message
    addEvent("🚀 Welcome, Commander! Manage the space station systems!", Colors::Green, 5.0f);
    addEvent("Use sliders to control power, oxygen, and temperature", Colors::Cyan, 4.0f);
    addEvent("Emergency keys: P (power), O (oxygen), S (shields)", Colors::Yellow, 4.0f);
    
    Fern::setDrawCallback([]() {
        handleInput();
        draw();
    });
    
    Fern::startRenderLoop();
    return 0;
}
