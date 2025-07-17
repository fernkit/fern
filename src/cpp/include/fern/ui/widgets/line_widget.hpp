#pragma once
#include "widget.hpp"
#include <memory>

namespace Fern {
    /**
     * @brief Widget for drawing lines between two points
     * 
     * LineWidget provides a simple way to draw straight lines with configurable
     * thickness and color. Useful for creating dividers, borders, or simple graphics.
     * 
     * @example Basic line usage:
     * @code
     * auto line = Line({100, 50}, {200, 150}, 2, Colors::White);
     * @endcode
     * 
     * @example Thick colored line:
     * @code
     * auto thickLine = Line({0, 100}, {400, 100}, 5, Colors::Red);
     * @endcode
     */
    class LineWidget : public Widget {
    public:
        /**
         * @brief Construct a new Line Widget
         * @param start Starting point of the line
         * @param end Ending point of the line
         * @param thickness Line thickness in pixels
         * @param color Line color in ARGB format
         */
        LineWidget(Point start, Point end, int thickness, uint32_t color);
        
        /**
         * @brief Render the line to the canvas
         */
        void render() override;
        
        /**
         * @brief Handle input events (lines don't handle input)
         * @param input Input state
         * @return false (lines are not interactive)
         */
        bool handleInput(const InputState& input) override;
        
        // Setters
        /**
         * @brief Set the starting point of the line
         * @param start New starting point
         */
        void setStart(Point start);
        
        /**
         * @brief Set the ending point of the line
         * @param end New ending point
         */
        void setEnd(Point end);
        
        /**
         * @brief Set the line thickness
         * @param thickness New thickness in pixels
         */
        void setThickness(int thickness);
        
        /**
         * @brief Set the line color
         * @param color New color in ARGB format
         */
        void setColor(uint32_t color);
        
        // Getters
        Point getStart() const { return start_; }
        Point getEnd() const { return end_; }
        int getThickness() const { return thickness_; }
        uint32_t getColor() const { return color_; }
        
    private:
        Point start_;
        Point end_;
        int thickness_;
        uint32_t color_;
    };
    
    /**
     * @brief Factory function for creating LineWidget
     * @param start Starting point of the line
     * @param end Ending point of the line
     * @param thickness Line thickness in pixels
     * @param color Line color in ARGB format
     * @param addToManager Whether to automatically add to widget manager
     * @return std::shared_ptr<LineWidget> Shared pointer to created widget
     */
    std::shared_ptr<LineWidget> Line(Point start, Point end, int thickness, uint32_t color, bool addToManager = true);
}