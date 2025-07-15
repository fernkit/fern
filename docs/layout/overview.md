# Layout System Overview

Fern's layout system provides powerful and flexible tools for organizing widgets in your application. The system is based on composable layout widgets that can be nested to create complex UI structures.

## Table of Contents
- [Core Concepts](#core-concepts)
- [Layout Widgets](#layout-widgets)
- [Basic Usage](#basic-usage)
- [Advanced Patterns](#advanced-patterns)
- [Examples](#examples)
- [Best Practices](#best-practices)

## Core Concepts

### Layout Philosophy
Fern's layout system follows these principles:
- **Composable**: Layout widgets can be nested arbitrarily
- **Declarative**: Describe what you want, not how to achieve it
- **Flexible**: Adapts to different screen sizes and content
- **Predictable**: Consistent behavior across platforms

### Layout Widgets vs Content Widgets
- **Content Widgets**: Display actual content (Text, Button, etc.)
- **Layout Widgets**: Arrange other widgets (Column, Row, Center, etc.)

### Widget Hierarchy
```
Root
├── CenterWidget
│   └── Column
│       ├── Text ("Title")
│       ├── SizedBox (spacing)
│       └── Row
│           ├── Button ("OK")
│           ├── SizedBox (spacing)
│           └── Button ("Cancel")
```

## Layout Widgets

### Linear Layouts
- **Column**: Arranges widgets vertically
- **Row**: Arranges widgets horizontally

### Positioning Layouts
- **Center**: Centers child widget
- **Padding**: Adds space around child widget
- **Expanded**: Expands child to fill available space

### Utility Widgets
- **SizedBox**: Fixed-size spacing widget
- **Spacing**: Flexible spacing widget
- **Container**: Groups widgets with styling

## Basic Usage

### Column Layout
```cpp
std::vector<std::shared_ptr<Widget>> children = {
    Text(Point(0, 0), "Item 1", 2, Colors::White),
    Text(Point(0, 0), "Item 2", 2, Colors::White),
    Text(Point(0, 0), "Item 3", 2, Colors::White)
};

auto column = Column(children);
addWidget(column);
```

### Row Layout
```cpp
std::vector<std::shared_ptr<Widget>> children = {
    Button(ButtonConfig(0, 0, 100, 40, "OK")),
    SizedBox(20, 0),  // Horizontal spacing
    Button(ButtonConfig(0, 0, 100, 40, "Cancel"))
};

auto row = Row(children);
addWidget(row);
```

### Centered Layout
```cpp
auto widget = Text(Point(0, 0), "Centered!", 3, Colors::White);

int width = Fern::getWidth();
int height = Fern::getHeight();
auto centerWidget = std::make_shared<CenterWidget>(0, 0, width, height);
centerWidget->add(widget);

addWidget(centerWidget);
```

### Nested Layouts
```cpp
// Create button row
std::vector<std::shared_ptr<Widget>> buttonRow = {
    Button(ButtonConfig(0, 0, 100, 40, "Save")),
    SizedBox(15, 0),
    Button(ButtonConfig(0, 0, 100, 40, "Cancel"))
};

// Create main column
std::vector<std::shared_ptr<Widget>> mainColumn = {
    Text(Point(0, 0), "Document Editor", 3, Colors::White),
    SizedBox(0, 30),
    Text(Point(0, 0), "Choose an action:", 2, Colors::Gray),
    SizedBox(0, 20),
    Row(buttonRow)  // Nested row inside column
};

// Center everything
int width = Fern::getWidth();
int height = Fern::getHeight();
auto centerWidget = std::make_shared<CenterWidget>(0, 0, width, height);
centerWidget->add(Column(mainColumn));

addWidget(centerWidget);
```

## Advanced Patterns

### Responsive Design
```cpp
void createResponsiveLayout() {
    int screenWidth = Fern::getWidth();
    int screenHeight = Fern::getHeight();
    
    // Adjust button size based on screen width
    int buttonWidth = std::min(200, screenWidth - 40);
    int buttonHeight = 50;
    
    // Choose layout based on screen size
    if (screenWidth > 600) {
        // Wide screen: use row layout
        std::vector<std::shared_ptr<Widget>> buttons = {
            Button(ButtonConfig(0, 0, buttonWidth, buttonHeight, "Action 1")),
            SizedBox(20, 0),
            Button(ButtonConfig(0, 0, buttonWidth, buttonHeight, "Action 2"))
        };
        
        auto layout = Row(buttons);
        // Add to center widget...
    } else {
        // Narrow screen: use column layout
        std::vector<std::shared_ptr<Widget>> buttons = {
            Button(ButtonConfig(0, 0, buttonWidth, buttonHeight, "Action 1")),
            SizedBox(0, 15),
            Button(ButtonConfig(0, 0, buttonWidth, buttonHeight, "Action 2"))
        };
        
        auto layout = Column(buttons);
        // Add to center widget...
    }
}
```

### Grid Layout Simulation
```cpp
void createGridLayout() {
    // Create 2x2 grid using nested rows and columns
    
    // Row 1
    std::vector<std::shared_ptr<Widget>> row1 = {
        Button(ButtonConfig(0, 0, 100, 60, "1")),
        SizedBox(10, 0),
        Button(ButtonConfig(0, 0, 100, 60, "2"))
    };
    
    // Row 2
    std::vector<std::shared_ptr<Widget>> row2 = {
        Button(ButtonConfig(0, 0, 100, 60, "3")),
        SizedBox(10, 0),
        Button(ButtonConfig(0, 0, 100, 60, "4"))
    };
    
    // Combine rows
    std::vector<std::shared_ptr<Widget>> grid = {
        Row(row1),
        SizedBox(0, 10),
        Row(row2)
    };
    
    auto gridLayout = Column(grid);
    
    // Center the grid
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    auto centerWidget = std::make_shared<CenterWidget>(0, 0, width, height);
    centerWidget->add(gridLayout);
    
    addWidget(centerWidget);
}
```

### Form Layout
```cpp
void createFormLayout() {
    // Create form fields
    auto nameField = TextInput(TextInputConfig(0, 0, 300, 40, "Enter your name"));
    auto emailField = TextInput(TextInputConfig(0, 0, 300, 40, "Enter your email"));
    
    // Create form
    std::vector<std::shared_ptr<Widget>> form = {
        Text(Point(0, 0), "Contact Form", 3, Colors::White),
        SizedBox(0, 30),
        
        Text(Point(0, 0), "Name:", 2, Colors::Gray),
        SizedBox(0, 10),
        nameField,
        SizedBox(0, 20),
        
        Text(Point(0, 0), "Email:", 2, Colors::Gray),
        SizedBox(0, 10),
        emailField,
        SizedBox(0, 30),
        
        // Submit button
        Button(ButtonConfig(0, 0, 200, 50, "Submit"))
    };
    
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    auto centerWidget = std::make_shared<CenterWidget>(0, 0, width, height);
    centerWidget->add(Column(form));
    
    addWidget(centerWidget);
}
```

## Examples

### Example 1: Dashboard Layout
```cpp
#include <fern/fern.hpp>
#include <iostream>

using namespace Fern;

void setupUI() {
    // Header
    auto header = Text(Point(0, 0), "Dashboard", 4, Colors::White);
    
    // Stats row
    auto createStatCard = [](const std::string& title, const std::string& value) {
        std::vector<std::shared_ptr<Widget>> card = {
            Text(Point(0, 0), title, 2, Colors::Gray),
            SizedBox(0, 10),
            Text(Point(0, 0), value, 3, Colors::White)
        };
        return Column(card);
    };
    
    std::vector<std::shared_ptr<Widget>> statsRow = {
        createStatCard("Users", "1,234"),
        SizedBox(40, 0),
        createStatCard("Orders", "567"),
        SizedBox(40, 0),
        createStatCard("Revenue", "$12,345")
    };
    
    // Action buttons
    std::vector<std::shared_ptr<Widget>> actionRow = {
        Button(ButtonConfig(0, 0, 120, 40, "View Reports")),
        SizedBox(20, 0),
        Button(ButtonConfig(0, 0, 120, 40, "Settings"))
    };
    
    // Main layout
    std::vector<std::shared_ptr<Widget>> mainLayout = {
        header,
        SizedBox(0, 40),
        Row(statsRow),
        SizedBox(0, 50),
        Row(actionRow)
    };
    
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    auto centerWidget = std::make_shared<CenterWidget>(0, 0, width, height);
    centerWidget->add(Column(mainLayout));
    
    addWidget(centerWidget);
}

void draw() {
    Draw::fill(Colors::DarkBlue);
}

int main() {
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}
```

### Example 2: Navigation Menu
```cpp
#include <fern/fern.hpp>
#include <iostream>

using namespace Fern;

void setupUI() {
    // Navigation buttons
    auto createNavButton = [](const std::string& text) {
        ButtonStyle style;
        style.normalColor(Colors::DarkGray)
             .hoverColor(Colors::LightGray)
             .pressColor(Colors::Gray)
             .textColor(Colors::White)
             .textScale(2);
        
        auto button = Button(ButtonConfig(0, 0, 150, 50, text).style(style));
        button->onClick.connect([text]() {
            std::cout << "Navigating to: " << text << std::endl;
        });
        return button;
    };
    
    // Create navigation menu
    std::vector<std::shared_ptr<Widget>> navMenu = {
        Text(Point(0, 0), "Navigation", 3, Colors::White),
        SizedBox(0, 30),
        createNavButton("Home"),
        SizedBox(0, 15),
        createNavButton("Products"),
        SizedBox(0, 15),
        createNavButton("About"),
        SizedBox(0, 15),
        createNavButton("Contact")
    };
    
    // Content area
    std::vector<std::shared_ptr<Widget>> content = {
        Text(Point(0, 0), "Main Content", 3, Colors::White),
        SizedBox(0, 20),
        Text(Point(0, 0), "Welcome to our application!", 2, Colors::Gray),
        SizedBox(0, 20),
        Text(Point(0, 0), "Select a navigation item from the left.", 2, Colors::Gray)
    };
    
    // Main layout (simulated two-column)
    std::vector<std::shared_ptr<Widget>> leftColumn = {
        Column(navMenu)
    };
    
    std::vector<std::shared_ptr<Widget>> rightColumn = {
        Column(content)
    };
    
    std::vector<std::shared_ptr<Widget>> mainRow = {
        Column(leftColumn),
        SizedBox(80, 0),  // Gap between columns
        Column(rightColumn)
    };
    
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    auto centerWidget = std::make_shared<CenterWidget>(0, 0, width, height);
    centerWidget->add(Row(mainRow));
    
    addWidget(centerWidget);
}

void draw() {
    Draw::fill(Colors::Black);
}

int main() {
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}
```

### Example 3: Settings Panel
```cpp
#include <fern/fern.hpp>
#include <iostream>

using namespace Fern;

void setupUI() {
    // Settings section helper
    auto createSettingsSection = [](const std::string& title, 
                                   const std::vector<std::shared_ptr<Widget>>& controls) {
        std::vector<std::shared_ptr<Widget>> section = {
            Text(Point(0, 0), title, 2, Colors::LightBlue),
            SizedBox(0, 15)
        };
        
        // Add controls
        for (const auto& control : controls) {
            section.push_back(control);
            section.push_back(SizedBox(0, 10));
        }
        
        return Column(section);
    };
    
    // Audio controls
    std::vector<std::shared_ptr<Widget>> audioControls = {
        Text(Point(0, 0), "Volume: 50%", 2, Colors::White),
        Text(Point(0, 0), "Quality: High", 2, Colors::White)
    };
    
    // Display controls
    std::vector<std::shared_ptr<Widget>> displayControls = {
        Text(Point(0, 0), "Resolution: 1920x1080", 2, Colors::White),
        Text(Point(0, 0), "Fullscreen: Enabled", 2, Colors::White)
    };
    
    // Main settings layout
    std::vector<std::shared_ptr<Widget>> settings = {
        Text(Point(0, 0), "Settings", 4, Colors::White),
        SizedBox(0, 40),
        createSettingsSection("Audio", audioControls),
        SizedBox(0, 30),
        createSettingsSection("Display", displayControls),
        SizedBox(0, 40),
        Row({
            Button(ButtonConfig(0, 0, 100, 40, "Save")),
            SizedBox(20, 0),
            Button(ButtonConfig(0, 0, 100, 40, "Reset"))
        })
    };
    
    int width = Fern::getWidth();
    int height = Fern::getHeight();
    auto centerWidget = std::make_shared<CenterWidget>(0, 0, width, height);
    centerWidget->add(Column(settings));
    
    addWidget(centerWidget);
}

void draw() {
    Draw::fill(Colors::DarkGray);
}

int main() {
    Fern::initialize();
    setupUI();
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}
```

## Best Practices

### 1. Consistent Spacing
Use consistent spacing throughout your application:
```cpp
namespace Spacing {
    constexpr int SMALL = 10;
    constexpr int MEDIUM = 20;
    constexpr int LARGE = 40;
}

// Usage
SizedBox(0, Spacing::MEDIUM)  // Consistent vertical spacing
SizedBox(Spacing::SMALL, 0)   // Consistent horizontal spacing
```

### 2. Responsive Design
Always consider different screen sizes:
```cpp
void createResponsiveButton() {
    int screenWidth = Fern::getWidth();
    int buttonWidth = std::min(300, screenWidth - 40);  // Never wider than screen - margin
    
    auto button = Button(ButtonConfig(0, 0, buttonWidth, 50, "Responsive"));
    // ...
}
```

### 3. Semantic Structure
Use meaningful widget hierarchies:
```cpp
// Good: Clear structure
std::vector<std::shared_ptr<Widget>> form = {
    header,
    SizedBox(0, 30),
    inputFields,
    SizedBox(0, 20),
    actionButtons
};

// Avoid: Flat structure without meaning
std::vector<std::shared_ptr<Widget>> messyLayout = {
    widget1, widget2, widget3, widget4, widget5  // What do these represent?
};
```

### 4. Reusable Components
Create reusable layout components:
```cpp
class FormField {
public:
    static std::shared_ptr<Widget> create(const std::string& label, 
                                         std::shared_ptr<Widget> input) {
        std::vector<std::shared_ptr<Widget>> field = {
            Text(Point(0, 0), label, 2, Colors::Gray),
            SizedBox(0, 10),
            input
        };
        return Column(field);
    }
};

// Usage
auto nameField = FormField::create("Name:", nameInput);
auto emailField = FormField::create("Email:", emailInput);
```

### 5. Performance Considerations
- Cache layout widgets when possible
- Avoid deeply nested layouts
- Use appropriate spacing rather than empty widgets

## Common Patterns

### Card Layout
```cpp
std::shared_ptr<Widget> createCard(const std::string& title, const std::string& content) {
    std::vector<std::shared_ptr<Widget>> card = {
        Text(Point(0, 0), title, 3, Colors::White),
        SizedBox(0, 15),
        Text(Point(0, 0), content, 2, Colors::Gray)
    };
    return Column(card);
}
```

### List Layout
```cpp
std::shared_ptr<Widget> createList(const std::vector<std::string>& items) {
    std::vector<std::shared_ptr<Widget>> list;
    
    for (const auto& item : items) {
        list.push_back(Text(Point(0, 0), "• " + item, 2, Colors::White));
        list.push_back(SizedBox(0, 10));
    }
    
    return Column(list);
}
```

### Toolbar Layout
```cpp
std::shared_ptr<Widget> createToolbar(const std::vector<std::shared_ptr<Widget>>& tools) {
    std::vector<std::shared_ptr<Widget>> toolbar;
    
    for (size_t i = 0; i < tools.size(); ++i) {
        toolbar.push_back(tools[i]);
        if (i < tools.size() - 1) {
            toolbar.push_back(SizedBox(10, 0));
        }
    }
    
    return Row(toolbar);
}
```

## Troubleshooting

### Layout Not Centering
- Ensure CenterWidget has proper dimensions
- Check that child widgets have appropriate sizes
- Verify screen dimensions are correct

### Widgets Overlapping
- Add proper spacing between widgets
- Check widget dimensions
- Ensure layout widgets are properly nested

### Performance Issues
- Avoid excessive nesting
- Cache layout widgets
- Use appropriate spacing values

### Responsive Issues
- Test on different screen sizes
- Use relative sizing where possible
- Consider different layout strategies for different screen sizes

---

**Related Documentation:**
- [Column Layout](column.md)
- [Row Layout](row.md)
- [Center Layout](center.md)
- [Container System](containers.md)
- [Button Widget](../widgets/button.md)
- [Text Widget](../widgets/text.md)
