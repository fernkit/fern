#pragma once
#include <string>
#include "widget.hpp"          
#include "../../core/input.hpp"
#include "../../core/types.hpp"
#include "../../font/font.hpp"

namespace Fern {
    
    class TextWidget : public Widget {
    public:
        TextWidget(Point position, const std::string& text, int size, uint32_t color, 
                  bool addToManager = true, FontType fontType = FontType::Bitmap);
        
        void render() override;
        bool handleInput(const InputState& input) override { return false; }
        
        void setText(const std::string& text);
        void setSize(int size);
        void setColor(uint32_t color);
        void setFontType(FontType type) { fontType_ = type; }
        void renderTTF();
        void renderBitmap();
        
        const std::string& getText() const { return text_; }
        int getSize() const { return size_; }
        uint32_t getColor() const { return color_; }
        FontType getFontType() const { return fontType_; }
        
    private:
        std::string text_;
        int size_;
        uint32_t color_;
        FontType fontType_;
        
        void updateDimensions();
    };
    
    // Helper function
    std::shared_ptr<TextWidget> Text(Point position, const std::string& text, 
                                    int size, uint32_t color, bool addToManager = true,
                                    FontType fontType = FontType::Bitmap);
}