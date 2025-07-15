# Row Layout

The `RowWidget` arranges child widgets horizontally in a row, providing flexible control over spacing, alignment, and sizing. It's the horizontal counterpart to the Column layout.

## Table of Contents
- [Basic Usage](#basic-usage)
- [Alignment](#alignment)
- [Flexible Sizing](#flexible-sizing)
- [Spacing](#spacing)
- [Advanced Features](#advanced-features)
- [Examples](#examples)
- [Best Practices](#best-practices)
- [Troubleshooting](#troubleshooting)

## Basic Usage

### Creating a Row

```cpp
#include <fern/fern.hpp>
using namespace Fern;

// Basic row with default alignment
std::vector<std::shared_ptr<Widget>> children = {
    Text(Point(0, 0), "Item 1", 2, Colors::White),
    Text(Point(0, 0), "Item 2", 2, Colors::White),
    Text(Point(0, 0), "Item 3", 2, Colors::White)
};

auto row = Row(children);
```

### Using RowWidget Class

```cpp
// Create row with explicit dimensions
auto row = std::make_shared<RowWidget>(
    100, 100, 600, 50,  // x, y, width, height
    MainAxisAlignment::Center,
    CrossAxisAlignment::Center
);

// Add children one by one
row->add(Text(Point(0, 0), "First Item", 2, Colors::White));
row->add(Text(Point(0, 0), "Second Item", 2, Colors::White));

// Or add multiple at once
std::vector<std::shared_ptr<Widget>> moreChildren = {
    Text(Point(0, 0), "Third Item", 2, Colors::White),
    Text(Point(0, 0), "Fourth Item", 2, Colors::White)
};
row->addAll(moreChildren);
```

### Helper Function

```cpp
// Convenient Row() function
auto row = Row(
    {
        Button(ButtonConfig(0, 0, 100, 40).text("Previous")),
        Text(Point(0, 0), "Page 1 of 10", 2, Colors::White),
        Button(ButtonConfig(0, 0, 100, 40).text("Next"))
    },
    false,  // addToManager
    MainAxisAlignment::SpaceBetween,
    CrossAxisAlignment::Center
);
```

## Alignment

### Main Axis Alignment (Horizontal)

Controls how children are distributed along the row's width:

```cpp
// Start - children aligned to left
auto row = Row(children, false, MainAxisAlignment::Start);

// Center - children centered horizontally
auto row = Row(children, false, MainAxisAlignment::Center);

// End - children aligned to right
auto row = Row(children, false, MainAxisAlignment::End);

// SpaceBetween - equal space between children
auto row = Row(children, false, MainAxisAlignment::SpaceBetween);

// SpaceAround - equal space around children
auto row = Row(children, false, MainAxisAlignment::SpaceAround);

// SpaceEvenly - equal space everywhere
auto row = Row(children, false, MainAxisAlignment::SpaceEvenly);
```

### Cross Axis Alignment (Vertical)

Controls how children are aligned vertically within the row:

```cpp
// Start - children aligned to top
auto row = Row(children, false, 
    MainAxisAlignment::Start, CrossAxisAlignment::Start);

// Center - children centered vertically
auto row = Row(children, false,
    MainAxisAlignment::Start, CrossAxisAlignment::Center);

// End - children aligned to bottom
auto row = Row(children, false,
    MainAxisAlignment::Start, CrossAxisAlignment::End);

// Stretch - children stretched to full height
auto row = Row(children, false,
    MainAxisAlignment::Start, CrossAxisAlignment::Stretch);
```

### Dynamic Alignment Changes

```cpp
auto row = std::make_shared<RowWidget>(0, 0, 600, 100);

// Change alignment at runtime
row->setMainAxisAlignment(MainAxisAlignment::SpaceBetween);
row->setCrossAxisAlignment(CrossAxisAlignment::Center);
```

## Flexible Sizing

### Using Expanded Widgets

```cpp
auto row = Row({
    Text(Point(0, 0), "Fixed", 2, Colors::White),
    
    // This will expand to fill available space
    Expanded(
        Text(Point(0, 0), "Flexible Content", 2, Colors::Green),
        1  // flex factor
    ),
    
    // This will take twice as much space
    Expanded(
        Text(Point(0, 0), "More Space", 2, Colors::Blue),
        2  // flex factor
    ),
    
    Button(ButtonConfig(0, 0, 80, 40).text("Fixed"))
});
```

### Flex Factors

```cpp
// Different flex ratios
auto row = Row({
    Expanded(widget1, 1),  // Takes 1/6 of available space
    Expanded(widget2, 2),  // Takes 2/6 of available space
    Expanded(widget3, 3)   // Takes 3/6 of available space
});
```

## Spacing

### Using SizedBox for Spacing

```cpp
auto row = Row({
    Text(Point(0, 0), "First", 2, Colors::White),
    SizedBox(20, 0),  // 20px horizontal spacing
    Text(Point(0, 0), "Second", 2, Colors::White),
    SizedBox(30, 0),  // 30px horizontal spacing
    Text(Point(0, 0), "Third", 2, Colors::White)
});
```

### Even Spacing with Alignment

```cpp
// Automatic spacing with SpaceEvenly
auto row = Row(
    {
        Text(Point(0, 0), "Item 1", 2, Colors::White),
        Text(Point(0, 0), "Item 2", 2, Colors::White),
        Text(Point(0, 0), "Item 3", 2, Colors::White)
    },
    false,
    MainAxisAlignment::SpaceEvenly
);
```

## Advanced Features

### Nested Rows

```cpp
auto innerRow = Row({
    Text(Point(0, 0), "Inner 1", 2, Colors::White),
    Text(Point(0, 0), "Inner 2", 2, Colors::White)
});

auto outerRow = Row({
    Text(Point(0, 0), "Left", 2, Colors::Yellow),
    innerRow,
    Text(Point(0, 0), "Right", 2, Colors::Yellow)
});
```

### Mixed Layout Types

```cpp
auto row = Row({
    // Vertical column inside horizontal row
    Column({
        Text(Point(0, 0), "Line 1", 2, Colors::White),
        Text(Point(0, 0), "Line 2", 2, Colors::White)
    }),
    
    SizedBox(20, 0),
    
    Button(ButtonConfig(0, 0, 100, 40).text("Action")),
    
    SizedBox(20, 0),
    
    Column({
        Text(Point(0, 0), "Info 1", 2, Colors::Gray),
        Text(Point(0, 0), "Info 2", 2, Colors::Gray)
    })
});
```

### Dynamic Content

```cpp
auto row = std::make_shared<RowWidget>(0, 0, 800, 60);

// Add items dynamically
void addItem(const std::string& text) {
    auto item = Text(Point(0, 0), text, 2, Colors::White);
    row->add(item);
}

// Add multiple items
void addMultipleItems(const std::vector<std::string>& items) {
    std::vector<std::shared_ptr<Widget>> widgets;
    for (const auto& item : items) {
        widgets.push_back(Text(Point(0, 0), item, 2, Colors::White));
    }
    row->addAll(widgets);
}
```

## Examples

### Navigation Bar

```cpp
void createNavigationBar() {
    auto navbar = Row({
        Text(Point(0, 0), "MyApp", 3, Colors::Yellow),
        
        // Flexible space to push buttons to right
        Expanded(SizedBox(0, 0), 1),
        
        Button(ButtonConfig(0, 0, 80, 35).text("Home")),
        SizedBox(10, 0),
        
        Button(ButtonConfig(0, 0, 80, 35).text("About")),
        SizedBox(10, 0),
        
        Button(ButtonConfig(0, 0, 80, 35).text("Contact"))
    });
    
    // Position at top of screen
    auto navContainer = std::make_shared<RowWidget>(
        0, 0, Fern::getWidth(), 50
    );
    navContainer->add(navbar);
    
    addWidget(navContainer);
}
```

### Toolbar

```cpp
void createToolbar() {
    auto toolbar = Row({
        Button(ButtonConfig(0, 0, 40, 40).text("📁")),  // Open
        SizedBox(5, 0),
        
        Button(ButtonConfig(0, 0, 40, 40).text("💾")),  // Save
        SizedBox(5, 0),
        
        Button(ButtonConfig(0, 0, 40, 40).text("✂️")),  // Cut
        SizedBox(5, 0),
        
        Button(ButtonConfig(0, 0, 40, 40).text("📋")),  // Copy
        SizedBox(5, 0),
        
        Button(ButtonConfig(0, 0, 40, 40).text("📄")),  // Paste
        
        // Spacer
        SizedBox(20, 0),
        
        Button(ButtonConfig(0, 0, 40, 40).text("↶")),  // Undo
        SizedBox(5, 0),
        
        Button(ButtonConfig(0, 0, 40, 40).text("↷"))   // Redo
    }, false, MainAxisAlignment::Start);
    
    addWidget(toolbar);
}
```

### Status Bar

```cpp
void createStatusBar() {
    auto statusBar = Row({
        Text(Point(0, 0), "Ready", 2, Colors::Green),
        
        // Flexible space
        Expanded(SizedBox(0, 0), 1),
        
        Text(Point(0, 0), "Line 1, Col 1", 2, Colors::White),
        SizedBox(20, 0),
        
        Text(Point(0, 0), "100%", 2, Colors::White),
        SizedBox(20, 0),
        
        Text(Point(0, 0), "UTF-8", 2, Colors::White)
    });
    
    // Position at bottom of screen
    auto statusContainer = std::make_shared<RowWidget>(
        0, Fern::getHeight() - 30, Fern::getWidth(), 30
    );
    statusContainer->add(statusBar);
    
    addWidget(statusContainer);
}
```

### Card Layout

```cpp
void createCard() {
    auto card = Column({
        // Card header
        Row({
            Text(Point(0, 0), "Card Title", 3, Colors::White),
            Expanded(SizedBox(0, 0), 1),
            Button(ButtonConfig(0, 0, 20, 20).text("×"))
        }),
        
        SizedBox(0, 15),
        
        // Card content
        Text(Point(0, 0), "This is the card content area.", 2, Colors::Gray),
        
        SizedBox(0, 20),
        
        // Card actions
        Row({
            Expanded(SizedBox(0, 0), 1),
            Button(ButtonConfig(0, 0, 80, 35).text("Cancel")),
            SizedBox(10, 0),
            Button(ButtonConfig(0, 0, 80, 35).text("OK"))
        })
    });
    
    addWidget(card);
}
```

### Form Row

```cpp
void createFormRow() {
    auto formRow = Row({
        // Label
        Text(Point(0, 0), "Name:", 2, Colors::White),
        SizedBox(10, 0),
        
        // Input field
        Expanded(
            TextInput(TextInputConfig(0, 0, 200, 35).placeholder("Enter name")),
            1
        ),
        
        SizedBox(10, 0),
        
        // Validation icon/button
        Button(ButtonConfig(0, 0, 30, 35).text("✓"))
    });
    
    addWidget(formRow);
}
```

### Media Player Controls

```cpp
void createMediaControls() {
    auto controls = Row({
        Button(ButtonConfig(0, 0, 40, 40).text("⏮")),   // Previous
        SizedBox(5, 0),
        
        Button(ButtonConfig(0, 0, 40, 40).text("⏯")),   // Play/Pause
        SizedBox(5, 0),
        
        Button(ButtonConfig(0, 0, 40, 40).text("⏭")),   // Next
        SizedBox(20, 0),
        
        // Progress bar (would be a slider in real implementation)
        Expanded(
            Text(Point(0, 0), "─────●─────", 2, Colors::White),
            1
        ),
        
        SizedBox(20, 0),
        
        Text(Point(0, 0), "2:30", 2, Colors::White),
        SizedBox(5, 0),
        
        Button(ButtonConfig(0, 0, 40, 40).text("🔊"))   // Volume
    }, false, MainAxisAlignment::Start, CrossAxisAlignment::Center);
    
    addWidget(controls);
}
```

### Data Table Row

```cpp
void createTableRow(const std::string& name, const std::string& email, int age) {
    auto tableRow = Row({
        // Name column
        Text(Point(0, 0), name, 2, Colors::White),
        SizedBox(20, 0),
        
        // Email column
        Expanded(
            Text(Point(0, 0), email, 2, Colors::Gray),
            1
        ),
        
        SizedBox(20, 0),
        
        // Age column
        Text(Point(0, 0), std::to_string(age), 2, Colors::White),
        SizedBox(20, 0),
        
        // Actions column
        Row({
            Button(ButtonConfig(0, 0, 60, 30).text("Edit")),
            SizedBox(5, 0),
            Button(ButtonConfig(0, 0, 60, 30).text("Delete"))
        })
    });
    
    addWidget(tableRow);
}
```

## Best Practices

### 1. Choose Appropriate Alignment

```cpp
// For toolbars and navigation - start alignment
auto toolbar = Row(items, false, MainAxisAlignment::Start);

// For centering content - center alignment
auto centeredRow = Row(items, false, MainAxisAlignment::Center);

// For space distribution - space between
auto statusBar = Row(items, false, MainAxisAlignment::SpaceBetween);
```

### 2. Use Flexible Sizing Wisely

```cpp
// Good: Mix fixed and flexible elements
auto row = Row({
    Text(Point(0, 0), "Fixed Label", 2, Colors::White),    // Fixed width
    Expanded(inputWidget, 1),                              // Flexible width
    Button(ButtonConfig(0, 0, 80, 40).text("Submit"))     // Fixed width
});

// Avoid: All expanded widgets can make layout unpredictable
```

### 3. Consistent Spacing

```cpp
// Define consistent spacing constants
const int SMALL_SPACING = 5;
const int MEDIUM_SPACING = 10;
const int LARGE_SPACING = 20;

auto row = Row({
    Text(Point(0, 0), "Item 1", 2, Colors::White),
    SizedBox(MEDIUM_SPACING, 0),
    Text(Point(0, 0), "Item 2", 2, Colors::White),
    SizedBox(MEDIUM_SPACING, 0),
    Text(Point(0, 0), "Item 3", 2, Colors::White)
});
```

### 4. Handle Different Screen Sizes

```cpp
// Responsive row that adapts to screen width
int screenWidth = Fern::getWidth();
int buttonWidth = std::max(80, screenWidth / 6);  // Responsive button width

auto row = Row({
    Button(ButtonConfig(0, 0, buttonWidth, 40).text("Button 1")),
    SizedBox(10, 0),
    Button(ButtonConfig(0, 0, buttonWidth, 40).text("Button 2")),
    SizedBox(10, 0),
    Button(ButtonConfig(0, 0, buttonWidth, 40).text("Button 3"))
});
```

### 5. Performance Considerations

```cpp
// Pre-create widgets when possible
std::vector<std::shared_ptr<Widget>> preCreatedItems;
for (int i = 0; i < 10; ++i) {
    preCreatedItems.push_back(
        Button(ButtonConfig(0, 0, 80, 40).text("Item " + std::to_string(i)))
    );
}

// Use addAll for multiple items
row->addAll(preCreatedItems);  // More efficient than multiple add() calls
```

## Troubleshooting

### Common Issues

1. **Children Not Visible**
   ```cpp
   // Ensure row has sufficient width
   auto row = std::make_shared<RowWidget>(0, 0, 600, 50);  // Adequate width
   
   // Check if children have proper dimensions
   auto text = Text(Point(0, 0), "Visible Text", 2, Colors::White);
   ```

2. **Alignment Not Working**
   ```cpp
   // Ensure row has dimensions larger than children
   auto row = std::make_shared<RowWidget>(0, 0, 800, 100);  // Large enough
   
   // Check alignment settings
   row->setMainAxisAlignment(MainAxisAlignment::Center);
   row->setCrossAxisAlignment(CrossAxisAlignment::Center);
   ```

3. **Expanded Widgets Not Expanding**
   ```cpp
   // Ensure there's available space to expand into
   auto row = std::make_shared<RowWidget>(0, 0, 600, 50);  // Fixed width
   
   // Check that other widgets don't consume all space
   auto layout = Row({
       Text(Point(0, 0), "Small", 2, Colors::White),     // Small fixed size
       Expanded(contentWidget, 1)                         // Now has space to expand
   });
   ```

4. **Spacing Issues**
   ```cpp
   // Use SizedBox for consistent spacing
   SizedBox(20, 0);  // 20px horizontal spacing
   
   // Or use alignment for automatic spacing
   MainAxisAlignment::SpaceEvenly  // Automatic even spacing
   ```

### Performance Issues

1. **Too Many Children**
   ```cpp
   // Consider horizontal scrolling for wide content
   // Or use pagination for large datasets
   const int MAX_VISIBLE_ITEMS = 10;
   
   // Only show visible items
   std::vector<std::shared_ptr<Widget>> visibleItems;
   for (int i = startIndex; i < startIndex + MAX_VISIBLE_ITEMS; ++i) {
       visibleItems.push_back(createItem(i));
   }
   ```

2. **Frequent Layout Changes**
   ```cpp
   // Batch updates when possible
   std::vector<std::shared_ptr<Widget>> newItems;
   // ... populate newItems ...
   
   row->addAll(newItems);  // Single layout update
   ```

### Layout Debugging

```cpp
// Debug row dimensions
#ifdef DEBUG
    std::cout << "Row: " << row->getX() << ", " << row->getY() 
              << ", " << row->getWidth() << "x" << row->getHeight() << std::endl;
    
    // Debug child positions after layout
    for (const auto& child : row->getChildren()) {
        std::cout << "Child: " << child->getX() << ", " << child->getY() 
                  << ", " << child->getWidth() << "x" << child->getHeight() << std::endl;
    }
#endif
```

## Related Documentation

- [Column Layout](column.md) - For vertical layouts
- [Layout System Overview](overview.md) - For general layout concepts
- [Expanded Layout](expanded.md) - For flexible sizing
- [Padding Layout](padding.md) - For adding padding around widgets
- [Spacing Layout](spacing.md) - For spacing between widgets

---

*This documentation covers the Fern Row Layout system. For more examples and advanced usage, see the examples directory.*
