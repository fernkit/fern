# Understanding Layouts

Layout widgets are the secret to building beautiful, responsive user interfaces without manually calculating pixel positions. If you've ever struggled with positioning elements on different screen sizes, or wondered how professional applications automatically arrange their content, this guide will teach you everything about Fern's powerful layout system.

## The Problem Layouts Solve

Imagine trying to build a login screen by manually positioning every element:

```cpp
// The hard way - manual positioning
void setupUI() {
    auto title = Text(Point(350, 100), "Login", 4, Colors::White);
    auto usernameLabel = Text(Point(300, 180), "Username:", 2, Colors::Gray);
    auto usernameInput = TextInput(Point(300, 210), Size(200, 30));
    auto passwordLabel = Text(Point(300, 260), "Password:", 2, Colors::Gray);
    auto passwordInput = TextInput(Point(300, 290), Size(200, 30));
    auto loginButton = Button(ButtonConfig(350, 340, 100, 40, "Login"));
    
    // What happens when the window is resized? What about different screen sizes?
    // What if you want to add another field? Recalculate everything!
}
```

This approach creates several problems:
- **Fragile positioning**: Elements break when window size changes
- **Maintenance nightmare**: Adding one element means repositioning everything
- **No responsiveness**: Doesn't adapt to different screen sizes
- **Repetitive math**: Constantly calculating spacing and alignment

## How Layouts Work

Layout widgets solve these problems by automatically calculating positions and sizes for their children. Instead of specifying exact pixel coordinates, you describe relationships and let the layout engine do the math.

### The Layout Philosophy

Think of layouts like instructions to an assistant:
- "Arrange these items vertically, with 20 pixels between each"
- "Center this content in the available space"
- "Put these buttons in a horizontal row, aligned to the right"

The layout widget handles all the pixel calculations, adapting automatically when sizes change.

### Layout Hierarchy

Layouts work through parent-child relationships:

```cpp
// The layout way - describe relationships
void setupUI() {
    std::vector<std::shared_ptr<Widget>> children = {
        Text(Point(0, 0), "Login", 4, Colors::White),
        SizedBox(0, 30),  // 30 pixels spacing
        Text(Point(0, 0), "Username:", 2, Colors::Gray),
        TextInput(Point(0, 0), Size(200, 30)),
        SizedBox(0, 20),  // 20 pixels spacing
        Text(Point(0, 0), "Password:", 2, Colors::Gray), 
        TextInput(Point(0, 0), Size(200, 30)),
        SizedBox(0, 30),  // 30 pixels spacing
        Button(ButtonConfig(0, 0, 100, 40, "Login"))
    };
    
    auto centerWidget = std::make_shared<CenterWidget>(0, 0, Fern::getWidth(), Fern::getHeight());
    centerWidget->add(Column(children));
    addWidget(centerWidget);
}
```

Notice how every widget uses `Point(0, 0)` - the layout calculates the real positions.

## Core Layout Widgets

### Column Widget

The Column widget arranges its children vertically, one below the other.

```cpp
std::vector<std::shared_ptr<Widget>> items = {
    Text(Point(0, 0), "First Item", 2, Colors::White),
    Text(Point(0, 0), "Second Item", 2, Colors::White),
    Text(Point(0, 0), "Third Item", 2, Colors::White)
};

auto column = Column(items);
```

**Key characteristics:**
- Children are stacked vertically
- Each child's width can be different
- The column's height is the sum of all children's heights
- Spacing between children can be controlled with SizedBox widgets

### Row Widget

The Row widget arranges its children horizontally, side by side.

```cpp
std::vector<std::shared_ptr<Widget>> buttons = {
    Button(ButtonConfig(0, 0, 80, 40, "OK")),
    Button(ButtonConfig(0, 0, 80, 40, "Cancel")),
    Button(ButtonConfig(0, 0, 80, 40, "Help"))
};

auto row = Row(buttons);
```

**Key characteristics:**
- Children are placed side by side horizontally
- Each child's height can be different
- The row's width is the sum of all children's widths
- Spacing between children can be controlled with SizedBox widgets

### CenterWidget

The CenterWidget positions its child content in the center of the available space.

```cpp
auto content = Text(Point(0, 0), "Centered Content", 3, Colors::White);

auto centerWidget = std::make_shared<CenterWidget>(0, 0, Fern::getWidth(), Fern::getHeight());
centerWidget->add(content);
```

**Key characteristics:**
- Takes a specific width and height (usually the full window)
- Positions its child content exactly in the center
- Automatically recalculates when window size changes
- Can center any widget or layout

### SizedBox

SizedBox creates spacing or fixed-size areas in your layouts.

```cpp
// Vertical spacing in a column
SizedBox(0, 20)  // 20 pixels of vertical space

// Horizontal spacing in a row  
SizedBox(20, 0)  // 20 pixels of horizontal space

// Fixed size area
SizedBox(100, 50)  // 100x50 pixel area
```

**Key characteristics:**
- Creates empty space of specified dimensions
- Essential for proper spacing between elements
- Can be used as a placeholder for future content
- Helps prevent text and widgets from overlapping

## Layout Alignment

One of the most powerful features of layout widgets is their ability to align children in different ways.

### Main Axis vs Cross Axis

Understanding the difference between main axis and cross axis is crucial:

- **Column**: Main axis is vertical (top to bottom), cross axis is horizontal (left to right)
- **Row**: Main axis is horizontal (left to right), cross axis is vertical (top to bottom)

### MainAxisAlignment

Controls how children are distributed along the main axis:

```cpp
auto column = Column(children);

// Alignment options:
column.setMainAxisAlignment(MainAxisAlignment::Start);    // Pack at beginning
column.setMainAxisAlignment(MainAxisAlignment::Center);   // Pack in center  
column.setMainAxisAlignment(MainAxisAlignment::End);      // Pack at end
column.setMainAxisAlignment(MainAxisAlignment::SpaceBetween); // Distribute evenly
column.setMainAxisAlignment(MainAxisAlignment::SpaceAround);  // Equal space around each
```

### CrossAxisAlignment

Controls how children are aligned across the cross axis:

```cpp
auto column = Column(children);

// Alignment options:
column.setCrossAxisAlignment(CrossAxisAlignment::Start);   // Align to left (for Column)
column.setCrossAxisAlignment(CrossAxisAlignment::Center);  // Center horizontally
column.setCrossAxisAlignment(CrossAxisAlignment::End);     // Align to right
column.setCrossAxisAlignment(CrossAxisAlignment::Stretch); // Fill available width
```

### Practical Alignment Examples

```cpp
// Create a header bar with logo on left, menu on right
std::vector<std::shared_ptr<Widget>> headerItems = {
    Text(Point(0, 0), "MyApp", 3, Colors::White),
    SizedBox(100, 0),  // Spacer to push menu right
    Button(ButtonConfig(0, 0, 80, 30, "Menu"))
};

auto headerRow = Row(headerItems);
headerRow.setMainAxisAlignment(MainAxisAlignment::SpaceBetween);
headerRow.setCrossAxisAlignment(CrossAxisAlignment::Center);

// Create a centered login form
std::vector<std::shared_ptr<Widget>> loginForm = {
    Text(Point(0, 0), "Welcome Back", 4, Colors::White),
    SizedBox(0, 30),
    TextInput(Point(0, 0), Size(250, 35)),
    SizedBox(0, 15),
    TextInput(Point(0, 0), Size(250, 35)),
    SizedBox(0, 25),
    Button(ButtonConfig(0, 0, 120, 40, "Sign In"))
};

auto loginColumn = Column(loginForm);
loginColumn.setMainAxisAlignment(MainAxisAlignment::Center);
loginColumn.setCrossAxisAlignment(CrossAxisAlignment::Center);
```

## Nested Layouts

The real power of layouts comes from nesting them inside each other to create complex, responsive designs.

### Building Complex Interfaces

```cpp
void setupUI() {
    // Top navigation bar
    std::vector<std::shared_ptr<Widget>> navItems = {
        Text(Point(0, 0), "MyApp", 3, Colors::White),
        Button(ButtonConfig(0, 0, 70, 30, "Home")),
        Button(ButtonConfig(0, 0, 70, 30, "About")),
        Button(ButtonConfig(0, 0, 70, 30, "Contact"))
    };
    auto navRow = Row(navItems);
    navRow.setMainAxisAlignment(MainAxisAlignment::SpaceBetween);
    
    // Main content area
    std::vector<std::shared_ptr<Widget>> contentItems = {
        Text(Point(0, 0), "Welcome to Our Website", 4, Colors::White),
        SizedBox(0, 20),
        Text(Point(0, 0), "This is the main content area where important information goes.", 2, Colors::Gray),
        SizedBox(0, 30),
        
        // Button row within content
        Row({
            Button(ButtonConfig(0, 0, 100, 40, "Get Started")),
            SizedBox(20, 0),
            Button(ButtonConfig(0, 0, 100, 40, "Learn More"))
        })
    };
    auto contentColumn = Column(contentItems);
    contentColumn.setCrossAxisAlignment(CrossAxisAlignment::Center);
    
    // Footer
    auto footer = Text(Point(0, 0), "© 2025 MyApp. All rights reserved.", 1, Colors::Gray);
    
    // Combine everything in a main layout
    std::vector<std::shared_ptr<Widget>> mainLayout = {
        navRow,
        SizedBox(0, 40),
        contentColumn,
        SizedBox(0, 40),
        footer
    };
    
    auto mainColumn = Column(mainLayout);
    mainColumn.setMainAxisAlignment(MainAxisAlignment::SpaceBetween);
    
    // Center the entire layout
    auto centerWidget = std::make_shared<CenterWidget>(0, 0, Fern::getWidth(), Fern::getHeight());
    centerWidget->add(mainColumn);
    addWidget(centerWidget);
}
```

This creates a complete page layout with:
- Navigation bar with logo and menu buttons
- Centered main content with text and action buttons
- Footer at the bottom
- Everything automatically responsive to window size changes

### Layout Composition Patterns

**Sidebar Layout:**
```cpp
// Main content area
auto mainContent = Column({/* main content widgets */});

// Sidebar  
auto sidebar = Column({/* sidebar widgets */});

// Combine horizontally
auto layout = Row({sidebar, SizedBox(20, 0), mainContent});
```

**Card Layout:**
```cpp
// Individual card
auto card = Column({
    Text(Point(0, 0), "Card Title", 3, Colors::White),
    SizedBox(0, 10),
    Text(Point(0, 0), "Card description goes here", 2, Colors::Gray),
    SizedBox(0, 15),
    Button(ButtonConfig(0, 0, 80, 30, "Action"))
});

// Grid of cards
auto cardRow1 = Row({card1, SizedBox(20, 0), card2, SizedBox(20, 0), card3});
auto cardRow2 = Row({card4, SizedBox(20, 0), card5, SizedBox(20, 0), card6});

auto cardGrid = Column({cardRow1, SizedBox(0, 20), cardRow2});
```

## Responsive Design

Layouts automatically adapt to different screen sizes, but you can enhance responsiveness with dynamic calculations.

### Dynamic Sizing

```cpp
void setupUI() {
    int screenWidth = Fern::getWidth();
    int screenHeight = Fern::getHeight();
    
    // Calculate responsive dimensions
    int contentWidth = std::min(600, screenWidth - 40);  // Max 600px, with 20px margins
    int buttonWidth = contentWidth / 3 - 10;  // Three buttons with spacing
    
    std::vector<std::shared_ptr<Widget>> content = {
        Text(Point(0, 0), "Responsive Layout", 4, Colors::White),
        SizedBox(0, 30),
        
        // Responsive button row
        Row({
            Button(ButtonConfig(0, 0, buttonWidth, 40, "Button 1")),
            SizedBox(10, 0),
            Button(ButtonConfig(0, 0, buttonWidth, 40, "Button 2")),
            SizedBox(10, 0),
            Button(ButtonConfig(0, 0, buttonWidth, 40, "Button 3"))
        })
    };
    
    auto centerWidget = std::make_shared<CenterWidget>(0, 0, screenWidth, screenHeight);
    centerWidget->add(Column(content));
    addWidget(centerWidget);
}
```

### Window Resize Handling

```cpp
void handleWindowResize(int newWidth, int newHeight) {
    // Clear existing widgets
    WidgetManager::getInstance().clear();
    
    // Recreate UI with new dimensions
    setupUI();
}

int main() {
    Fern::initialize();
    setupUI();
    
    // Set up resize callback
    Fern::setWindowResizeCallback(handleWindowResize);
    
    Fern::setDrawCallback(draw);
    Fern::startRenderLoop();
    return 0;
}
```

## Common Layout Patterns

### Form Layout

```cpp
std::vector<std::shared_ptr<Widget>> createFormField(const std::string& label, int inputWidth = 200) {
    return {
        Text(Point(0, 0), label, 2, Colors::White),
        SizedBox(0, 5),
        TextInput(Point(0, 0), Size(inputWidth, 30)),
        SizedBox(0, 20)
    };
}

void setupUI() {
    std::vector<std::shared_ptr<Widget>> formElements;
    
    // Add form fields
    auto nameField = createFormField("Full Name:");
    auto emailField = createFormField("Email Address:");
    auto passwordField = createFormField("Password:");
    
    formElements.insert(formElements.end(), nameField.begin(), nameField.end());
    formElements.insert(formElements.end(), emailField.begin(), emailField.end());
    formElements.insert(formElements.end(), passwordField.begin(), passwordField.end());
    
    // Add submit button
    formElements.push_back(Button(ButtonConfig(0, 0, 120, 40, "Submit")));
    
    auto form = Column(formElements);
    form.setCrossAxisAlignment(CrossAxisAlignment::Start);
    
    auto centerWidget = std::make_shared<CenterWidget>(0, 0, Fern::getWidth(), Fern::getHeight());
    centerWidget->add(form);
    addWidget(centerWidget);
}
```

### Toolbar Layout

```cpp
void createToolbar() {
    std::vector<std::shared_ptr<Widget>> leftTools = {
        Button(ButtonConfig(0, 0, 40, 30, "⬅")),
        Button(ButtonConfig(0, 0, 40, 30, "➡")),
        SizedBox(10, 0),
        Button(ButtonConfig(0, 0, 60, 30, "Home"))
    };
    
    std::vector<std::shared_ptr<Widget>> rightTools = {
        Button(ButtonConfig(0, 0, 60, 30, "Search")),
        Button(ButtonConfig(0, 0, 60, 30, "Settings")),
        Button(ButtonConfig(0, 0, 60, 30, "Profile"))
    };
    
    auto leftRow = Row(leftTools);
    auto rightRow = Row(rightTools);
    
    std::vector<std::shared_ptr<Widget>> toolbarItems = {
        leftRow,
        rightRow  // SizedBox between them would center, SpaceBetween alignment pushes them apart
    };
    
    auto toolbar = Row(toolbarItems);
    toolbar.setMainAxisAlignment(MainAxisAlignment::SpaceBetween);
    toolbar.setCrossAxisAlignment(CrossAxisAlignment::Center);
    
    addWidget(toolbar);
}
```

### Modal Dialog Layout

```cpp
void showModal() {
    // Semi-transparent overlay
    auto overlay = std::make_shared<Widget>();  // Custom widget that draws transparent background
    
    // Modal content
    std::vector<std::shared_ptr<Widget>> modalContent = {
        Text(Point(0, 0), "Confirm Action", 3, Colors::White),
        SizedBox(0, 20),
        Text(Point(0, 0), "Are you sure you want to proceed?", 2, Colors::Gray),
        SizedBox(0, 30),
        
        Row({
            Button(ButtonConfig(0, 0, 80, 35, "Cancel")),
            SizedBox(20, 0),
            Button(ButtonConfig(0, 0, 80, 35, "OK"))
        })
    };
    
    auto modal = Column(modalContent);
    modal.setCrossAxisAlignment(CrossAxisAlignment::Center);
    
    auto centerWidget = std::make_shared<CenterWidget>(0, 0, Fern::getWidth(), Fern::getHeight());
    centerWidget->add(modal);
    
    addWidget(overlay);
    addWidget(centerWidget);
}
```

## Best Practices

### Planning Your Layout Structure

Before writing code, sketch your interface and identify the layout hierarchy:

1. **Identify main sections**: Header, content, footer, sidebar
2. **Determine directions**: Which sections stack vertically vs horizontally?
3. **Plan spacing**: Where do you need SizedBox widgets?
4. **Consider responsiveness**: How should it adapt to different sizes?

### Widget Positioning Rules

- **Always use Point(0, 0)** for widgets inside layouts
- **Use actual coordinates only** for widgets added directly with `addWidget()`
- **Let layouts handle positioning** - don't try to manually adjust positions

### Spacing Guidelines

- **Related content**: 10-15 pixels spacing
- **Section separation**: 20-30 pixels spacing  
- **Major sections**: 40+ pixels spacing
- **Use SizedBox consistently** instead of manual margins

### Performance Considerations

- **Avoid deep nesting**: Too many nested layouts can impact performance
- **Cache layout widgets**: Don't recreate layouts every frame
- **Use static variables**: For layouts that change based on state

### Debugging Layout Issues

Common problems and solutions:

**Widgets not appearing**: Check that you're using Point(0, 0) for layout children

**Overlapping text**: Add proper SizedBox spacing between text elements

**Content cut off**: Ensure your CenterWidget dimensions are large enough

**Misaligned elements**: Check your MainAxisAlignment and CrossAxisAlignment settings

## Advanced Layout Techniques

### Expanded Widgets

Use Expanded widgets to fill available space proportionally:

```cpp
auto row = Row({
    Button(ButtonConfig(0, 0, 100, 40, "Fixed")),  // Fixed width
    Expanded(Text(Point(0, 0), "Flexible", 2, Colors::White)),  // Takes remaining space
    Button(ButtonConfig(0, 0, 100, 40, "Fixed"))   // Fixed width
});
```

### Container Widgets

Container widgets provide padding, margins, and decoration:

```cpp
auto container = std::make_shared<Container>(
    Point(0, 0), 
    Size(300, 200),
    ContainerStyle().setPadding(20).setBackgroundColor(Colors::DarkGray)
);

container->add(Column({
    Text(Point(0, 0), "Inside Container", 2, Colors::White),
    Button(ButtonConfig(0, 0, 100, 40, "Click"))
}));
```

### Custom Layout Widgets

For specialized layouts, you can create custom layout widgets:

```cpp
class GridLayout : public Widget {
public:
    GridLayout(int columns, int spacing = 10) 
        : columns_(columns), spacing_(spacing) {}
    
    void addChild(std::shared_ptr<Widget> child) {
        children_.push_back(child);
        arrangeChildren();
    }
    
private:
    void arrangeChildren() {
        // Calculate grid positions
        for (int i = 0; i < children_.size(); ++i) {
            int row = i / columns_;
            int col = i % columns_;
            
            int x = col * (childWidth_ + spacing_);
            int y = row * (childHeight_ + spacing_);
            
            children_[i]->setPosition(Point(x, y));
        }
    }
    
    std::vector<std::shared_ptr<Widget>> children_;
    int columns_;
    int spacing_;
    int childWidth_ = 100;
    int childHeight_ = 100;
};
```

Understanding layouts deeply will transform how you build user interfaces. Instead of fighting with pixel calculations, you'll describe relationships and let Fern handle the complexity. This leads to more maintainable code, better responsiveness, and interfaces that adapt gracefully to different screen sizes and content changes.
