#include "../../include/fern/font/ttf_font_renderer.hpp"
#include <algorithm>
#include <cmath>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

namespace Fern {

TTFFontRenderer::TTFFontRenderer(const std::string& fontPath) {
    ttfReader_ = std::make_unique<TTFReader>();
    
    if (!ttfReader_->openFont(fontPath)) {
        throw std::runtime_error("Failed to open font: " + fontPath);
    }
    
    // Load required tables
    if (!ttfReader_->loadHeadTable()) {

    }
    
    if (!ttfReader_->loadCmapTable()) {
        throw std::runtime_error("Failed to load cmap table");
    }
    
    if (!ttfReader_->loadLocaTable()) {
        throw std::runtime_error("Failed to load loca table");
    }
    
}

TTFFontRenderer::RasterizedGlyph TTFFontRenderer::rasterizeGlyph(char character, int fontSize) {
    int cacheKey = getCacheKey(character, fontSize);
    
    // Check cache first
    auto it = glyphCache_.find(cacheKey);
    if (it != glyphCache_.end()) {
        return it->second;
    }
    
    
    // Get glyph index for character
    uint16_t glyphIndex = ttfReader_->getGlyphIndex(static_cast<uint32_t>(character));
    
    
    // Read glyph data
    SimpleGlyph glyph;
    if (!ttfReader_->readGlyphByIndex(glyphIndex, glyph)) {
        // Return fallback glyph for missing characters
        RasterizedGlyph empty;
        empty.width = fontSize / 2;
        empty.height = fontSize;
        empty.advance = fontSize / 2;
        empty.bearingX = 0;
        empty.bearingY = fontSize;
        
        // Create a simple fallback bitmap (small rectangle)
        empty.bitmap.resize(empty.width * empty.height, 0);
        for (int y = 2; y < empty.height - 2; y++) {
            for (int x = 2; x < empty.width - 2; x++) {
                empty.bitmap[y * empty.width + x] = 128;
            }
        }
        
        glyphCache_[cacheKey] = empty;
        return empty;
    }
    
    // Rasterize the glyph
    RasterizedGlyph rasterized;
    rasterizeGlyphOutline(glyph, fontSize, rasterized);
    
    // Cache the result
    glyphCache_[cacheKey] = rasterized;
    
    return rasterized;
}

void TTFFontRenderer::rasterizeGlyphOutline(const SimpleGlyph& glyph, int fontSize, RasterizedGlyph& output) {
    float scale = static_cast<float>(fontSize) / static_cast<float>(ttfReader_->getUnitsPerEm());
    
    int glyphWidth = glyph.header.xMax - glyph.header.xMin;
    int glyphHeight = glyph.header.yMax - glyph.header.yMin;
    
#ifdef __EMSCRIPTEN__
    EM_ASM({
        console.log("🌿 Glyph bounds: width=" + $0 + ", height=" + $1 + ", scale=" + $2 + ", unitsPerEm=" + $3);
    }, glyphWidth, glyphHeight, scale, ttfReader_->getUnitsPerEm());
#endif
    

    int scaledWidth = std::max(2, static_cast<int>(glyphWidth * scale));
    int scaledHeight = std::max(2, static_cast<int>(glyphHeight * scale)); 
    
    output.width = scaledWidth + 4;
    output.height = scaledHeight + 4;
    output.bearingX = static_cast<int>(glyph.header.xMin * scale);
    output.bearingY = static_cast<int>(glyph.header.yMax * scale); // Distance from baseline to top of glyph
    output.advance = std::max(1, static_cast<int>(glyphWidth * scale));
    
    // Create bitmap
    output.bitmap.resize(output.width * output.height, 0);
    
#ifdef __EMSCRIPTEN__
    EM_ASM({
        console.log("🌿 Rasterizing glyph: outputW=" + $0 + ", outputH=" + $1 + ", bearingY=" + $2 + ", advance=" + $3);
    }, output.width, output.height, output.bearingY, output.advance);
#endif
    
    // If no points, create fallback
    if (glyph.points.empty()) {
        // Create a simple filled rectangle as fallback
        for (int y = 1; y < output.height - 1; y++) {
            for (int x = 1; x < output.width - 1; x++) {
                output.bitmap[y * output.width + x] = 255;
            }
        }
        return;
    }
    
    rasterizeGlyphContours(glyph, output, scale);
    
    int pixelCount = 0;
    for (uint8_t pixel : output.bitmap) {
        if (pixel > 0) pixelCount++;
    }
    

}

void TTFFontRenderer::renderText(Canvas* canvas, const std::string& text, 
    int x, int y, int fontSize, uint32_t color) {
    int currentX = x;
    
    for (char c : text) {
        if (c == ' ') {
            currentX += fontSize / 2;
            continue;
        }
        
        RasterizedGlyph glyph = rasterizeGlyph(c, fontSize);
     
        // Render glyph bitmap to canvas
        int pixelsRendered = 0;
        for (int gy = 0; gy < glyph.height; gy++) {
            for (int gx = 0; gx < glyph.width; gx++) {
                uint8_t alpha = glyph.bitmap[gy * glyph.width + gx];
                if (alpha > 0) {
                    int px = currentX + gx + glyph.bearingX;
                    int py = y - glyph.bearingY + gy;
                    
                    if (px >= 0 && px < canvas->getWidth() && 
                        py >= 0 && py < canvas->getHeight()) {
                        
                        // Simple alpha blending
                        if (alpha == 255) {
                            canvas->setPixel(px, py, color);
                        } else {
                            uint32_t existing = canvas->getPixel(px, py);
                            uint32_t blended = blendPixel(existing, color, alpha);
                            canvas->setPixel(px, py, blended);
                        }
                        pixelsRendered++;
                    }
                }
            }
        }
        
        
        currentX += glyph.advance;
    }
}

int TTFFontRenderer::getTextWidth(const std::string& text, int fontSize) {
    int totalWidth = 0;
    
    for (char c : text) {
        if (c == ' ') {
            totalWidth += fontSize / 2;
            continue;
        }
        
        RasterizedGlyph glyph = rasterizeGlyph(c, fontSize);
        totalWidth += glyph.advance;
    }
    
    return totalWidth;
}

int TTFFontRenderer::getTextHeight(int fontSize) {
    return fontSize;
}

int TTFFontRenderer::getLineHeight(int fontSize) {
    return static_cast<int>(fontSize * 1.2f);
}

std::string TTFFontRenderer::getFontName() const {
    return "TTF Font";
}

uint32_t TTFFontRenderer::blendPixel(uint32_t background, uint32_t foreground, uint8_t alpha) {
    if (alpha == 0) return background;
    if (alpha == 255) return foreground;
    
    float a = alpha / 255.0f;
    float invA = 1.0f - a;
    
    uint8_t bgR = (background >> 16) & 0xFF;
    uint8_t bgG = (background >> 8) & 0xFF;
    uint8_t bgB = background & 0xFF;
    uint8_t bgA = (background >> 24) & 0xFF;
    
    uint8_t fgR = (foreground >> 16) & 0xFF;
    uint8_t fgG = (foreground >> 8) & 0xFF;
    uint8_t fgB = foreground & 0xFF;
    uint8_t fgA = (foreground >> 24) & 0xFF;
    
    uint8_t outR = static_cast<uint8_t>(fgR * a + bgR * invA);
    uint8_t outG = static_cast<uint8_t>(fgG * a + bgG * invA);
    uint8_t outB = static_cast<uint8_t>(fgB * a + bgB * invA);
    uint8_t outA = std::max(bgA, fgA);
    
    return (outA << 24) | (outR << 16) | (outG << 8) | outB;
}

bool TTFFontManager::loadFont(const std::string& name, const std::string& fontPath) {
    try {
        auto renderer = std::make_unique<TTFFontRenderer>(fontPath);
        fonts_[name] = std::move(renderer);
        
        if (defaultFontName_.empty()) {
            defaultFontName_ = name;
        }
        
        return true;
    } catch (const std::runtime_error& e) {
       
        return false;
    }
}

TTFFontRenderer* TTFFontManager::getFont(const std::string& name) {
    auto it = fonts_.find(name);
    return (it != fonts_.end()) ? it->second.get() : nullptr;
}

void TTFFontManager::setDefaultFont(const std::string& name) {
    if (fonts_.find(name) != fonts_.end()) {
        defaultFontName_ = name;
    }
}

TTFFontRenderer* TTFFontManager::getDefaultFont() {
    if (!defaultFontName_.empty()) {
        return getFont(defaultFontName_);
    }
    return nullptr;
}

void TTFFontManager::clearAllCaches() {
    for (auto& fontPair : fonts_) {
        fontPair.second->clearCache();
    }
}

void TTFFontRenderer::rasterizeGlyphContours(const SimpleGlyph& glyph, RasterizedGlyph& output, float scale) {

    size_t pointIndex = 0;
    
    for (size_t contour = 0; contour < glyph.endPtsOfContours.size(); contour++) {
        size_t startPt = pointIndex;
        size_t endPt = glyph.endPtsOfContours[contour];
        
      
        std::vector<Point2D> contourPoints = generateHighQualityContour(glyph, startPt, endPt, scale);
        
        
        rasterizeContourLine(contourPoints, output);
        
        pointIndex = endPt + 1;
    }
    
    fillInterior(output);
}

std::vector<TTFFontRenderer::Point2D> TTFFontRenderer::generateHighQualityContour(
    const SimpleGlyph& glyph, size_t startPt, size_t endPt, float scale) {
    
    std::vector<Point2D> contourPoints;
    
    if (startPt > endPt || endPt >= glyph.points.size()) return contourPoints;
    
    auto transformPoint = [&](const TTFPoint& pt) -> Point2D {
        float x = (pt.x - glyph.header.xMin) * scale + 2; 
        float y = (glyph.header.yMax - pt.y) * scale + 2;
        return {x, y};
    };
    
    const auto& firstPoint = glyph.points[startPt];
    Point2D startPos = transformPoint(firstPoint);
    contourPoints.push_back(startPos);
    
    for (size_t i = startPt; i <= endPt; i++) {
        size_t nextIndex = (i == endPt) ? startPt : i + 1; 
        
        const auto& currentPt = glyph.points[i];
        const auto& nextPt = glyph.points[nextIndex];
        
        Point2D nextPos = transformPoint(nextPt);
        
        if (nextPt.onCurve) {
            contourPoints.push_back(nextPos);
        } else {
            size_t endIndex = (nextIndex == endPt) ? startPt : nextIndex + 1;
            if (endIndex > endPt) endIndex = startPt;
            
            const auto& endPoint = glyph.points[endIndex];
            Point2D endPos;
            
            if (endPoint.onCurve) {
                endPos = transformPoint(endPoint);
                i++; 
            } else {
                Point2D impliedPoint;
                impliedPoint.x = (nextPt.x + endPoint.x) / 2.0f;
                impliedPoint.y = (nextPt.y + endPoint.y) / 2.0f;
                endPos = transformPoint({static_cast<int16_t>(impliedPoint.x), static_cast<int16_t>(impliedPoint.y), true});
            }
            
            Point2D currentPos = contourPoints.back();
            Point2D controlPos = transformPoint(nextPt);
            
            int resolution = std::max(6, static_cast<int>(scale * 6));
            for (int j = 1; j <= resolution; j++) {
                float t = static_cast<float>(j) / resolution;
                Point2D curvePoint = quadraticBezier(currentPos, controlPos, endPos, t);
                contourPoints.push_back(curvePoint);
            }
        }
    }
    
    return contourPoints;
}

void TTFFontRenderer::rasterizeContourLine(const std::vector<Point2D>& points, RasterizedGlyph& output) {
    if (points.size() < 2) return;
    
    for (size_t i = 0; i < points.size() - 1; i++) {
        drawLine(points[i], points[i + 1], output);
    }
    
    if (points.size() > 2) {
        drawLine(points.back(), points.front(), output);
    }
}

void TTFFontRenderer::fillInterior(RasterizedGlyph& output) {
    for (int y = 0; y < output.height; y++) {
        std::vector<int> crossings;
        
        for (int x = 0; x < output.width; x++) {
            bool currentPixel = output.bitmap[y * output.width + x] > 0;
            
            if (currentPixel) {
                bool isVerticalCrossing = false;
                
                // Check pixel above
                if (y > 0) {
                    bool pixelAbove = output.bitmap[(y - 1) * output.width + x] > 0;
                    if (!pixelAbove) {
                        isVerticalCrossing = true;
                    }
                }
                
                // Check pixel below
                if (y < output.height - 1) {
                    bool pixelBelow = output.bitmap[(y + 1) * output.width + x] > 0;
                    if (!pixelBelow) {
                        isVerticalCrossing = true;
                    }
                }
                
                if (y == 0 || y == output.height - 1) {
                    isVerticalCrossing = true;
                }
                
                bool isIsolatedPixel = true;
                if (x > 0 && output.bitmap[y * output.width + (x - 1)] > 0) {
                    isIsolatedPixel = false;
                }
                if (x < output.width - 1 && output.bitmap[y * output.width + (x + 1)] > 0) {
                    isIsolatedPixel = false;
                }
                
                if (isVerticalCrossing || isIsolatedPixel) {
                    crossings.push_back(x);
                }
            }
        }
        
        std::vector<int> filteredCrossings;
        for (size_t i = 0; i < crossings.size(); i++) {
            bool isConsecutive = false;
            if (i > 0 && crossings[i] == crossings[i-1] + 1) {
                isConsecutive = true;
            }
            if (i < crossings.size() - 1 && crossings[i] == crossings[i+1] - 1) {
                isConsecutive = true;
            }
            
            if (!isConsecutive) {
                filteredCrossings.push_back(crossings[i]);
            }
        }
        
        // Fill between pairs of crossings
        for (size_t i = 0; i + 1 < filteredCrossings.size(); i += 2) {
            for (int x = filteredCrossings[i] + 1; x < filteredCrossings[i + 1]; x++) {
                if (x >= 0 && x < output.width) {
                    output.bitmap[y * output.width + x] = 255;
                }
            }
        }
    }
}

void TTFFontRenderer::drawLine(const Point2D& p1, const Point2D& p2, RasterizedGlyph& output) {
    // Bresenham's line algorithm with bounds checking
    int x0 = static_cast<int>(std::round(p1.x));
    int y0 = static_cast<int>(std::round(p1.y));
    int x1 = static_cast<int>(std::round(p2.x));
    int y1 = static_cast<int>(std::round(p2.y));
    
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;
    
    int x = x0, y = y0;
    int pixelsDrawn = 0;
    
    while (true) {
        // Set pixel if within bounds
        if (x >= 0 && x < output.width && y >= 0 && y < output.height) {
            output.bitmap[y * output.width + x] = 255;
            pixelsDrawn++;
        }
        
        if (x == x1 && y == y1) break;
        
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }
}

void TTFFontRenderer::fillContour(RasterizedGlyph& output) {
    // Simple scanline fill algorithm 
    for (int y = 0; y < output.height; y++) {
        bool inside = false;
        for (int x = 0; x < output.width; x++) {
            int idx = y * output.width + x;
            if (output.bitmap[idx] > 0) {
                inside = !inside; // Toggle when hitting outline
            } else if (inside) {
                output.bitmap[idx] = 255; // Fill interior
            }
        }
    }
}

TTFFontRenderer::Point2D TTFFontRenderer::quadraticBezier(const Point2D& start, const Point2D& control, const Point2D& end, float t) {
    // Linear interpolation helper
    auto lerp = [](const Point2D& p1, const Point2D& p2, float t) -> Point2D {
        return {p1.x + t * (p2.x - p1.x), p1.y + t * (p2.y - p1.y)};
    };
    
    Point2D p01 = lerp(start, control, t);
    Point2D p12 = lerp(control, end, t);
    return lerp(p01, p12, t);
}

} // namespace Fern