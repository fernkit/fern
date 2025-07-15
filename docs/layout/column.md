# Column Layout

The `ColumnWidget` arranges child widgets vertically in a column, providing flexible control over spacing, alignment, and sizing. It's one of the core layout widgets in Fern UI.

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

### Creating a Column

```cpp
#include <fern/fern.hpp>
using namespace Fern;

// Basic column with default alignment
std::vector<std::shared_ptr<Widget>> children = {
    Text(Point(0, 0), "Item 1", 2, Colors::White),
    Text(Point(0, 0), "Item 2", 2, Colors::White),
    Text(Point(0, 0), "Item 3", 2, Colors::White)
};

auto column = Column(children);
```

### Using ColumnWidget Class

```cpp
// Create column with explicit dimensions
auto column = std::make_shared<ColumnWidget>(
    100, 100, 300, 400,  // x, y, width, height
    MainAxisAlignment::Center,
    CrossAxisAlignment::Center
);

// Add children one by one
column->add(Text(Point(0, 0), "First Item", 2, Colors::White));
column->add(Text(Point(0, 0), "Second Item", 2, Colors::White));

// Or add multiple at once
std::vector<std::shared_ptr<Widget>> moreChildren = {
    Text(Point(0, 0), "Third Item", 2, Colors::White),
    Text(Point(0, 0), "Fourth Item", 2, Colors::White)
};
column->addAll(moreChildren);
```

### Helper Function

```cpp
// Convenient Column() function
auto column = Column(
    {
        Text(Point(0, 0), "Header", 3, Colors::Yellow),
        Text(Point(0, 0), "Content", 2, Colors::White),
        Button(ButtonConfig(0, 0, 200, 40).text("Click Me"))
    },
    false,  // addToManager
    MainAxisAlignment::Center,
    CrossAxisAlignment::Center
);
```

## Alignment

### Main Axis Alignment (Vertical)

Controls how children are distributed along the column's height:

```cpp
// Start - children aligned to top
auto column = Column(children, false, MainAxisAlignment::Start);

// Center - children centered vertically
auto column = Column(children, false, MainAxisAlignment::Center);

// End - children aligned to bottom
auto column = Column(children, false, MainAxisAlignment::End);

// SpaceBetween - equal space between children
auto column = Column(children, false, MainAxisAlignment::SpaceBetween);

// SpaceAround - equal space around children
auto column = Column(children, false, MainAxisAlignment::SpaceAround);

// SpaceEvenly - equal space everywhere
auto column = Column(children, false, MainAxisAlignment::SpaceEvenly);
```

### Cross Axis Alignment (Horizontal)

Controls how children are aligned horizontally within the column:

```cpp
// Start - children aligned to left
auto column = Column(children, false, 
    MainAxisAlignment::Start, CrossAxisAlignment::Start);

// Center - children centered horizontally
auto column = Column(children, false,
    MainAxisAlignment::Start, CrossAxisAlignment::Center);

// End - children aligned to right
auto column = Column(children, false,
    MainAxisAlignment::Start, CrossAxisAlignment::End);

// Stretch - children stretched to full width
auto column = Column(children, false,
    MainAxisAlignment::Start, CrossAxisAlignment::Stretch);
```

### Dynamic Alignment Changes

```cpp
auto column = std::make_shared<ColumnWidget>(0, 0, 400, 600);

// Change alignment at runtime
column->setMainAxisAlignment(MainAxisAlignment::Center);
column->setCrossAxisAlignment(CrossAxisAlignment::Stretch);
```

## Flexible Sizing

### Using Expanded Widgets

```cpp
auto column = Column({
    Text(Point(0, 0), "Fixed Header", 3, Colors::Yellow),
    
    // This will expand to fill available space
    Expanded(
        Text(Point(0, 0), "Flexible Content", 2, Colors::White),
        1  // flex factor
    ),
    
    // This will take twice as much space
    Expanded(
        Text(Point(0, 0), "More Content", 2, Colors::Green),
        2  // flex factor
    ),
    
    Button(ButtonConfig(0, 0, 200, 40).text("Fixed Footer"))
});
```

### Flex Factors

```cpp
// Different flex ratios
auto column = Column({
    Expanded(widget1, 1),  // Takes 1/6 of available space
    Expanded(widget2, 2),  // Takes 2/6 of available space
    Expanded(widget3, 3)   // Takes 3/6 of available space
});
```

## Spacing

### Using SizedBox for Spacing

```cpp
auto column = Column({
    Text(Point(0, 0), "First Item", 2, Colors::White),
    SizedBox(0, 20),  // 20px vertical spacing
    Text(Point(0, 0), "Second Item", 2, Colors::White),
    SizedBox(0, 30),  // 30px vertical spacing
    Text(Point(0, 0), "Third Item", 2, Colors::White)
});
```

### Even Spacing with Alignment

```cpp
// Automatic spacing with SpaceEvenly
auto column = Column(
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

### Nested Columns

```cpp
auto innerColumn = Column({
    Text(Point(0, 0), "Nested Item 1", 2, Colors::White),
    Text(Point(0, 0), "Nested Item 2", 2, Colors::White)
});

auto outerColumn = Column({
    Text(Point(0, 0), "Main Header", 3, Colors::Yellow),
    innerColumn,
    Text(Point(0, 0), "Main Footer", 2, Colors::Gray)
});
```

### Mixed Layout Types

```cpp
auto column = Column({
    Text(Point(0, 0), "Title", 3, Colors::Yellow),
    
    // Horizontal row inside vertical column
    Row({
        Button(ButtonConfig(0, 0, 100, 40).text("Button 1")),
        SizedBox(20, 0),  // Horizontal spacing
        Button(ButtonConfig(0, 0, 100, 40).text("Button 2"))
    }),
    
    SizedBox(0, 20),
    Text(Point(0, 0), "Description", 2, Colors::White)
});
```

### Dynamic Content

```cpp
auto column = std::make_shared<ColumnWidget>(0, 0, 400, 600);

// Add items dynamically
void addItem(const std::string& text) {
    auto item = Text(Point(0, 0), text, 2, Colors::White);
    column->add(item);
}

// Add multiple items
void addMultipleItems(const std::vector<std::string>& items) {
    std::vector<std::shared_ptr<Widget>> widgets;
    for (const auto& item : items) {
        widgets.push_back(Text(Point(0, 0), item, 2, Colors::White));
    }
    column->addAll(widgets);
}
```

## Examples

### Simple Menu

```cpp
void createMenu() {
    auto menuItems = Column({
        Text(Point(0, 0), "MAIN MENU", 3, Colors::Yellow),
        SizedBox(0, 30),
        
        Button(ButtonConfig(0, 0, 200, 40).text("Start Game")),
        SizedBox(0, 10),
        
        Button(ButtonConfig(0, 0, 200, 40).text("Settings")),
        SizedBox(0, 10),
        
        Button(ButtonConfig(0, 0, 200, 40).text("Exit")),
    }, false, MainAxisAlignment::Center);
    
    // Center the menu on screen
    auto centerWidget = std::make_shared<CenterWidget>(
        0, 0, Fern::getWidth(), Fern::getHeight()
    );
    centerWidget->add(menuItems);
    
    addWidget(centerWidget);
}
```

### Settings Panel

```cpp
void createSettingsPanel() {
    auto settings = Column({
        Text(Point(0, 0), "Settings", 3, Colors::White),
        SizedBox(0, 30),
        
        // Volume setting
        Row({
            Text(Point(0, 0), "Volume:", 2, Colors::White),
            SizedBox(20, 0),
            // Slider would go here
        }),
        SizedBox(0, 20),
        
        // Graphics setting
        Row({
            Text(Point(0, 0), "Graphics:", 2, Colors::White),
            SizedBox(20, 0),
            // Dropdown would go here
        }),
        SizedBox(0, 20),
        
        // Buttons
        Row({
            Button(ButtonConfig(0, 0, 100, 40).text("Save")),
            SizedBox(20, 0),
            Button(ButtonConfig(0, 0, 100, 40).text("Cancel"))
        }, false, MainAxisAlignment::Center)
    });
    
    addWidget(settings);
}
```

### Form Layout

```cpp
void createForm() {
    auto form = Column({
        Text(Point(0, 0), "Contact Form", 3, Colors::White),
        SizedBox(0, 30),
        
        // Name field
        Column({
            Text(Point(0, 0), "Name:", 2, Colors::White),
            SizedBox(0, 5),
            TextInput(TextInputConfig(0, 0, 300, 40).placeholder("Enter your name"))
        }, false, MainAxisAlignment::Start, CrossAxisAlignment::Start),
        
        SizedBox(0, 20),
        
        // Email field
        Column({
            Text(Point(0, 0), "Email:", 2, Colors::White),
            SizedBox(0, 5),
            TextInput(TextInputConfig(0, 0, 300, 40).placeholder("Enter your email"))
        }, false, MainAxisAlignment::Start, CrossAxisAlignment::Start),
        
        SizedBox(0, 20),
        
        // Message field
        Column({
            Text(Point(0, 0), "Message:", 2, Colors::White),
            SizedBox(0, 5),
            TextInput(TextInputConfig(0, 0, 300, 100).placeholder("Enter your message"))
        }, false, MainAxisAlignment::Start, CrossAxisAlignment::Start),
        
        SizedBox(0, 30),
        
        // Submit button
        Button(ButtonConfig(0, 0, 200, 40).text("Submit"))
    });
    
    addWidget(form);
}
```

### Responsive Layout

```cpp
void createResponsiveLayout() {
    int screenWidth = Fern::getWidth();
    int screenHeight = Fern::getHeight();
    
    auto layout = Column({
        // Header - fixed size
        Text(Point(0, 0), "Header", 3, Colors::Yellow),
        
        // Content - flexible size
        Expanded(
            Text(Point(0, 0), "Main Content Area", 2, Colors::White),
            1
        ),
        
        // Footer - fixed size
        Text(Point(0, 0), "Footer", 2, Colors::Gray)
    });
    
    // Make it fill the screen
    auto container = std::make_shared<ColumnWidget>(
        0, 0, screenWidth, screenHeight
    );
    container->add(layout);
    
    addWidget(container);
}
```

### Scrollable List

```cpp
void createScrollableList() {
    std::vector<std::shared_ptr<Widget>> items;
    
    // Generate list items
    for (int i = 1; i <= 20; ++i) {
        items.push_back(
            Row({
                Text(Point(0, 0), "Item " + std::to_string(i), 2, Colors::White),
                SizedBox(20, 0),
                Button(ButtonConfig(0, 0, 80, 30).text("Action"))
            })
        );
        
        if (i < 20) {
            items.push_back(SizedBox(0, 10));  // Spacing between items
        }
    }
    
    auto listColumn = Column(items);
    
    // In a real implementation, you'd wrap this in a scrollable container
    addWidget(listColumn);
}
```

## Best Practices

### 1. Choose Appropriate Alignment

```cpp
// For menus and forms - center alignment
auto menu = Column(items, false, MainAxisAlignment::Center);

// For lists - start alignment
auto list = Column(items, false, MainAxisAlignment::Start);

// For even distribution - space evenly
auto toolbar = Column(items, false, MainAxisAlignment::SpaceEvenly);
```

### 2. Use Flexible Sizing Wisely

```cpp
// Good: Mix fixed and flexible elements
auto layout = Column({
    Text(Point(0, 0), "Fixed Header", 3, Colors::White),      // Fixed
    Expanded(contentWidget, 1),                                // Flexible
    Button(ButtonConfig(0, 0, 200, 40).text("Fixed Footer"))  // Fixed
});

// Avoid: All expanded widgets without fixed elements
// This can make layout unpredictable
```

### 3. Consistent Spacing

```cpp
// Define consistent spacing constants
const int SMALL_SPACING = 10;
const int MEDIUM_SPACING = 20;
const int LARGE_SPACING = 30;

auto column = Column({
    Text(Point(0, 0), "Title", 3, Colors::White),
    SizedBox(0, LARGE_SPACING),
    Text(Point(0, 0), "Subtitle", 2, Colors::Gray),
    SizedBox(0, MEDIUM_SPACING),
    Text(Point(0, 0), "Content", 2, Colors::White)
});
```

### 4. Responsive Design

```cpp
// Adapt to screen size
int screenWidth = Fern::getWidth();
int screenHeight = Fern::getHeight();

int columnWidth = std::min(400, screenWidth - 40);  // Max 400px, min 40px margin
auto column = std::make_shared<ColumnWidget>(
    (screenWidth - columnWidth) / 2,  // Center horizontally
    20,  // Top margin
    columnWidth,
    screenHeight - 40  // Leave bottom margin
);
```

### 5. Performance Considerations

```cpp
// Pre-create widgets when possible
std::vector<std::shared_ptr<Widget>> preCreatedItems;
for (int i = 0; i < 100; ++i) {
    preCreatedItems.push_back(
        Text(Point(0, 0), "Item " + std::to_string(i), 2, Colors::White)
    );
}

// Use addAll for multiple items
column->addAll(preCreatedItems);  // More efficient than multiple add() calls
```

## Troubleshooting

### Common Issues

1. **Children Not Visible**
   ```cpp
   // Ensure column has sufficient height
   auto column = std::make_shared<ColumnWidget>(0, 0, 300, 400);  // Adequate height
   
   // Check if children have proper dimensions
   auto text = Text(Point(0, 0), "Visible Text", 2, Colors::White);
   ```

2. **Alignment Not Working**
   ```cpp
   // Ensure column has dimensions larger than children
   auto column = std::make_shared<ColumnWidget>(0, 0, 400, 600);  // Large enough
   
   // Check alignment settings
   column->setMainAxisAlignment(MainAxisAlignment::Center);
   column->setCrossAxisAlignment(CrossAxisAlignment::Center);
   ```

3. **Expanded Widgets Not Expanding**
   ```cpp
   // Ensure there's available space to expand into
   auto column = std::make_shared<ColumnWidget>(0, 0, 300, 500);  // Fixed height
   
   // Check that other widgets don't consume all space
   auto layout = Column({
       Text(Point(0, 0), "Small Header", 2, Colors::White),  // Small fixed size
       Expanded(contentWidget, 1)  // Now has space to expand
   });
   ```

4. **Spacing Issues**
   ```cpp
   // Use SizedBox for consistent spacing
   SizedBox(0, 20);  // 20px vertical spacing
   
   // Or use alignment for automatic spacing
   MainAxisAlignment::SpaceEvenly  // Automatic even spacing
   ```

### Performance Issues

1. **Too Many Children**
   ```cpp
   // Consider virtualization for large lists
   // Or use pagination
   const int ITEMS_PER_PAGE = 50;
   
   // Only create visible items
   std::vector<std::shared_ptr<Widget>> visibleItems;
   for (int i = startIndex; i < startIndex + ITEMS_PER_PAGE; ++i) {
       visibleItems.push_back(createItem(i));
   }
   ```

2. **Frequent Layout Changes**
   ```cpp
   // Batch updates when possible
   std::vector<std::shared_ptr<Widget>> newItems;
   // ... populate newItems ...
   
   column->addAll(newItems);  // Single layout update
   ```

### Layout Debugging

```cpp
// Debug column dimensions
#ifdef DEBUG
    std::cout << "Column: " << column->getX() << ", " << column->getY() 
              << ", " << column->getWidth() << "x" << column->getHeight() << std::endl;
    
    // Debug child positions after layout
    for (const auto& child : column->getChildren()) {
        std::cout << "Child: " << child->getX() << ", " << child->getY() 
                  << ", " << child->getWidth() << "x" << child->getHeight() << std::endl;
    }
#endif
```

## Related Documentation

- [Row Layout](row.md) - For horizontal layouts
- [Layout System Overview](overview.md) - For general layout concepts
- [Expanded Layout](expanded.md) - For flexible sizing
- [Padding Layout](padding.md) - For adding padding around widgets
- [Spacing Layout](spacing.md) - For spacing between widgets

---

*This documentation covers the Fern Column Layout system. For more examples and advanced usage, see the examples directory.*
