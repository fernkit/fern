# Expanded Layout

The `ExpandedWidget` makes a child widget expand to fill available space within a `Row` or `Column`. It provides flexible sizing with customizable flex factors to control how much space each expanded widget should take relative to others.

## Table of Contents
- [Basic Usage](#basic-usage)
- [Flex Factors](#flex-factors)
- [Usage in Layouts](#usage-in-layouts)
- [Responsive Design](#responsive-design)
- [Examples](#examples)
- [Best Practices](#best-practices)
- [Troubleshooting](#troubleshooting)

## Basic Usage

### Creating an Expanded Widget

```cpp
#include <fern/fern.hpp>
using namespace Fern;

// Basic expanded widget with flex factor 1
auto expandedWidget = Expanded(
    Text(Point(0, 0), "I will expand!", 2, Colors::White),
    1  // flex factor (optional, defaults to 1)
);

// Using in a column
auto column = Column({
    Text(Point(0, 0), "Fixed Header", 3, Colors::Yellow),
    Expanded(
        Text(Point(0, 0), "Flexible Content", 2, Colors::White),
        1
    ),
    Text(Point(0, 0), "Fixed Footer", 2, Colors::Gray)
});
```

### Using ExpandedWidget Class

```cpp
// Create with constructor
auto expandedWidget = std::make_shared<ExpandedWidget>(
    Text(Point(0, 0), "Expanded Content", 2, Colors::White),
    2  // flex factor
);

// Get flex factor
int flex = expandedWidget->getFlex();  // Returns 2
```

## Flex Factors

### Understanding Flex Factors

Flex factors determine how much of the available space each expanded widget should take:

```cpp
// Available space is divided proportionally based on flex factors
auto row = Row({
    Expanded(widget1, 1),  // Takes 1/4 of available space
    Expanded(widget2, 2),  // Takes 2/4 of available space  
    Expanded(widget3, 1)   // Takes 1/4 of available space
});
// Total flex = 1 + 2 + 1 = 4
```

### Flex Factor Examples

```cpp
// Equal distribution (all flex = 1)
auto equalRow = Row({
    Expanded(Text(Point(0, 0), "Equal 1", 2, Colors::White), 1),
    Expanded(Text(Point(0, 0), "Equal 2", 2, Colors::White), 1),
    Expanded(Text(Point(0, 0), "Equal 3", 2, Colors::White), 1)
});
// Each widget takes 1/3 of available space

// Proportional distribution
auto proportionalRow = Row({
    Expanded(Text(Point(0, 0), "Small", 2, Colors::White), 1),    // 1/6
    Expanded(Text(Point(0, 0), "Medium", 2, Colors::White), 2),   // 2/6  
    Expanded(Text(Point(0, 0), "Large", 2, Colors::White), 3)     // 3/6
});

// One widget takes most space
auto dominantRow = Row({
    Text(Point(0, 0), "Fixed", 2, Colors::White),                 // Fixed size
    Expanded(Text(Point(0, 0), "Flexible", 2, Colors::White), 1)  // Takes remaining space
});
```

## Usage in Layouts

### In Row Layouts

```cpp
// Horizontal expansion
auto row = Row({
    Text(Point(0, 0), "Left", 2, Colors::White),              // Fixed width
    Expanded(
        Text(Point(0, 0), "Center (Expanded)", 2, Colors::Green),
        1
    ),
    Text(Point(0, 0), "Right", 2, Colors::White)              // Fixed width
});
```

### In Column Layouts

```cpp
// Vertical expansion
auto column = Column({
    Text(Point(0, 0), "Top", 2, Colors::White),               // Fixed height
    Expanded(
        Text(Point(0, 0), "Middle (Expanded)", 2, Colors::Green),
        1
    ),
    Text(Point(0, 0), "Bottom", 2, Colors::White)             // Fixed height
});
```

### Mixed Fixed and Flexible

```cpp
auto layout = Column({
    Text(Point(0, 0), "Header", 3, Colors::Yellow),           // Fixed: ~30px
    SizedBox(0, 20),                                          // Fixed: 20px
    
    Expanded(
        Text(Point(0, 0), "Main Content Area", 2, Colors::White),
        3  // Takes 3/4 of remaining space
    ),
    
    Expanded(
        Text(Point(0, 0), "Secondary Content", 2, Colors::Gray),
        1  // Takes 1/4 of remaining space  
    ),
    
    SizedBox(0, 10),                                          // Fixed: 10px
    Text(Point(0, 0), "Footer", 2, Colors::Gray)             // Fixed: ~20px
});
```

## Responsive Design

### Adaptive Content

```cpp
void createResponsiveLayout() {
    int screenWidth = Fern::getWidth();
    int screenHeight = Fern::getHeight();
    
    // Create responsive column that fills screen
    auto responsiveColumn = Column({
        Text(Point(0, 0), "App Header", 3, Colors::White),
        SizedBox(0, 20),
        
        // Main content area expands to fill available space
        Expanded(
            Column({
                Text(Point(0, 0), "Main Content", 2, Colors::White),
                SizedBox(0, 20),
                
                // Nested responsive row
                Row({
                    Expanded(
                        Text(Point(0, 0), "Left Panel", 2, Colors::Green),
                        1
                    ),
                    SizedBox(20, 0),
                    Expanded(
                        Text(Point(0, 0), "Right Panel", 2, Colors::Blue),
                        2  // Twice as wide as left panel
                    )
                })
            }),
            1
        ),
        
        SizedBox(0, 20),
        Text(Point(0, 0), "Footer", 2, Colors::Gray)
    });
    
    // Make the column fill the screen
    auto screenContainer = std::make_shared<ColumnWidget>(
        0, 0, screenWidth, screenHeight
    );
    screenContainer->add(responsiveColumn);
    addWidget(screenContainer);
}
```

### Device Adaptation

```cpp
void createDeviceAdaptiveLayout() {
    int screenWidth = Fern::getWidth();
    
    // Adjust flex factors based on screen size
    int leftFlex = 1;
    int rightFlex = 2;
    
    if (screenWidth < 600) {
        // On small screens, make left panel smaller
        leftFlex = 1;
        rightFlex = 3;
    } else if (screenWidth > 1200) {
        // On large screens, more balanced
        leftFlex = 2;
        rightFlex = 3;
    }
    
    auto adaptiveRow = Row({
        Expanded(
            Text(Point(0, 0), "Sidebar", 2, Colors::Green),
            leftFlex
        ),
        SizedBox(20, 0),
        Expanded(
            Text(Point(0, 0), "Main Content", 2, Colors::White),
            rightFlex
        )
    });
}
```

## Examples

### App Layout

```cpp
void createAppLayout() {
    int screenWidth = Fern::getWidth();
    int screenHeight = Fern::getHeight();
    
    auto appLayout = Column({
        // Header (fixed height)
        Text(Point(0, 0), "My Application", 3, Colors::White),
        SizedBox(0, 10),
        
        // Main content area (expandable)
        Expanded(
            Row({
                // Sidebar (fixed width ratio)
                Expanded(
                    Column({
                        Text(Point(0, 0), "Navigation", 2, Colors::Yellow),
                        SizedBox(0, 20),
                        Button(ButtonConfig(0, 0, 120, 35).text("Home")),
                        SizedBox(0, 10),
                        Button(ButtonConfig(0, 0, 120, 35).text("Settings")),
                        SizedBox(0, 10),
                        Button(ButtonConfig(0, 0, 120, 35).text("About"))
                    }),
                    1  // Sidebar takes 1/4 of width
                ),
                
                SizedBox(20, 0),
                
                // Main content (flexible width)
                Expanded(
                    Column({
                        Text(Point(0, 0), "Main Content Area", 3, Colors::White),
                        SizedBox(0, 20),
                        Text(Point(0, 0), "This area expands to fill available space", 2, Colors::Gray),
                        SizedBox(0, 30),
                        
                        // Content that also expands
                        Expanded(
                            Text(Point(0, 0), "Dynamic Content", 2, Colors::Green),
                            1
                        )
                    }),
                    3  // Main content takes 3/4 of width
                )
            }),
            1  // Main area takes all available height
        ),
        
        SizedBox(0, 10),
        
        // Footer (fixed height)
        Text(Point(0, 0), "Status: Ready", 2, Colors::Gray)
    });
    
    // Make it fill the screen
    auto screenContainer = std::make_shared<ColumnWidget>(
        0, 0, screenWidth, screenHeight
    );
    screenContainer->add(appLayout);
    addWidget(screenContainer);
}
```

### Dashboard Layout

```cpp
void createDashboard() {
    auto dashboard = Column({
        // Dashboard header
        Text(Point(0, 0), "Dashboard", 4, Colors::White),
        SizedBox(0, 30),
        
        // Top row of cards
        Row({
            Expanded(
                Column({
                    Text(Point(0, 0), "Card 1", 2, Colors::White),
                    SizedBox(0, 10),
                    Text(Point(0, 0), "Data: 123", 2, Colors::Green)
                }),
                1
            ),
            SizedBox(20, 0),
            Expanded(
                Column({
                    Text(Point(0, 0), "Card 2", 2, Colors::White),
                    SizedBox(0, 10),
                    Text(Point(0, 0), "Data: 456", 2, Colors::Blue)
                }),
                1
            ),
            SizedBox(20, 0),
            Expanded(
                Column({
                    Text(Point(0, 0), "Card 3", 2, Colors::White),
                    SizedBox(0, 10),
                    Text(Point(0, 0), "Data: 789", 2, Colors::Red)
                }),
                1
            )
        }),
        
        SizedBox(0, 30),
        
        // Main content area
        Expanded(
            Row({
                // Chart area
                Expanded(
                    Column({
                        Text(Point(0, 0), "Chart Area", 3, Colors::White),
                        SizedBox(0, 20),
                        Text(Point(0, 0), "Chart content would go here", 2, Colors::Gray)
                    }),
                    2  // Chart takes 2/3 of width
                ),
                
                SizedBox(20, 0),
                
                // Sidebar info
                Expanded(
                    Column({
                        Text(Point(0, 0), "Info Panel", 3, Colors::White),
                        SizedBox(0, 20),
                        Text(Point(0, 0), "Additional info", 2, Colors::Gray),
                        SizedBox(0, 10),
                        Button(ButtonConfig(0, 0, 100, 35).text("Action"))
                    }),
                    1  // Info takes 1/3 of width
                )
            }),
            1  // Main area expands to fill available height
        )
    });
    
    addWidget(dashboard);
}
```

### Form Layout

```cpp
void createExpandableForm() {
    auto form = Column({
        Text(Point(0, 0), "Contact Form", 3, Colors::White),
        SizedBox(0, 30),
        
        // Form fields
        Row({
            Text(Point(0, 0), "Name:", 2, Colors::White),
            SizedBox(20, 0),
            Expanded(
                TextInput(TextInputConfig(0, 0, 200, 35).placeholder("Enter name")),
                1
            )
        }),
        SizedBox(0, 20),
        
        Row({
            Text(Point(0, 0), "Email:", 2, Colors::White),
            SizedBox(20, 0),
            Expanded(
                TextInput(TextInputConfig(0, 0, 200, 35).placeholder("Enter email")),
                1
            )
        }),
        SizedBox(0, 20),
        
        // Message field with more space
        Column({
            Text(Point(0, 0), "Message:", 2, Colors::White),
            SizedBox(0, 10),
            Expanded(
                TextInput(TextInputConfig(0, 0, 200, 100).placeholder("Enter message")),
                1  // Expands to fill available space
            )
        }),
        
        SizedBox(0, 30),
        
        // Submit button
        Row({
            Expanded(SizedBox(0, 0), 1),  // Spacer
            Button(ButtonConfig(0, 0, 150, 40).text("Submit")),
            Expanded(SizedBox(0, 0), 1)   // Spacer
        })
    });
    
    addWidget(form);
}
```

### Chat Interface

```cpp
void createChatInterface() {
    auto chatLayout = Column({
        // Chat header
        Text(Point(0, 0), "Chat Room", 3, Colors::White),
        SizedBox(0, 20),
        
        // Messages area (expandable)
        Expanded(
            Column({
                Text(Point(0, 0), "Alice: Hello everyone!", 2, Colors::Green),
                SizedBox(0, 5),
                Text(Point(0, 0), "Bob: Hi there!", 2, Colors::Blue),
                SizedBox(0, 5),
                Text(Point(0, 0), "Charlie: How's it going?", 2, Colors::Yellow),
                SizedBox(0, 5),
                
                // This area would expand to show more messages
                Expanded(
                    Text(Point(0, 0), "... more messages ...", 2, Colors::Gray),
                    1
                )
            }),
            1  // Messages area takes most space
        ),
        
        SizedBox(0, 20),
        
        // Input area (fixed height)
        Row({
            Expanded(
                TextInput(TextInputConfig(0, 0, 200, 35).placeholder("Type message...")),
                1  // Input expands to fill available width
            ),
            SizedBox(10, 0),
            Button(ButtonConfig(0, 0, 80, 35).text("Send"))
        })
    });
    
    addWidget(chatLayout);
}
```

### Media Player

```cpp
void createMediaPlayer() {
    auto player = Column({
        // Album art and info
        Row({
            Circle(60, Point(0, 0), Colors::Blue),  // Album art placeholder
            SizedBox(20, 0),
            Expanded(
                Column({
                    Text(Point(0, 0), "Song Title", 3, Colors::White),
                    SizedBox(0, 5),
                    Text(Point(0, 0), "Artist Name", 2, Colors::Gray),
                    SizedBox(0, 5),
                    Text(Point(0, 0), "Album Name", 2, Colors::Gray)
                }),
                1
            )
        }),
        
        SizedBox(0, 30),
        
        // Progress bar
        Row({
            Text(Point(0, 0), "2:30", 2, Colors::White),
            SizedBox(10, 0),
            Expanded(
                // Progress bar would go here
                Line(Point(0, 0), Point(300, 0), 4, Colors::Green),
                1
            ),
            SizedBox(10, 0),
            Text(Point(0, 0), "4:15", 2, Colors::White)
        }),
        
        SizedBox(0, 20),
        
        // Controls
        Row({
            Expanded(SizedBox(0, 0), 1),  // Spacer
            Button(ButtonConfig(0, 0, 40, 40).text("⏮")),
            SizedBox(10, 0),
            Button(ButtonConfig(0, 0, 40, 40).text("⏯")),
            SizedBox(10, 0),
            Button(ButtonConfig(0, 0, 40, 40).text("⏭")),
            Expanded(SizedBox(0, 0), 1)   // Spacer
        })
    });
    
    addWidget(player);
}
```

## Best Practices

### 1. Use Appropriate Flex Factors

```cpp
// Good: Meaningful ratios
auto layout = Row({
    Expanded(sidebar, 1),      // 1/4 of space
    Expanded(mainContent, 3)   // 3/4 of space
});

// Avoid: Overly complex ratios
auto complexLayout = Row({
    Expanded(widget1, 7),      // Hard to understand ratio
    Expanded(widget2, 13)      // Hard to understand ratio
});
```

### 2. Combine Fixed and Flexible Elements

```cpp
// Good: Mix of fixed and flexible
auto layout = Column({
    Text(Point(0, 0), "Header", 3, Colors::White),        // Fixed
    Expanded(contentArea, 1),                             // Flexible
    Text(Point(0, 0), "Footer", 2, Colors::Gray)         // Fixed
});

// Avoid: All expanded (can be unpredictable)
auto allExpanded = Column({
    Expanded(header, 1),
    Expanded(content, 3),
    Expanded(footer, 1)
});
```

### 3. Consider Content Requirements

```cpp
// Good: Flex factors based on content needs
auto layout = Row({
    Expanded(navigation, 1),   // Small navigation
    Expanded(content, 4),      // Large content area
    Expanded(sidebar, 1)       // Small sidebar
});

// Consider minimum sizes
auto responsiveLayout = Row({
    // Ensure minimum usable width
    Expanded(navigation, std::max(1, screenWidth / 200)),
    Expanded(content, 3)
});
```

### 4. Nested Expansion

```cpp
// Good: Logical nesting
auto layout = Column({
    Text(Point(0, 0), "Title", 3, Colors::White),
    Expanded(
        Row({
            Expanded(leftPanel, 1),
            Expanded(rightPanel, 2)
        }),
        1
    )
});
```

### 5. Performance Considerations

```cpp
// Pre-calculate flex factors when possible
int totalFlex = 0;
for (const auto& widget : widgets) {
    totalFlex += widget->getFlex();
}

// Use consistent flex factors
const int SIDEBAR_FLEX = 1;
const int CONTENT_FLEX = 3;
const int PANEL_FLEX = 2;
```

## Troubleshooting

### Common Issues

1. **Expanded Widget Not Expanding**
   ```cpp
   // Ensure parent layout supports expansion (Row/Column)
   auto column = Column({
       Expanded(widget, 1)  // This will work
   });
   
   // Expanded doesn't work outside Row/Column
   auto center = std::make_shared<CenterWidget>(0, 0, 400, 300);
   center->add(Expanded(widget, 1));  // Won't expand as expected
   ```

2. **Incorrect Proportions**
   ```cpp
   // Check flex factor calculations
   auto row = Row({
       Expanded(widget1, 2),  // 2/5 = 40%
       Expanded(widget2, 3)   // 3/5 = 60%
   });
   // Total flex = 2 + 3 = 5
   ```

3. **Content Overflow**
   ```cpp
   // Ensure expanded content can handle variable sizes
   auto expandedContent = Column({
       Text(Point(0, 0), "Flexible text content", 2, Colors::White),
       // Text should wrap or truncate appropriately
   });
   ```

4. **No Available Space to Expand**
   ```cpp
   // Ensure parent has dimensions
   auto parent = std::make_shared<ColumnWidget>(0, 0, 400, 300);  // Explicit size
   parent->add(Column({
       Expanded(widget, 1)  // Now has space to expand into
   }));
   ```

### Performance Issues

1. **Complex Nested Expansions**
   ```cpp
   // Avoid overly deep nesting
   // Consider flattening complex layouts
   ```

2. **Frequent Flex Changes**
   ```cpp
   // Cache expanded widgets when possible
   static std::shared_ptr<ExpandedWidget> cachedExpanded = nullptr;
   
   if (!cachedExpanded) {
       cachedExpanded = Expanded(content, 1);
   }
   ```

### Debugging

```cpp
// Debug expanded widget properties
#ifdef DEBUG
    auto expanded = Expanded(widget, 2);
    std::cout << "Expanded flex: " << expanded->getFlex() << std::endl;
    std::cout << "Expanded size: " << expanded->getWidth() << "x" << expanded->getHeight() << std::endl;
#endif
```

## Related Documentation

- [Column Layout](column.md) - For using Expanded in columns
- [Row Layout](row.md) - For using Expanded in rows
- [Layout System Overview](overview.md) - For general layout concepts
- [Spacing Layout](spacing.md) - For fixed spacing between widgets
- [Padding Layout](padding.md) - For adding padding around widgets

---

*This documentation covers the Fern Expanded Layout system. For more examples and advanced usage, see the examples directory.*
