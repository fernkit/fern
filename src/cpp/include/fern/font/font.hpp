#pragma once
#include "../core/canvas.hpp"
#include <string>
#include <memory>

namespace Fern {
    
    // Forward declarations
    class Canvas;
    class TTFFontRenderer;
    
    enum class FontType {
        Bitmap,  // Your existing bitmap font
        TTF      // New TTF support
    };
    
    class Font {
    public:
        // TTF font management
        static bool loadTTFFont(const std::string& name, const std::string& fontPath);
        static void setDefaultTTFFont(const std::string& name);
        static bool hasTTFFont();
        
        // Unified text rendering
        static void renderText(Canvas* canvas, const std::string& text, int x, int y, 
                              int size, uint32_t color, FontType type = FontType::Bitmap);
        
        // Keep existing bitmap font functions for compatibility
        static void renderBitmap(Canvas* canvas, const std::string& text, int x, int y, 
                                int size, uint32_t color);
        
        // TTF-specific rendering
        static void renderTTF(Canvas* canvas, const std::string& text, int x, int y, 
                             int size, uint32_t color, const std::string& fontName = "");
        
        // Text metrics
        static int getTextWidth(const std::string& text, int size, FontType type = FontType::Bitmap);
        static int getTextHeight(int size, FontType type = FontType::Bitmap);
    };
    
    // Helper functions for easy TTF usage
    namespace TTF {
        bool load(const std::string& name, const std::string& path);
        void setDefault(const std::string& name);
        void render(Canvas* canvas, const std::string& text, int x, int y, 
                   int size, uint32_t color, const std::string& fontName = "");
        int textWidth(const std::string& text, int size, const std::string& fontName = "");
        int textHeight(int size, const std::string& fontName = "");
    }
}
