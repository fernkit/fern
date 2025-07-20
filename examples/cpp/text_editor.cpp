#include <fern/fern.hpp>
#include <string>

using namespace Fern;

// Modern text editor color scheme
namespace EditorColors {
    const uint32_t Background = 0xFF1E1E1E;      // Dark editor background
    const uint32_t Sidebar = 0xFF2D2D30;         // Sidebar color
    const uint32_t TextArea = 0xFF252526;        // Text area background
    const uint32_t Text = 0xFFCCCCCC;            // Main text color
    const uint32_t LineNumbers = 0xFF858585;     // Line number color
    const uint32_t Selection = 0xFF264F78;       // Selection color
    const uint32_t Button = 0xFF0E639C;          // Button color
    const uint32_t ButtonHover = 0xFF1177BB;     // Button hover
}

// Global state
static std::string editorText = "Welcome to Fern Text Editor!\n\nStart typing here...";
static std::shared_ptr<TextInputWidget> textInput;
static std::shared_ptr<TextWidget> statusText;
static std::shared_ptr<CenterWidget> centerWidget;
static int wordCount = 0;
static int charCount = 0;

void updateStatus() {
    // Count words and characters
    charCount = editorText.length();
    wordCount = 0;
    
    bool inWord = false;
    for (char c : editorText) {
        if (c != ' ' && c != '\t' && c != '\n' && c != '\r') {
            if (!inWord) {
                wordCount++;
                inWord = true;
            }
        } else {
            inWord = false;
        }
    }
    
    std::string status = "Characters: " + std::to_string(charCount) + 
                        " | Words: " + std::to_string(wordCount);
    statusText->setText(status);
}

void setupUI() {
    // Title
    auto titleText = Text(Point(0, 0), "Fern Text Editor", 3, EditorColors::Text);
    
    // Text input area
    TextInputStyle inputStyle;
    inputStyle.backgroundColor(EditorColors::TextArea)
             .textColor(EditorColors::Text)
             .borderColor(EditorColors::Selection)
             .borderWidth(2)
             .fontSize(2)
             .padding(10);
    
    textInput = TextInput(TextInputConfig(0, 0, 500, 300)
        .placeholder("Start typing...")
        .style(inputStyle));
    
    textInput->setText(editorText);
    
    // Connect to text change events
    textInput->onTextChanged.connect([](const std::string& newText) {
        editorText = newText;
        updateStatus();
    });
    
    // Status bar
    statusText = Text(Point(0, 0), "", 2, EditorColors::LineNumbers);
    
    // Action buttons
    ButtonStyle buttonStyle;
    buttonStyle.normalColor(EditorColors::Button)
              .hoverColor(EditorColors::ButtonHover)
              .pressColor(0xFF0A5A94)
              .textColor(0xFFFFFFFF)
              .textScale(2)
              .borderRadius(4);
    
    auto clearBtn = Button(ButtonConfig(0, 0, 80, 35, "Clear").style(buttonStyle));
    clearBtn->onClick.connect([]() {
        textInput->setText("");
        editorText = "";
        updateStatus();
    });
    
    auto saveBtn = Button(ButtonConfig(0, 0, 80, 35, "Save").style(buttonStyle));
    saveBtn->onClick.connect([]() {
        // In a real editor, this would save to file
        statusText->setText("File saved! (simulated)");
    });
    
    auto loadBtn = Button(ButtonConfig(0, 0, 80, 35, "Load").style(buttonStyle));
    loadBtn->onClick.connect([]() {
        // Load sample text
        editorText = "// Sample C++ Code\n#include <iostream>\n\nint main() {\n    std::cout << \"Hello, Fern!\" << std::endl;\n    return 0;\n}";
        textInput->setText(editorText);
        updateStatus();
    });
    
    // Button row
    std::vector<std::shared_ptr<Widget>> buttonRow = {
        clearBtn, SizedBox(10, 0), saveBtn, SizedBox(10, 0), loadBtn
    };
    
    // Main layout
    std::vector<std::shared_ptr<Widget>> mainLayout = {
        titleText,
        SizedBox(0, 20),
        textInput,
        SizedBox(0, 15),
        statusText,
        SizedBox(0, 15),
        Row(buttonRow)
    };
    
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    centerWidget = std::make_shared<CenterWidget>(0, 0, width, height);
    centerWidget->add(Column(mainLayout));
    addWidget(centerWidget);
    
    // Initial status update
    updateStatus();
}

void onWindowResize(int newWidth, int newHeight) {
    if (centerWidget) {
        centerWidget->resize(newWidth, newHeight);
    }
}

void draw() {
    Draw::fill(EditorColors::Background);
}

int main() {
    Fern::initialize(700, 600);
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::setWindowResizeCallback(onWindowResize);
    Fern::startRenderLoop();
    return 0;
}
