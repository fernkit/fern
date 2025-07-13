#include <fern/fern.hpp>
#include <memory>
#include <vector>
#include <random>
#include <chrono>

using namespace Fern;

// Game state
enum class GamePhase {
    Menu,
    Instructions,
    Playing,
    GameOver
};

enum class TaskType {
    SetSlider,
    SelectRadio,
    SelectDropdown,
    WaitForProgress
};

struct Task {
    TaskType type;
    std::string instruction;
    float targetValue;
    std::string targetOption;
    bool completed;
    float timeLimit;
    float timeRemaining;
    uint32_t color;
};

// Game variables
GamePhase currentPhase = GamePhase::Menu;
std::vector<Task> tasks;
int currentTaskIndex = 0;
int score = 0;
int level = 1;
float gameTime = 0.0f;
bool gameRunning = false;
std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

// Widgets
std::shared_ptr<SliderWidget> reactionSlider;
std::shared_ptr<ProgressBarWidget> timeProgress;
std::shared_ptr<RadioButtonGroup> colorGroup;
std::shared_ptr<DropdownWidget> shapeDropdown;
std::shared_ptr<CircularIndicatorWidget> scoreIndicator;

// Game functions
void generateRandomTask() {
    std::uniform_int_distribution<int> taskTypeDist(0, 3);
    TaskType type = static_cast<TaskType>(taskTypeDist(rng));
    
    Task newTask;
    newTask.type = type;
    newTask.completed = false;
    newTask.timeLimit = 5.0f - (level * 0.2f); // Gets faster each level
    newTask.timeLimit = std::max(1.5f, newTask.timeLimit);
    newTask.timeRemaining = newTask.timeLimit;
    
    switch (type) {
        case TaskType::SetSlider: {
            std::uniform_int_distribution<int> valueDist(10, 90);
            newTask.targetValue = static_cast<float>(valueDist(rng));
            newTask.instruction = "Set slider to " + std::to_string(static_cast<int>(newTask.targetValue)) + "%";
            newTask.color = Colors::Blue;
            break;
        }
        
        case TaskType::SelectRadio: {
            std::vector<std::string> colors = {"Red", "Green", "Blue", "Yellow"};
            std::uniform_int_distribution<int> colorDist(0, colors.size() - 1);
            newTask.targetOption = colors[colorDist(rng)];
            newTask.instruction = "Select " + newTask.targetOption + " button";
            newTask.color = Colors::Purple;
            break;
        }
        
        case TaskType::SelectDropdown: {
            std::vector<std::string> shapes = {"Circle", "Square", "Triangle", "Diamond"};
            std::uniform_int_distribution<int> shapeDist(0, shapes.size() - 1);
            newTask.targetOption = shapes[shapeDist(rng)];
            newTask.instruction = "Choose " + newTask.targetOption + " from dropdown";
            newTask.color = Colors::Orange;
            break;
        }
        
        case TaskType::WaitForProgress: {
            newTask.instruction = "Wait for progress bar to reach 100%";
            newTask.color = Colors::Green;
            newTask.timeLimit = 3.0f; // Fixed time for progress bar
            newTask.timeRemaining = newTask.timeLimit;
            break;
        }
    }
    
    tasks.push_back(newTask);
}

void setupWidgets() {
    WidgetManager::getInstance().clear();
    
    // Reaction slider
    reactionSlider = Slider(
        SliderConfig(100, 120, 250, 30)
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
    
    // Time progress bar
    timeProgress = ProgressBar(
        ProgressBarConfig(100, 200, 300, 25)
            .range(0.0f, 1.0f)
            .value(1.0f)
            .style(ProgressBarStyle()
                .backgroundColor(Colors::DarkGray)
                .fillColor(Colors::Green)
                .borderColor(Colors::White)
                .borderWidth(2)
                .showPercentage(false)),
        true
    );
    
    // Color selection radio buttons
    colorGroup = std::make_shared<RadioButtonGroup>();
    
    auto redButton = RadioButton(
        RadioButtonConfig(100, 280, "Red", "color")
            .style(RadioButtonStyle()
                .backgroundColor(Colors::DarkGray)
                .selectedColor(Colors::Red)
                .borderColor(Colors::Gray)
                .textColor(Colors::White)
                .fontSize(1)
                .radius(6)),
        true
    );
    redButton->setGroup(colorGroup);
    
    auto greenButton = RadioButton(
        RadioButtonConfig(150, 280, "Green", "color")
            .style(RadioButtonStyle()
                .backgroundColor(Colors::DarkGray)
                .selectedColor(Colors::Green)
                .borderColor(Colors::Gray)
                .textColor(Colors::White)
                .fontSize(1)
                .radius(6)),
        true
    );
    greenButton->setGroup(colorGroup);
    
    auto blueButton = RadioButton(
        RadioButtonConfig(210, 280, "Blue", "color")
            .style(RadioButtonStyle()
                .backgroundColor(Colors::DarkGray)
                .selectedColor(Colors::Blue)
                .borderColor(Colors::Gray)
                .textColor(Colors::White)
                .fontSize(1)
                .radius(6)),
        true
    );
    blueButton->setGroup(colorGroup);
    
    auto yellowButton = RadioButton(
        RadioButtonConfig(260, 280, "Yellow", "color")
            .style(RadioButtonStyle()
                .backgroundColor(Colors::DarkGray)
                .selectedColor(Colors::Yellow)
                .borderColor(Colors::Gray)
                .textColor(Colors::White)
                .fontSize(1)
                .radius(6)),
        true
    );
    yellowButton->setGroup(colorGroup);
    
    // Shape dropdown
    std::vector<DropdownItem> shapes = {
        DropdownItem("Circle"),
        DropdownItem("Square"),
        DropdownItem("Triangle"),
        DropdownItem("Diamond")
    };
    
    shapeDropdown = Dropdown(
        DropdownConfig(100, 350, 150, 30)
            .placeholder("Select Shape")
            .items(shapes)
            .selectedIndex(-1)
            .style(DropdownStyle()
                .backgroundColor(Colors::DarkGray)
                .borderColor(Colors::Gray)
                .selectedColor(Colors::Orange)
                .textColor(Colors::White)
                .fontSize(1)
                .useBitmapFont()),
        true
    );
    
    // Score indicator
    scoreIndicator = CircularIndicator(
        CircularIndicatorConfig(500, 150, 80)
            .range(0.0f, 1000.0f)
            .value(0.0f)
            .style(CircularIndicatorStyle()
                .backgroundColor(Colors::DarkGray)
                .fillColor(Colors::Gold)
                .thickness(8)
                .showPercentage(false)
                .textColor(Colors::White)
                .fontSize(1)),
        true
    );
    
    // Event handlers
    reactionSlider->onValueChanged.connect([](float value) {
        if (gameRunning && currentTaskIndex < tasks.size()) {
            Task& task = tasks[currentTaskIndex];
            if (task.type == TaskType::SetSlider && !task.completed) {
                float tolerance = 5.0f;
                if (abs(value - task.targetValue) <= tolerance) {
                    task.completed = true;
                    score += static_cast<int>(100 * (task.timeRemaining / task.timeLimit));
                }
            }
        }
    });
    
    colorGroup->onSelectionChanged.connect([](std::shared_ptr<RadioButtonWidget> selected) {
        if (gameRunning && selected && currentTaskIndex < tasks.size()) {
            Task& task = tasks[currentTaskIndex];
            if (task.type == TaskType::SelectRadio && !task.completed) {
                if (selected->getText() == task.targetOption) {
                    task.completed = true;
                    score += static_cast<int>(100 * (task.timeRemaining / task.timeLimit));
                }
            }
        }
    });
    
    shapeDropdown->onSelectionChanged.connect([](int index) {
        if (gameRunning && currentTaskIndex < tasks.size()) {
            Task& task = tasks[currentTaskIndex];
            if (task.type == TaskType::SelectDropdown && !task.completed) {
                auto item = shapeDropdown->getSelectedItem();
                if (item && item->text == task.targetOption) {
                    task.completed = true;
                    score += static_cast<int>(100 * (task.timeRemaining / task.timeLimit));
                }
            }
        }
    });
}

void startNewGame() {
    currentPhase = GamePhase::Playing;
    tasks.clear();
    currentTaskIndex = 0;
    score = 0;
    level = 1;
    gameTime = 0.0f;
    gameRunning = true;
    
    // Generate initial tasks
    for (int i = 0; i < 5; i++) {
        generateRandomTask();
    }
}

void updateGame() {
    if (currentPhase != GamePhase::Playing || !gameRunning) return;
    
    gameTime += 0.016f; // 60 FPS
    
    // Update current task
    if (currentTaskIndex < tasks.size()) {
        Task& currentTask = tasks[currentTaskIndex];
        currentTask.timeRemaining -= 0.016f;
        
        // Update progress bar for wait task
        if (currentTask.type == TaskType::WaitForProgress) {
            float progress = 1.0f - (currentTask.timeRemaining / currentTask.timeLimit);
            timeProgress->setValue(progress);
            
            if (progress >= 1.0f && !currentTask.completed) {
                currentTask.completed = true;
                score += static_cast<int>(100 * (currentTask.timeRemaining / currentTask.timeLimit));
            }
        } else {
            // Update time bar to show remaining time
            float timeRatio = currentTask.timeRemaining / currentTask.timeLimit;
            timeProgress->setValue(timeRatio);
        }
        
        // Check if task is completed or time ran out
        if (currentTask.completed || currentTask.timeRemaining <= 0.0f) {
            currentTaskIndex++;
            
            // Reset widgets for next task
            if (currentTaskIndex < tasks.size()) {
                reactionSlider->setValue(50.0f);
                shapeDropdown->setSelectedIndex(-1);
                // Reset radio buttons by selecting none (if possible)
            }
            
            // Level up every 5 tasks
            if (currentTaskIndex % 5 == 0) {
                level++;
                // Generate more tasks
                for (int i = 0; i < 5; i++) {
                    generateRandomTask();
                }
            }
        }
        
        // Update score indicator
        scoreIndicator->setValue(static_cast<float>(score));
    }
    
    // Game over if we run out of tasks (shouldn't happen) or player makes too many mistakes
    if (currentTaskIndex >= tasks.size()) {
        currentPhase = GamePhase::GameOver;
        gameRunning = false;
    }
}

void handleInput() {
    const auto& input = Input::getState();
    
    if (input.keyPressed) {
        switch (input.lastKeyPressed) {
            case KeyCode::Space:
                if (currentPhase == GamePhase::Menu) {
                    currentPhase = GamePhase::Instructions;
                } else if (currentPhase == GamePhase::Instructions) {
                    startNewGame();
                } else if (currentPhase == GamePhase::GameOver) {
                    currentPhase = GamePhase::Menu;
                }
                break;
                
            case KeyCode::Escape:
                if (currentPhase == GamePhase::Playing) {
                    currentPhase = GamePhase::GameOver;
                    gameRunning = false;
                }
                break;
        }
    }
}

void draw() {
    Draw::fill(0xFF1A1A2E);
    
    switch (currentPhase) {
        case GamePhase::Menu:
            Text(Point(200, 150), "WIDGET REACTION GAME", 3.0, Colors::Cyan, true);
            Text(Point(250, 220), "Test your reflexes!", 2.0, Colors::White, true);
            Text(Point(280, 280), "Press SPACE to start", 1.5, Colors::Yellow, true);
            Text(Point(200, 350), "Use widgets to complete tasks as fast as possible!", 1.2, Colors::LightGray, true);
            break;
            
        case GamePhase::Instructions:
            Text(Point(250, 100), "HOW TO PLAY", 2.5, Colors::Cyan, true);
            Text(Point(100, 150), "• Blue tasks: Adjust the slider to the target value", 1.3, Colors::Blue, true);
            Text(Point(100, 180), "• Purple tasks: Click the correct radio button", 1.3, Colors::Purple, true);
            Text(Point(100, 210), "• Orange tasks: Select the right option from dropdown", 1.3, Colors::Orange, true);
            Text(Point(100, 240), "• Green tasks: Wait for the progress bar to fill", 1.3, Colors::Green, true);
            Text(Point(100, 290), "Complete tasks quickly for bonus points!", 1.3, Colors::Yellow, true);
            Text(Point(100, 320), "Game gets faster each level!", 1.3, Colors::Red, true);
            Text(Point(250, 380), "Press SPACE to begin", 1.5, Colors::White, true);
            break;
            
        case GamePhase::Playing:
            // Game UI
            Text(Point(50, 20), "WIDGET REACTION GAME", 2.0, Colors::Cyan, true);
            Text(Point(50, 50), "Level: " + std::to_string(level), 1.5, Colors::White, true);
            Text(Point(150, 50), "Score: " + std::to_string(score), 1.5, Colors::Yellow, true);
            Text(Point(450, 50), "Task " + std::to_string(currentTaskIndex + 1) + "/" + std::to_string(tasks.size()), 1.5, Colors::LightGray, true);
            
            // Current task
            if (currentTaskIndex < tasks.size()) {
                const Task& currentTask = tasks[currentTaskIndex];
                Text(Point(100, 90), "CURRENT TASK:", 1.5, currentTask.color, true);
                Text(Point(100, 110), currentTask.instruction, 1.8, currentTask.color, true);
                
                // Time remaining
                Text(Point(100, 170), "Time remaining:", 1.2, Colors::White, true);
                
                // Widget labels
                Text(Point(100, 250), "Radio Buttons:", 1.2, Colors::Purple, true);
                Text(Point(100, 320), "Dropdown:", 1.2, Colors::Orange, true);
                
                // Score display
                Text(Point(450, 120), "SCORE", 1.5, Colors::Gold, true);
                
                // Instructions
                Text(Point(50, 400), "ESC = Quit", 1.0, Colors::LightGray, true);
            }
            
            updateGame();
            break;
            
        case GamePhase::GameOver:
            Text(Point(280, 150), "GAME OVER!", 3.0, Colors::Red, true);
            Text(Point(250, 220), "Final Score: " + std::to_string(score), 2.0, Colors::Yellow, true);
            Text(Point(280, 270), "Level Reached: " + std::to_string(level), 1.5, Colors::White, true);
            
            std::string rating;
            uint32_t ratingColor;
            if (score >= 2000) {
                rating = "AMAZING!";
                ratingColor = Colors::Gold;
            } else if (score >= 1000) {
                rating = "Great job!";
                ratingColor = Colors::Green;
            } else if (score >= 500) {
                rating = "Good effort!";
                ratingColor = Colors::Cyan;
            } else {
                rating = "Keep practicing!";
                ratingColor = Colors::Yellow;
            }
            
            Text(Point(300, 320), rating, 1.8, ratingColor, true);
            Text(Point(280, 380), "Press SPACE to return to menu", 1.2, Colors::LightGray, true);
            break;
    }
    
    handleInput();
}

int main() {
    Fern::initialize();
    setupWidgets();
    
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    
    return 0;
}
