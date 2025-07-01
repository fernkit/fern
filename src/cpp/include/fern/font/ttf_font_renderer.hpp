#pragma once
#include "ttf_reader.hpp"
#include "../core/canvas.hpp"
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>
#include <stdexcept>

namespace Fern {
    
    class TTFFontRenderer {
    public:
        struct RasterizedGlyph {
            std::vector<uint8_t> bitmap;
            int width = 0;
            int height = 0;
            int bearingX = 0;    // Left side bearing
            int bearingY = 0;    // Top side bearing
            int advance = 0;     // How much to advance cursor
        };
        
        // Constructor - can throw std::runtime_error if font fails to load
        explicit TTFFontRenderer(const std::string& fontPath);
        
        // Destructor
        ~TTFFontRenderer() = default;
        
        // Delete copy constructor and assignment operator
        TTFFontRenderer(const TTFFontRenderer&) = delete;
        TTFFontRenderer& operator=(const TTFFontRenderer&) = delete;
        
        // Move constructor and assignment operator
        TTFFontRenderer(TTFFontRenderer&&) = default;
        TTFFontRenderer& operator=(TTFFontRenderer&&) = default;
        
        // Main rendering functions
        RasterizedGlyph rasterizeGlyph(char character, int fontSize);
        void renderText(Canvas* canvas, const std::string& text, int x, int y, 
                       int fontSize, uint32_t color);
        
        // Text metrics
        int getTextWidth(const std::string& text, int fontSize);
        int getTextHeight(int fontSize);
        int getLineHeight(int fontSize);
        
        // Font information
        bool isValid() const { return ttfReader_ != nullptr; }
        std::string getFontName() const;
        
        // Cache management
        void clearCache() { glyphCache_.clear(); }
        size_t getCacheSize() const { return glyphCache_.size(); }
        
    private:
        std::unique_ptr<TTFReader> ttfReader_;
        std::unordered_map<int, RasterizedGlyph> glyphCache_; // char + size -> glyph
        
        // 2D Point structure for outline generation
        struct Point2D {
            float x, y;
            Point2D(float x = 0, float y = 0) : x(x), y(y) {}
        };
        
        // Internal rendering methods
        void rasterizeGlyphOutline(const SimpleGlyph& glyph, int fontSize, RasterizedGlyph& output);
        void rasterizeGlyphContours(const SimpleGlyph& glyph, RasterizedGlyph& output, float scale);
        std::vector<Point2D> generateHighQualityContour(const SimpleGlyph& glyph, size_t startPt, size_t endPt, float scale);
        Point2D quadraticBezier(const Point2D& start, const Point2D& control, const Point2D& end, float t);
        void rasterizeContourLine(const std::vector<Point2D>& points, RasterizedGlyph& output);
        void drawLine(const Point2D& p1, const Point2D& p2, RasterizedGlyph& output);
        void fillInterior(RasterizedGlyph& output);
        void fillContour(RasterizedGlyph& output); // Legacy method
        
        // Legacy methods (kept for compatibility)
        void drawLine(std::vector<uint8_t>& bitmap, int width, int height, 
                     int x0, int y0, int x1, int y1);
        void fillGlyph(std::vector<uint8_t>& bitmap, int width, int height);
        
        // Helper methods
        int getCacheKey(char character, int fontSize) const { 
            return (fontSize << 8) | static_cast<unsigned char>(character); 
        }
        
        uint32_t blendPixel(uint32_t background, uint32_t foreground, uint8_t alpha);
        
        // Font metrics
        float getScaleFactor(int fontSize) const { return static_cast<float>(fontSize) / 1000.0f; }
        int getUnitsPerEm() const { return 1000; } // Most TTF fonts use 1000 units per em
    };
    
    // Global font management
    class TTFFontManager {
    public:
        static TTFFontManager& getInstance() {
            static TTFFontManager instance;
            return instance;
        }
        
        bool loadFont(const std::string& name, const std::string& fontPath);
        TTFFontRenderer* getFont(const std::string& name);
        void setDefaultFont(const std::string& name);
        TTFFontRenderer* getDefaultFont();
        
        void clearAllCaches();
        
        // Check if any fonts are loaded
        bool hasAnyFont() const { return !fonts_.empty(); }
        
    private:
        std::unordered_map<std::string, std::unique_ptr<TTFFontRenderer>> fonts_;
        std::string defaultFontName_;
        
        TTFFontManager() = default;
        ~TTFFontManager() = default;
        
        // Delete copy/move constructors for singleton
        TTFFontManager(const TTFFontManager&) = delete;
        TTFFontManager& operator=(const TTFFontManager&) = delete;
        TTFFontManager(TTFFontManager&&) = delete;
        TTFFontManager& operator=(TTFFontManager&&) = delete;
    };
}
