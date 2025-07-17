#pragma once
#include "../widgets/widget.hpp"
#include "../../graphics/primitives.hpp"
#include "../../core/responsive_widget.hpp"
#include <vector>
#include <memory>

namespace Fern {

    /**
     * @brief Alignment options for the main axis in layout widgets
     * 
     * Controls how children are positioned along the primary axis
     * (horizontal for rows, vertical for columns).
     */
    enum class MainAxisAlignment {
        Start,         ///< Align children to the start
        Center,        ///< Center children
        End,           ///< Align children to the end
        SpaceBetween,  ///< Space children evenly with no space at edges
        SpaceAround,   ///< Space children evenly with half space at edges
        SpaceEvenly    ///< Space children evenly with equal space everywhere
    };

    /**
     * @brief Alignment options for the cross axis in layout widgets
     * 
     * Controls how children are positioned along the secondary axis
     * (vertical for rows, horizontal for columns).
     */
    enum class CrossAxisAlignment {
        Start,    ///< Align children to the start
        Center,   ///< Center children
        End,      ///< Align children to the end
        Stretch   ///< Stretch children to fill the cross axis
    };

    /**
     * @brief Base class for layout widgets that arrange child widgets
     * 
     * LayoutWidget provides common functionality for arranging multiple
     * child widgets within a container. Subclasses implement specific
     * layout algorithms (e.g., row, column, center).
     * 
     * @note This is an abstract base class - use specific layout widgets
     * like RowWidget, ColumnWidget, or CenterWidget instead.
     */
    class LayoutWidget : public Widget {
    public:
        /**
         * @brief Construct a new Layout Widget
         * 
         * @param x X position
         * @param y Y position
         * @param width Width in pixels
         * @param height Height in pixels
         */
        LayoutWidget(int x, int y, int width, int height) {
            x_ = x;
            y_ = y;
            width_ = width;
            height_ = height;
        }
        
        virtual ~LayoutWidget() = default;
        
        /**
         * @brief Render all child widgets
         */
        void render() override {        
            for (auto& child : children_) {
                child->render();
            }
        }
        
        /**
         * @brief Update the layout arrangement of children
         */
        void updateLayout() {
            arrangeChildren();
        }

        /**
         * @brief Handle input events and forward to children
         * 
         * @param input Current input state
         * @return true if any child handled the input
         */
        bool handleInput(const InputState& input) override {
            for (auto it = children_.rbegin(); it != children_.rend(); ++it) {
                if ((*it)->handleInput(input)) {
                    return true;
                }
            }
            return false;
        }
        
        /**
         * @brief Set the position and update child positions
         * 
         * @param x New X position
         * @param y New Y position
         */
        void setPosition(int x, int y) override {
            int deltaX = x - x_;
            int deltaY = y - y_;
            
            x_ = x;
            y_ = y;
            
            for (auto& child : children_) {
                child->setPosition(child->getX() + deltaX, child->getY() + deltaY);
            }
        }
        
        /**
         * @brief Resize the layout and rearrange children
         * 
         * @param width New width in pixels
         * @param height New height in pixels
         */
        void resize(int width, int height) override {
            width_ = width;
            height_ = height;
            arrangeChildren();
        }
        
    protected:
        /**
         * @brief Abstract method to arrange child widgets
         * 
         * Subclasses must implement this to define their specific layout algorithm.
         */
        virtual void arrangeChildren() = 0;
        
        std::vector<std::shared_ptr<Widget>> children_;  ///< Child widgets
    };

    /**
     * @brief Layout widget that centers its child widget
     * 
     * CenterWidget positions its child at the center of its bounds and
     * supports responsive resizing when the window size changes.
     * 
     * @example Centering a button:
     * @code
     * auto button = Button(ButtonConfig(0, 0, 100, 30, "Click"));
     * auto centered = Center(button);
     * @endcode
     */
    class CenterWidget : public LayoutWidget, public ResponsiveWidget {
    public:
        /**
         * @brief Construct a new Center Widget
         * 
         * @param x X position
         * @param y Y position
         * @param width Width in pixels
         * @param height Height in pixels
         */
        CenterWidget(int x, int y, int width, int height);
        
        /**
         * @brief Add a child widget to center
         * 
         * @param child Widget to center
         */
        void add(std::shared_ptr<Widget> child);
        
        /**
         * @brief Handle window resize events
         * 
         * @param newWidth New window width
         * @param newHeight New window height
         */
        void onWindowResize(int newWidth, int newHeight) override;
        
    protected:
        /**
         * @brief Arrange child at center position
         */
        void arrangeChildren() override;
    };

    /**
     * @brief Layout widget that arranges children vertically in a column
     * 
     * ColumnWidget stacks child widgets vertically with configurable
     * alignment options for both main axis (vertical) and cross axis (horizontal).
     * 
     * @example Creating a column of buttons:
     * @code
     * auto column = Column({
     *     Button(ButtonConfig(0, 0, 100, 30, "First")),
     *     Button(ButtonConfig(0, 0, 100, 30, "Second")),
     *     Button(ButtonConfig(0, 0, 100, 30, "Third"))
     * }, false, MainAxisAlignment::Center, CrossAxisAlignment::Center);
     * @endcode
     */
    class ColumnWidget : public LayoutWidget {
    private:
        MainAxisAlignment mainAxisAlignment_ = MainAxisAlignment::Start;   ///< Vertical alignment
        CrossAxisAlignment crossAxisAlignment_ = CrossAxisAlignment::Center; ///< Horizontal alignment
    public:
        /**
         * @brief Construct a new Column Widget
         * 
         * @param x X position
         * @param y Y position
         * @param width Width in pixels
         * @param height Height in pixels
         * @param mainAlignment Vertical alignment (default: Start)
         * @param crossAlignment Horizontal alignment (default: Center)
         */
        ColumnWidget(int x, int y, int width, int height, 
                 MainAxisAlignment mainAlignment = MainAxisAlignment::Start,
                 CrossAxisAlignment crossAlignment = CrossAxisAlignment::Center);
        
        /**
         * @brief Add a child widget to the column
         * 
         * @param child Widget to add
         */
        void add(std::shared_ptr<Widget> child);
        
        /**
         * @brief Add multiple child widgets to the column
         * 
         * @param children Vector of widgets to add
         */
        void addAll(const std::vector<std::shared_ptr<Widget>>& children);
        
        /**
         * @brief Set the main axis (vertical) alignment
         * 
         * @param alignment New vertical alignment
         */
        void setMainAxisAlignment(MainAxisAlignment alignment);
        
        /**
         * @brief Set the cross axis (horizontal) alignment
         * 
         * @param alignment New horizontal alignment
         */
        void setCrossAxisAlignment(CrossAxisAlignment alignment);
        
    protected:
        /**
         * @brief Arrange children in a vertical column
         */
        void arrangeChildren() override;
    };

    /**
     * @brief Layout widget that arranges children horizontally in a row
     * 
     * RowWidget places child widgets side by side horizontally with configurable
     * alignment options for both main axis (horizontal) and cross axis (vertical).
     * 
     * @example Creating a row of buttons:
     * @code
     * auto row = Row({
     *     Button(ButtonConfig(0, 0, 80, 30, "OK")),
     *     Button(ButtonConfig(0, 0, 80, 30, "Cancel"))
     * }, false, MainAxisAlignment::SpaceBetween);
     * @endcode
     */
    class RowWidget : public LayoutWidget {
    private:
        MainAxisAlignment mainAxisAlignment_ = MainAxisAlignment::Start;   ///< Horizontal alignment
        CrossAxisAlignment crossAxisAlignment_ = CrossAxisAlignment::Center; ///< Vertical alignment
    public:
        /**
         * @brief Construct a new Row Widget
         * 
         * @param x X position
         * @param y Y position
         * @param width Width in pixels
         * @param height Height in pixels
         * @param mainAlignment Horizontal alignment (default: Start)
         * @param crossAlignment Vertical alignment (default: Center)
         */
        RowWidget(int x, int y, int width, int height, MainAxisAlignment mainAlignment = MainAxisAlignment::Start,
              CrossAxisAlignment crossAlignment = CrossAxisAlignment::Center);
        
        /**
         * @brief Set the main axis (horizontal) alignment
         * 
         * @param alignment New horizontal alignment
         */
        void setMainAxisAlignment(MainAxisAlignment alignment);
        
        /**
         * @brief Set the cross axis (vertical) alignment
         * 
         * @param alignment New vertical alignment
         */
        void setCrossAxisAlignment(CrossAxisAlignment alignment);
        
        /**
         * @brief Add a child widget to the row
         * 
         * @param child Widget to add
         */
        void add(std::shared_ptr<Widget> child);
        
        /**
         * @brief Add multiple child widgets to the row
         * 
         * @param children Vector of widgets to add
         */
        void addAll(const std::vector<std::shared_ptr<Widget>>& children);
        
    protected:
        /**
         * @brief Arrange children in a horizontal row
         */
        void arrangeChildren() override;
    };

    /**
     * @brief Widget that expands to fill available space in flex layouts
     * 
     * ExpandedWidget is used within Row or Column layouts to make a child
     * widget expand to fill the available space. The flex value determines
     * how much space to take relative to other Expanded widgets.
     * 
     * @example Using expanded widgets in a row:
     * @code
     * auto row = Row({
     *     button1,
     *     Expanded(button2, 2),  // Takes 2x space
     *     Expanded(button3, 1)   // Takes 1x space
     * });
     * @endcode
     */
    class ExpandedWidget : public LayoutWidget {
    public:
        /**
         * @brief Construct a new Expanded Widget
         * 
         * @param child Child widget to expand
         * @param flex Flex factor (default: 1)
         */
        ExpandedWidget(std::shared_ptr<Widget> child, int flex = 1);
        
        /**
         * @brief Get the flex factor
         * 
         * @return Flex factor value
         */
        int getFlex() const { return flex_; }
        
        /**
         * @brief Render the expanded child widget
         */
        void render() override;
        
    protected:
        /**
         * @brief Arrange the child to fill available space
         */
        void arrangeChildren() override;
        
        int flex_;  ///< Flex factor for space allocation
    };
    
    /**
     * @brief Factory function to create an expanded widget
     * 
     * @param child Child widget to expand
     * @param flex Flex factor (default: 1)
     * @param addToManager Whether to add to widget manager (default: false)
     * @return std::shared_ptr<ExpandedWidget> Shared pointer to expanded widget
     */
    std::shared_ptr<ExpandedWidget> Expanded(
        std::shared_ptr<Widget> child, 
        int flex = 1,
        bool addToManager = false
    );

    /**
     * @brief Widget that adds padding around its child widget
     * 
     * PaddingWidget creates space around a child widget by applying
     * padding on all four sides. Different padding values can be
     * specified for each side.
     * 
     * @example Adding padding around a button:
     * @code
     * auto paddedButton = Padding(button, 10); // 10px on all sides
     * auto customPadding = Padding(button, 5, 10, 5, 10); // left, top, right, bottom
     * @endcode
     */
    class PaddingWidget : public LayoutWidget {
    public:
        /**
         * @brief Construct a new Padding Widget
         * 
         * @param x X position
         * @param y Y position
         * @param width Width in pixels
         * @param height Height in pixels
         * @param left Left padding in pixels
         * @param top Top padding in pixels
         * @param right Right padding in pixels
         * @param bottom Bottom padding in pixels
         */
        PaddingWidget(int x, int y, int width, int height, 
                    int left, int top, int right, int bottom);
        
        /**
         * @brief Add a child widget to apply padding to
         * 
         * @param child Child widget
         */
        void add(std::shared_ptr<Widget> child);
        
    protected:
        /**
         * @brief Arrange child with padding applied
         */
        void arrangeChildren() override;
        
    private:
        int paddingLeft_, paddingTop_, paddingRight_, paddingBottom_;  ///< Padding values
    };

    /**
     * @brief Widget that creates empty space of specified dimensions
     * 
     * SpacingWidget is useful for creating gaps between widgets in layouts.
     * It doesn't render anything but takes up the specified space.
     * 
     * @example Adding space between buttons in a column:
     * @code
     * auto column = Column({
     *     button1,
     *     SizedBox(0, 10),  // 10px vertical spacing
     *     button2
     * });
     * @endcode
     */
    class SpacingWidget : public Widget {
    public:
        /**
         * @brief Construct a new Spacing Widget
         * 
         * @param width Width in pixels
         * @param height Height in pixels
         */
        SpacingWidget(int width, int height);
        
        /**
         * @brief Render nothing (empty space)
         */
        void render() override;
        
        /**
         * @brief Handle no input (passes through)
         * 
         * @param input Current input state
         * @return false (never handles input)
         */
        bool handleInput(const InputState& input) override;
    };

    /**
     * @brief Factory function to create a center widget
     * 
     * @param child Child widget to center
     * @param addToManager Whether to add to widget manager (default: false)
     * @return std::shared_ptr<CenterWidget> Shared pointer to center widget
     */
    std::shared_ptr<CenterWidget> Center(
        std::shared_ptr<Widget> child, 
        bool addToManager = false
    );
    
    /**
     * @brief Factory function to create a row widget
     * 
     * @param children Vector of child widgets to arrange horizontally
     * @param addToManager Whether to add to widget manager (default: false)
     * @param mainAlignment Horizontal alignment (default: Start)
     * @param crossAlignment Vertical alignment (default: Center)
     * @return std::shared_ptr<RowWidget> Shared pointer to row widget
     */
    std::shared_ptr<RowWidget> Row(
        const std::vector<std::shared_ptr<Widget>>& children, 
        bool addToManager = false,
        MainAxisAlignment mainAlignment = MainAxisAlignment::Start,
        CrossAxisAlignment crossAlignment = CrossAxisAlignment::Center
    );

    /**
     * @brief Factory function to create a column widget
     * 
     * @param children Vector of child widgets to arrange vertically
     * @param addToManager Whether to add to widget manager (default: false)
     * @param mainAlignment Vertical alignment (default: Start)
     * @param crossAlignment Horizontal alignment (default: Center)
     * @return std::shared_ptr<ColumnWidget> Shared pointer to column widget
     */
    std::shared_ptr<ColumnWidget> Column(
        const std::vector<std::shared_ptr<Widget>>& children, 
        bool addToManager = false,
        MainAxisAlignment mainAlignment = MainAxisAlignment::Start,
        CrossAxisAlignment crossAlignment = CrossAxisAlignment::Center
    );
    
    /**
     * @brief Factory function to create padding widget with uniform padding
     * 
     * @param child Child widget to add padding around
     * @param all Padding amount for all sides
     * @param addToManager Whether to add to widget manager (default: false)
     * @return std::shared_ptr<PaddingWidget> Shared pointer to padding widget
     */
    std::shared_ptr<PaddingWidget> Padding(std::shared_ptr<Widget> child, int all, bool addToManager = false);
    
    /**
     * @brief Factory function to create padding widget with custom padding
     * 
     * @param child Child widget to add padding around
     * @param left Left padding in pixels
     * @param top Top padding in pixels
     * @param right Right padding in pixels
     * @param bottom Bottom padding in pixels
     * @param addToManager Whether to add to widget manager (default: false)
     * @return std::shared_ptr<PaddingWidget> Shared pointer to padding widget
     */
    std::shared_ptr<PaddingWidget> Padding(std::shared_ptr<Widget> child, 
                                         int left, int top, int right, int bottom, bool addToManager = false);
    
    /**
     * @brief Factory function to create a spacing/sized box widget
     * 
     * @param width Width in pixels
     * @param height Height in pixels
     * @param addToManager Whether to add to widget manager (default: false)
     * @return std::shared_ptr<SpacingWidget> Shared pointer to spacing widget
     */
    std::shared_ptr<SpacingWidget> SizedBox(int width, int height, bool addToManager = false);
}