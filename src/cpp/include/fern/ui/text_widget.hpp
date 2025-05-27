#pragma once
#include "widget.hpp"
#include <string>
#include <memory>

namespace Fern {
    // First, define the class BEFORE using it with shared_ptr
    class TextWidget : public Widget {
    public:
        TextWidget(Point position, const std::string& text, int scale, uint32_t color);
        
        void render() override;
        bool handleInput(const InputState& input) override;
        
        // Text-specific methods
        void setText(const std::string& text);
        void setPosition(const Point& position);
        void setScale(int scale);
        void setColor(uint32_t color);
        
        // Getters
        const std::string& getText() const { return text_; }
        const Point& getPosition() const { return position_; }
        int getScale() const { return scale_; }
        uint32_t getColor() const { return color_; }
        int getWidth() const override { 
            return text_.length() * 8 * scale_; 
        }

        int getHeight() const override { 
            return 8 * scale_; 
        }
        
    private:
        std::string text_;
        Point position_;
        int scale_;
        uint32_t color_;
    };

    std::shared_ptr<TextWidget> Text(Point position, const std::string& text, 
                                     int scale, uint32_t color, bool addToManager = true);
    
                                              
}