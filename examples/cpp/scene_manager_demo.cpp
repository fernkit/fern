#include <fern/fern.hpp>
#include <iostream>
#include <chrono>

using namespace Fern;

// Forward declarations
class MenuScene;
class GameScene;
class SettingsScene;
class PauseScene;

// Menu Scene - The main menu of the application
class MenuScene : public Scene {
public:
    MenuScene() : Scene("MenuScene") {}
    
    void onCreate() override {
        std::cout << "Menu scene created" << std::endl;
    }
    
    void onEnter() override {
        std::cout << "Entered menu scene" << std::endl;
        setupUI();
    }
    
    void onExit() override {
        std::cout << "Exiting menu scene" << std::endl;
        WidgetManager::getInstance().clear();
    }
    
    void update(float deltaTime) override {
        // Update logic for menu (animations, etc.)
    }
    
    void render() override {
        Draw::fill(Colors::DarkBlue);
        Scene::render(); // Renders all widgets
    }
    
private:
    void setupUI() {
        // Title
        auto title = Text(Point(0, 0), "Fern Scene Manager Demo", 4, Colors::White);
        
        // Menu buttons
        ButtonStyle buttonStyle;
        buttonStyle.normalColor(Colors::Blue)
                  .hoverColor(Colors::LightBlue)
                  .pressColor(Colors::DarkBlue)
                  .textColor(Colors::White)
                  .textScale(2);
        
        auto startButton = Button(ButtonConfig(0, 0, 200, 50, "Start Game").style(buttonStyle));
        startButton->onClick.connect([]() {
            SceneManager::getInstance().pushScene("GameScene");
        });
        
        auto settingsButton = Button(ButtonConfig(0, 0, 200, 50, "Settings").style(buttonStyle));
        settingsButton->onClick.connect([]() {
            SceneManager::getInstance().pushScene("SettingsScene");
        });
        
        auto exitButton = Button(ButtonConfig(0, 0, 200, 50, "Exit").style(buttonStyle));
        exitButton->onClick.connect([]() {
            std::cout << "Exiting application..." << std::endl;
            exit(0);
        });
        
        // Layout
        std::vector<std::shared_ptr<Widget>> children = {
            title,
            SizedBox(0, 50),
            startButton,
            SizedBox(0, 20),
            settingsButton,
            SizedBox(0, 20),
            exitButton
        };
        
        int width = Fern::getWidth();
        int height = Fern::getHeight();
        auto centerWidget = std::make_shared<CenterWidget>(0, 0, width, height);
        centerWidget->add(Column(children));
        addWidget(centerWidget);
    }
};

// Game Scene - The main game area
class GameScene : public Scene {
private:
    std::shared_ptr<TextWidget> scoreText;
    std::shared_ptr<TextWidget> timeText;
    int score = 0;
    float gameTime = 0.0f;
    bool isPaused = false;
    
public:
    GameScene() : Scene("GameScene") {}
    
    void onCreate() override {
        std::cout << "Game scene created" << std::endl;
        score = 0;
        gameTime = 0.0f;
        isPaused = false;
    }
    
    void onEnter() override {
        std::cout << "Entered game scene" << std::endl;
        setupUI();
    }
    
    void onExit() override {
        std::cout << "Exiting game scene" << std::endl;
        WidgetManager::getInstance().clear();
    }
    
    void update(float deltaTime) override {
        if (!isPaused) {
            gameTime += deltaTime;
            
            // Simple score increment for demo
            if (static_cast<int>(gameTime) % 2 == 0 && static_cast<int>(gameTime * 10) % 10 == 0) {
                score += 10;
                updateUI();
            }
        }
    }
    
    void render() override {
        Draw::fill(Colors::DarkGreen);
        Scene::render();
    }
    
private:
    void setupUI() {
        // Game UI
        scoreText = Text(Point(0, 0), "Score: 0", 3, Colors::Yellow);
        timeText = Text(Point(0, 0), "Time: 0.0s", 2, Colors::White);
        
        // Game controls
        ButtonStyle buttonStyle;
        buttonStyle.normalColor(Colors::Red)
                  .hoverColor(Colors::LightGray)
                  .textColor(Colors::White)
                  .textScale(2);
        
        auto pauseButton = Button(ButtonConfig(0, 0, 150, 40, "Pause").style(buttonStyle));
        pauseButton->onClick.connect([this]() {
            isPaused = true;
            SceneManager::getInstance().pushScene("PauseScene");
        });
        
        auto backButton = Button(ButtonConfig(0, 0, 150, 40, "Back to Menu").style(buttonStyle));
        backButton->onClick.connect([]() {
            SceneManager::getInstance().replaceScene("MenuScene");
        });
        
        // Game area (placeholder)
        auto gameArea = Text(Point(0, 0), "🎮 Game Area - Score increases automatically!", 2, Colors::White);
        
        // Layout
        std::vector<std::shared_ptr<Widget>> topBar = {
            scoreText,
            SizedBox(0, 10),
            timeText
        };
        
        std::vector<std::shared_ptr<Widget>> bottomBar = {
            pauseButton,
            SizedBox(0, 10),
            backButton
        };
        
        std::vector<std::shared_ptr<Widget>> mainLayout = {
            Column(topBar),
            SizedBox(0, 40),
            gameArea,
            SizedBox(0, 60),
            Column(bottomBar)
        };
        
        int width = Fern::getWidth();
        int height = Fern::getHeight();
        auto centerWidget = std::make_shared<CenterWidget>(0, 0, width, height);
        centerWidget->add(Column(mainLayout));
        addWidget(centerWidget);
    }
    
    void updateUI() {
        if (scoreText) {
            scoreText->setText("Score: " + std::to_string(score));
        }
        if (timeText) {
            timeText->setText("Time: " + std::to_string(static_cast<int>(gameTime)) + "s");
        }
    }
};

// Settings Scene - Configuration options
class SettingsScene : public Scene {
public:
    SettingsScene() : Scene("SettingsScene") {}
    
    void onCreate() override {
        std::cout << "Settings scene created" << std::endl;
    }
    
    void onEnter() override {
        std::cout << "Entered settings scene" << std::endl;
        setupUI();
    }
    
    void onExit() override {
        std::cout << "Exiting settings scene" << std::endl;
        WidgetManager::getInstance().clear();
    }
    
    void render() override {
        Draw::fill(Colors::DarkGray);
        Scene::render();
    }
    
private:
    void setupUI() {
        auto title = Text(Point(0, 0), "Settings", 4, Colors::White);
        
        // Settings options (placeholder)
        auto volumeText = Text(Point(0, 0), "🔊 Volume: 75%", 2, Colors::White);
        auto graphicsText = Text(Point(0, 0), "🎨 Graphics: High", 2, Colors::White);
        auto controlsText = Text(Point(0, 0), "🎮 Controls: WASD", 2, Colors::White);
        
        ButtonStyle buttonStyle;
        buttonStyle.normalColor(Colors::Gray)
                  .hoverColor(Colors::LightGray)
                  .textColor(Colors::White)
                  .textScale(2);
        
        auto backButton = Button(ButtonConfig(0, 0, 150, 40, "Back").style(buttonStyle));
        backButton->onClick.connect([]() {
            SceneManager::getInstance().popScene(); // Go back to previous scene
        });
        
        // Layout
        std::vector<std::shared_ptr<Widget>> children = {
            title,
            SizedBox(0, 40),
            volumeText,
            SizedBox(0, 20),
            graphicsText,
            SizedBox(0, 20),
            controlsText,
            SizedBox(0, 60),
            backButton
        };
        
        int width = Fern::getWidth();
        int height = Fern::getHeight();
        auto centerWidget = std::make_shared<CenterWidget>(0, 0, width, height);
        centerWidget->add(Column(children));
        addWidget(centerWidget);
    }
};

// Pause Scene - Overlay when game is paused
class PauseScene : public Scene {
public:
    PauseScene() : Scene("PauseScene") {}
    
    void onCreate() override {
        std::cout << "Pause scene created" << std::endl;
    }
    
    void onEnter() override {
        std::cout << "Game paused" << std::endl;
        setupUI();
    }
    
    void onExit() override {
        std::cout << "Game resumed" << std::endl;
        WidgetManager::getInstance().clear();
    }
    
    void render() override {
        // Render with semi-transparent overlay effect
        Draw::fill(0x80000000); // Semi-transparent black
        Scene::render();
    }
    
private:
    void setupUI() {
        auto title = Text(Point(0, 0), "PAUSED", 4, Colors::Yellow);
        
        ButtonStyle buttonStyle;
        buttonStyle.normalColor(Colors::Blue)
                  .hoverColor(Colors::LightBlue)
                  .textColor(Colors::White)
                  .textScale(2);
        
        auto resumeButton = Button(ButtonConfig(0, 0, 150, 40, "Resume").style(buttonStyle));
        resumeButton->onClick.connect([]() {
            SceneManager::getInstance().popScene(); // Go back to game
        });
        
        auto menuButton = Button(ButtonConfig(0, 0, 150, 40, "Main Menu").style(buttonStyle));
        menuButton->onClick.connect([]() {
            // Clear pause and game scenes, go to menu
            SceneManager::getInstance().clearScenes();
            SceneManager::getInstance().pushScene("MenuScene");
        });
        
        // Layout
        std::vector<std::shared_ptr<Widget>> children = {
            title,
            SizedBox(0, 40),
            resumeButton,
            SizedBox(0, 20),
            menuButton
        };
        
        int width = Fern::getWidth();
        int height = Fern::getHeight();
        auto centerWidget = std::make_shared<CenterWidget>(0, 0, width, height);
        centerWidget->add(Column(children));
        addWidget(centerWidget);
    }
};

// Global variables for time tracking
static auto lastTime = std::chrono::high_resolution_clock::now();

void setupScenes() {
    // Register all scenes
    REGISTER_SCENE("MenuScene", MenuScene);
    REGISTER_SCENE("GameScene", GameScene);
    REGISTER_SCENE("SettingsScene", SettingsScene);
    REGISTER_SCENE("PauseScene", PauseScene);
    
    // Start with the menu scene
    SceneManager::getInstance().pushScene("MenuScene");
}

void update() {
    // Calculate delta time
    auto currentTime = std::chrono::high_resolution_clock::now();
    float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
    lastTime = currentTime;
    
    // Update current scene
    SceneManager::getInstance().update(deltaTime);
}

void draw() {
    // Render current scene
    SceneManager::getInstance().render();
}

int main() {
    std::cout << "🌿 Starting Scene Manager Demo..." << std::endl;
    
    Fern::initialize();
    setupScenes();
    
    Fern::setDrawCallback(draw);
    
    // Set up update loop (if available)
    // Note: This assumes there's an update callback - adapt as needed for your Fern version
    
    Fern::startRenderLoop();
    
    return 0;
}
