#include <fern/fern.hpp>
#include <iostream>
#include <memory>

using namespace Fern;

void draw() {
    Draw::fill(Colors::DarkBlue);
}

// Main Menu Scene
class MainMenuScene : public Scene {
public:
    MainMenuScene() : Scene("MainMenu") {}
    
    void onCreate() override {
        std::cout << "Creating Main Menu Scene" << std::endl;
    }
    
    void onEnter() override {
        std::cout << "Entering Main Menu Scene" << std::endl;
        
        // Clear any existing widgets
        WidgetManager::getInstance().clear();
        
        // Get screen dimensions
        int width = Fern::getWidth();
        int height = Fern::getHeight();
        
        // Create UI elements
        auto titleText = Text(Point(0,0), "Fern Scene Demo", 4, Colors::White, false);
        auto subtitleText = Text(Point(0,0), "Main Menu", 2, Colors::LightGray, false);
        
        // Create buttons
        auto gameButton = Button(ButtonConfig{0, 0, 200, 50, Colors::Green, Colors::LightGreen, Colors::DarkGreen, "Start Game", 2, Colors::White});
        auto settingsButton = Button(ButtonConfig{0, 0, 200, 50, Colors::Blue, Colors::LightBlue, Colors::DarkBlue, "Settings", 2, Colors::White});
        auto quitButton = Button(ButtonConfig{0, 0, 200, 50, Colors::Red, Colors::LightRed, Colors::DarkRed, "Quit", 2, Colors::White});
        
        // Connect button events
        gameButton->onClick.connect([](){
            std::cout << "Starting game..." << std::endl;
            pushScene("GameScene");
        });
        
        settingsButton->onClick.connect([](){
            std::cout << "Opening settings..." << std::endl;
            pushScene("SettingsScene");
        });
        
        quitButton->onClick.connect([](){
            std::cout << "Quitting application..." << std::endl;
            // TODO: Implement proper application exit
        });
        
        // Create layout
        auto layout = Column({
            titleText,
            SizedBox(0, 20, false),
            subtitleText,
            SizedBox(0, 40, false),
            gameButton,
            SizedBox(0, 20, false),
            settingsButton,
            SizedBox(0, 20, false),
            quitButton
        }, false, MainAxisAlignment::Center, CrossAxisAlignment::Center);
        
        // Add to widget manager
        addWidget(Container(Colors::Transparent, 0, 0, width, height, Center(layout, false)));
    }
    
    void onExit() override {
        std::cout << "Exiting Main Menu Scene" << std::endl;
    }
};

// Game Scene
class GameScene : public Scene {
private:
    int score = 0;
    std::shared_ptr<TextWidget> scoreText;
    
public:
    GameScene() : Scene("GameScene") {}
    
    void onCreate() override {
        std::cout << "Creating Game Scene" << std::endl;
    }
    
    void onEnter() override {
        std::cout << "Entering Game Scene" << std::endl;
        
        // Clear any existing widgets
        WidgetManager::getInstance().clear();
        
        // Get screen dimensions
        int width = Fern::getWidth();
        int height = Fern::getHeight();
        
        // Create UI elements
        auto titleText = Text(Point(0,0), "Game Scene", 3, Colors::White, false);
        scoreText = Text(Point(0,0), "Score: 0", 2, Colors::Yellow, false);
        
        // Create game buttons
        auto increaseScoreButton = Button(ButtonConfig{0, 0, 180, 50, Colors::Green, Colors::LightGreen, Colors::DarkGreen, "Increase Score", 2, Colors::White});
        auto pauseButton = Button(ButtonConfig{0, 0, 180, 50, Colors::Orange, Colors::Yellow, Colors::DarkGray, "Pause", 2, Colors::White});
        auto backButton = Button(ButtonConfig{0, 0, 180, 50, Colors::Gray, Colors::LightGray, Colors::DarkGray, "Back to Menu", 2, Colors::White});
        
        // Connect button events
        increaseScoreButton->onClick.connect([this](){
            score += 10;
            scoreText->setText("Score: " + std::to_string(score));
            std::cout << "Score increased to: " << score << std::endl;
        });
        
        pauseButton->onClick.connect([](){
            std::cout << "Opening pause menu..." << std::endl;
            pushScene("PauseScene");
        });
        
        backButton->onClick.connect([](){
            std::cout << "Returning to main menu..." << std::endl;
            popScene();
        });
        
        // Create layout
        auto layout = Column({
            titleText,
            SizedBox(0, 20, false),
            scoreText,
            SizedBox(0, 30, false),
            increaseScoreButton,
            SizedBox(0, 20, false),
            pauseButton,
            SizedBox(0, 20, false),
            backButton
        }, false, MainAxisAlignment::Center, CrossAxisAlignment::Center);
        
        // Add to widget manager
        addWidget(Container(Colors::Transparent, 0, 0, width, height, Center(layout, false)));
    }
    
    void onExit() override {
        std::cout << "Exiting Game Scene (Score: " << score << ")" << std::endl;
    }
};

// Settings Scene
class SettingsScene : public Scene {
public:
    SettingsScene() : Scene("SettingsScene") {}
    
    void onCreate() override {
        std::cout << "Creating Settings Scene" << std::endl;
    }
    
    void onEnter() override {
        std::cout << "Entering Settings Scene" << std::endl;
        
        // Clear any existing widgets
        WidgetManager::getInstance().clear();
        
        // Get screen dimensions
        int width = Fern::getWidth();
        int height = Fern::getHeight();
        
        // Create UI elements
        auto titleText = Text(Point(0,0), "Settings", 3, Colors::White, false);
        auto volumeText = Text(Point(0,0), "Volume: 50%", 2, Colors::LightGray, false);
        auto difficultyText = Text(Point(0,0), "Difficulty: Normal", 2, Colors::LightGray, false);
        
        // Create buttons
        auto volumeUpButton = Button(ButtonConfig{0, 0, 120, 40, Colors::Green, Colors::LightGreen, Colors::DarkGreen, "Volume +", 1, Colors::White});
        auto volumeDownButton = Button(ButtonConfig{0, 0, 120, 40, Colors::Red, Colors::LightRed, Colors::DarkRed, "Volume -", 1, Colors::White});
        auto difficultyButton = Button(ButtonConfig{0, 0, 180, 40, Colors::Orange, Colors::Yellow, Colors::DarkGray, "Change Difficulty", 1, Colors::White});
        auto backButton = Button(ButtonConfig{0, 0, 180, 50, Colors::Gray, Colors::LightGray, Colors::DarkGray, "Back", 2, Colors::White});
        
        // Connect button events
        volumeUpButton->onClick.connect([volumeText](){
            static int volume = 50;
            volume = std::min(100, volume + 10);
            volumeText->setText("Volume: " + std::to_string(volume) + "%");
            std::cout << "Volume increased to: " << volume << "%" << std::endl;
        });
        
        volumeDownButton->onClick.connect([volumeText](){
            static int volume = 50;
            volume = std::max(0, volume - 10);
            volumeText->setText("Volume: " + std::to_string(volume) + "%");
            std::cout << "Volume decreased to: " << volume << "%" << std::endl;
        });
        
        difficultyButton->onClick.connect([difficultyText](){
            static int difficulty = 1; // 0=Easy, 1=Normal, 2=Hard
            difficulty = (difficulty + 1) % 3;
            std::string diffText = difficulty == 0 ? "Easy" : (difficulty == 1 ? "Normal" : "Hard");
            difficultyText->setText("Difficulty: " + diffText);
            std::cout << "Difficulty changed to: " << diffText << std::endl;
        });
        
        backButton->onClick.connect([](){
            std::cout << "Returning to main menu..." << std::endl;
            popScene();
        });
        
        // Create volume controls row
        auto volumeRow = Row({
            volumeDownButton,
            SizedBox(20, 0, false),
            volumeUpButton
        }, false, MainAxisAlignment::Center, CrossAxisAlignment::Center);
        
        // Create layout
        auto layout = Column({
            titleText,
            SizedBox(0, 30, false),
            volumeText,
            SizedBox(0, 10, false),
            volumeRow,
            SizedBox(0, 30, false),
            difficultyText,
            SizedBox(0, 10, false),
            difficultyButton,
            SizedBox(0, 40, false),
            backButton
        }, false, MainAxisAlignment::Center, CrossAxisAlignment::Center);
        
        // Add to widget manager
        addWidget(Container(Colors::Transparent, 0, 0, width, height, Center(layout, false)));
    }
    
    void onExit() override {
        std::cout << "Exiting Settings Scene" << std::endl;
    }
};

// Pause Scene
class PauseScene : public Scene {
public:
    PauseScene() : Scene("PauseScene") {}
    
    void onCreate() override {
        std::cout << "Creating Pause Scene" << std::endl;
    }
    
    void onEnter() override {
        std::cout << "Entering Pause Scene" << std::endl;
        
        // Clear any existing widgets
        WidgetManager::getInstance().clear();
        
        // Get screen dimensions
        int width = Fern::getWidth();
        int height = Fern::getHeight();
        
        // Create UI elements
        auto titleText = Text(Point(0,0), "Game Paused", 3, Colors::White, false);
        auto instructionText = Text(Point(0,0), "What would you like to do?", 2, Colors::LightGray, false);
        
        // Create buttons
        auto resumeButton = Button(ButtonConfig{0, 0, 180, 50, Colors::Green, Colors::LightGreen, Colors::DarkGreen, "Resume", 2, Colors::White});
        auto mainMenuButton = Button(ButtonConfig{0, 0, 180, 50, Colors::Blue, Colors::LightBlue, Colors::DarkBlue, "Main Menu", 2, Colors::White});
        auto quitButton = Button(ButtonConfig{0, 0, 180, 50, Colors::Red, Colors::LightRed, Colors::DarkRed, "Quit Game", 2, Colors::White});
        
        // Connect button events
        resumeButton->onClick.connect([](){
            std::cout << "Resuming game..." << std::endl;
            popScene(); // Return to game scene
        });
        
        mainMenuButton->onClick.connect([](){
            std::cout << "Returning to main menu..." << std::endl;
            popScene(); // Pop pause scene
            popScene(); // Pop game scene, back to main menu
        });
        
        quitButton->onClick.connect([](){
            std::cout << "Quitting to main menu..." << std::endl;
            replaceScene("MainMenu"); // Replace all scenes with main menu
        });
        
        // Create layout
        auto layout = Column({
            titleText,
            SizedBox(0, 20, false),
            instructionText,
            SizedBox(0, 40, false),
            resumeButton,
            SizedBox(0, 20, false),
            mainMenuButton,
            SizedBox(0, 20, false),
            quitButton
        }, false, MainAxisAlignment::Center, CrossAxisAlignment::Center);
        
        // Add to widget manager
        addWidget(Container(Colors::Transparent, 0, 0, width, height, Center(layout, false)));
    }
    
    void onExit() override {
        std::cout << "Exiting Pause Scene" << std::endl;
    }
};

int main() {
    std::cout << "Fern Scene Management Demo" << std::endl;
    
    Fern::initialize();
    
    // Register all scenes
    REGISTER_SCENE("MainMenu", MainMenuScene);
    REGISTER_SCENE("GameScene", GameScene);
    REGISTER_SCENE("SettingsScene", SettingsScene);
    REGISTER_SCENE("PauseScene", PauseScene);
    
    // Start with main menu
    pushScene("MainMenu");
    
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    
    return 0;
}
