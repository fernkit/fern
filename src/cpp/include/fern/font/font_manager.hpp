#pragma once
#include "ttf_font_renderer.hpp"
#include <memory>
#include <string>

namespace Fern {
    
    class FontManager {
    public:
        static FontManager& getInstance() {
            static FontManager instance;
            return instance;
        }
        
        // TTF font management
        bool loadTTFFont(const std::string& name, const std::string& fontPath);
        void setDefaultTTFFont(const std::string& name);
        bool hasTTFFont() const;
        TTFFontRenderer* getTTFFont(const std::string& name = "");
        
        // Font metrics and rendering
        int getTextWidth(const std::string& text, int size, const std::string& fontName = "");
        int getTextHeight(int size, const std::string& fontName = "");
        
        // Cache management
        void clearAllCaches();
        
    private:
        FontManager() = default;
        ~FontManager() = default;
        
        // Delete copy/move constructors for singleton
        FontManager(const FontManager&) = delete;
        FontManager& operator=(const FontManager&) = delete;
        FontManager(FontManager&&) = delete;
        FontManager& operator=(FontManager&&) = delete;
        
        TTFFontManager& getTTFManager() { return TTFFontManager::getInstance(); }
    };
}
