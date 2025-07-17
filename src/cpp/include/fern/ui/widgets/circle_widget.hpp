#pragma once
#include "widget.hpp"
#include <memory>
#include "../../core/signal.hpp"

namespace Fern {
    /**
     * @brief A circular widget that can be clicked and hovered
     * 
     * CircleWidget represents a filled circle that can respond to mouse events.
     * It supports hover effects and click handling through signals. The circle
     * is positioned by its center point and has a configurable radius and color.
     * 
     * @example Basic circle usage:
     * @code
     * auto circle = Circle(30, Point(100, 100), Colors::Red);
     * circle->onClick.connect([]() { 
     *     std::cout << "Circle clicked!" << std::endl; 
     * });
     * circle->onHover.connect([](bool hovered) {
     *     if (hovered) std::cout << "Circle hovered" << std::endl;
     * });
     * addWidget(circle);
     * @endcode
     */
    class CircleWidget : public Widget {
    public:
        /**
         * @brief Construct a new Circle Widget
         * 
         * @param radius The radius of the circle in pixels
         * @param position The center position of the circle
         * @param color The fill color of the circle (ARGB format)
         */
        CircleWidget(int radius, Point position, uint32_t color);
        
        /**
         * @brief Render the circle to the canvas
         */
        void render() override;
        
        /**
         * @brief Handle input events (mouse hover and clicks)
         * 
         * @param input Current input state
         * @return true if input was handled, false otherwise
         */
        bool handleInput(const InputState& input) override;
        
        /**
         * @brief Set the radius of the circle
         * 
         * @param radius New radius in pixels
         */
        void setRadius(int radius);
        
        /**
         * @brief Set the center position of the circle
         * 
         * @param position New center position
         */
        void setPosition(const Point& position);
        
        /**
         * @brief Set the fill color of the circle
         * 
         * @param color New color in ARGB format
         */
        void setColor(uint32_t color);
        
        /**
         * @brief Get the current radius
         * 
         * @return Current radius in pixels
         */
        int getRadius() const { return radius_; }
        
        /**
         * @brief Get the current center position
         * 
         * @return Current center position
         */
        Point getPosition() const { return position_; }
        
        /**
         * @brief Get the current fill color
         * 
         * @return Current color in ARGB format
         */
        uint32_t getColor() const { return color_; }
        
        /**
         * @brief Get the width of the circle's bounding box
         * 
         * @return Width (diameter) of the circle
         */
        int getWidth() const override { return radius_ * 2; }
        
        /**
         * @brief Get the height of the circle's bounding box
         * 
         * @return Height (diameter) of the circle
         */
        int getHeight() const override { return radius_ * 2; }
        
        /**
         * @brief Signal emitted when the circle is clicked
         * 
         * Connect to this signal to handle click events:
         * @code
         * circle->onClick.connect()
         * @endcode
         */
        Signal<> onClick; 
        
        /**
         * @brief Signal emitted when hover state changes
         * 
         * Connect to this signal to handle hover events:
         * @code
         * circle->onHover.connect([](bool isHovered) { 
         *     // Handle hover state change
         * });
         * @endcode
         * 
         * @param isHovered true when mouse enters, false when mouse leaves
         */
        Signal<bool> onHover; 
        
    private:
        int radius_;         ///< Radius of the circle in pixels
        Point position_;     ///< Center position of the circle
        uint32_t color_;     ///< Fill color in ARGB format
        bool isHovered_;     ///< Current hover state
    };
    
    /**
     * @brief Factory function to create a circle widget
     * 
     * @param radius The radius of the circle in pixels
     * @param position The center position of the circle
     * @param color The fill color of the circle (ARGB format)
     * @param addToManager Whether to automatically add to widget manager (default: true)
     * @return std::shared_ptr<CircleWidget> Shared pointer to the created circle
     * 
     * @example
     * @code
     * auto circle = Circle(25, Point(200, 150), Colors::Blue);
     * @endcode
     */
    std::shared_ptr<CircleWidget> Circle(int radius, Point position, uint32_t color, bool addToManager = true);
}