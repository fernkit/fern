#include "../../include/fern/font/font.hpp"
#include "../../include/fern/text/font.hpp"
#include "../../include/fern/font/ttf_font_renderer.hpp"
#include "../../include/fern/core/canvas.hpp"
#include <iostream>

namespace Fern {
    
    bool Font::loadTTFFont(const std::string& name, const std::string& fontPath) {
        return TTFFontManager::getInstance().loadFont(name, fontPath);
    }
    
    void Font::setDefaultTTFFont(const std::string& name) {
        TTFFontManager::getInstance().setDefaultFont(name);
    }
    
    bool Font::hasTTFFont() {
        return TTFFontManager::getInstance().getDefaultFont() != nullptr;
    }
    
    void Font::renderText(Canvas* canvas, const std::string& text, int x, int y, 
                         int size, uint32_t color, FontType type) {
        switch (type) {
            case FontType::TTF:
                renderTTF(canvas, text, x, y, size, color);
                break;
            case FontType::Bitmap:
            default:
                renderBitmap(canvas, text, x, y, size, color);
                break;
        }
    }
    
    void Font::renderTTF(Canvas* canvas, const std::string& text, int x, int y, 
                        int size, uint32_t color, const std::string& fontName) {
        auto& manager = TTFFontManager::getInstance();
        TTFFontRenderer* renderer = fontName.empty() ? 
            manager.getDefaultFont() : 
            manager.getFont(fontName);
            
        if (renderer) {
            renderer->renderText(canvas, text, x, y, size, color);
        } else {
            std::cerr << "TTF font not found, falling back to bitmap font" << std::endl;
            renderBitmap(canvas, text, x, y, size, color);
        }
    }
    
    int Font::getTextWidth(const std::string& text, int size, FontType type) {
        if (type == FontType::TTF) {
            auto* renderer = TTFFontManager::getInstance().getDefaultFont();
            if (renderer) {
                return renderer->getTextWidth(text, size);
            }
        }
        
        // Fallback to bitmap font calculation
        return text.length() * (size * 6 / 8); // Approximate bitmap font width
    }
    
    int Font::getTextHeight(int size, FontType type) {
        if (type == FontType::TTF) {
            auto* renderer = TTFFontManager::getInstance().getDefaultFont();
            if (renderer) {
                return renderer->getTextHeight(size);
            }
        }
        
        return size; // Bitmap font height
    }

    void Font::renderBitmap(Canvas* canvas, const std::string& text, int x, int y, 
                       int size, uint32_t color) {
        // Bridge to existing DrawText system
        DrawText::drawText(text.c_str(), x, y, size, color);
    }

    // Helper namespace implementation
    namespace TTF {
        bool load(const std::string& name, const std::string& path) {
            return Font::loadTTFFont(name, path);
        }
        
        void setDefault(const std::string& name) {
            Font::setDefaultTTFFont(name);
        }
        
        void render(Canvas* canvas, const std::string& text, int x, int y, 
                   int size, uint32_t color, const std::string& fontName) {
            Font::renderTTF(canvas, text, x, y, size, color, fontName);
        }
        
        int textWidth(const std::string& text, int size, const std::string& fontName) {
            auto& manager = TTFFontManager::getInstance();
            TTFFontRenderer* renderer = fontName.empty() ? 
                manager.getDefaultFont() : 
                manager.getFont(fontName);
                
            return renderer ? renderer->getTextWidth(text, size) : 0;
        }
        
        int textHeight(int size, const std::string& fontName) {
            auto& manager = TTFFontManager::getInstance();
            TTFFontRenderer* renderer = fontName.empty() ? 
                manager.getDefaultFont() : 
                manager.getFont(fontName);
                
            return renderer ? renderer->getTextHeight(size) : 0;
        }
    }
}