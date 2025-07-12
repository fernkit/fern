#include "../../include/fern/graphics/primitives.hpp"
#include <cmath>

extern Fern::Canvas* globalCanvas;

namespace Fern {
    namespace Draw {
        void fill(uint32_t color) {
            if (!globalCanvas) return;
            for (int i = 0; i < globalCanvas->getWidth() * globalCanvas->getHeight(); ++i) {
                globalCanvas->getBuffer()[i] = color;
            }
        }
        
        void rect(int x, int y, int width, int height, uint32_t color) {
            if (!globalCanvas) return;
            
            for (int dx = 0; dx < height; ++dx) {
                for (int dy = 0; dy < width; ++dy) {
                    int px = x + dy;
                    int py = y + dx;
                    if (px >= 0 && px < globalCanvas->getWidth() && 
                        py >= 0 && py < globalCanvas->getHeight()) {
                        globalCanvas->getBuffer()[py * globalCanvas->getWidth() + px] = color;
                    }
                }
            }
        }
        
        void circle(int cx, int cy, int radius, uint32_t color) {
            if (!globalCanvas) return;
            
            for (int x = -radius; x <= radius; x++) {
                for (int y = -radius; y <= radius; y++) {
                    if (x*x + y*y <= radius*radius) {
                        int px = cx + x;
                        int py = cy + y;
                        if (px >= 0 && px < globalCanvas->getWidth() && 
                            py >= 0 && py < globalCanvas->getHeight()) {
                            globalCanvas->getBuffer()[py * globalCanvas->getWidth() + px] = color;
                        }
                    }
                }
            }
        }
        
        void line(int x1, int y1, int x2, int y2, int thickness, uint32_t color) {
            if (!globalCanvas) return;
            
            int dx = std::abs(x2 - x1);
            int dy = std::abs(y2 - y1);
            int sx = x1 < x2 ? 1 : -1;
            int sy = y1 < y2 ? 1 : -1;
            int err = dx - dy;
            
            while (true) {
                circle(x1, y1, thickness, color);
                
                if (x1 == x2 && y1 == y2) break;
                
                int e2 = 2 * err;
                if (e2 > -dy) {
                    err -= dy;
                    x1 += sx;
                }
                if (e2 < dx) {
                    err += dx;
                    y1 += sy;
                }
            }
        }
        
        void roundedRect(int x, int y, int width, int height, int radius, uint32_t color) {
            if (!globalCanvas) return;
            
            // Clamp radius to not exceed half of width or height
            radius = std::min(radius, std::min(width / 2, height / 2));
            
            // Draw the main rectangle (without corners)
            rect(x + radius, y, width - 2 * radius, height, color);
            rect(x, y + radius, radius, height - 2 * radius, color);
            rect(x + width - radius, y + radius, radius, height - 2 * radius, color);
            
            // Draw rounded corners using circles
            // Top-left corner
            for (int dx = -radius; dx <= 0; dx++) {
                for (int dy = -radius; dy <= 0; dy++) {
                    if (dx*dx + dy*dy <= radius*radius) {
                        int px = x + radius + dx;
                        int py = y + radius + dy;
                        if (px >= 0 && px < globalCanvas->getWidth() && 
                            py >= 0 && py < globalCanvas->getHeight()) {
                            globalCanvas->getBuffer()[py * globalCanvas->getWidth() + px] = color;
                        }
                    }
                }
            }
            
            // Top-right corner
            for (int dx = 0; dx <= radius; dx++) {
                for (int dy = -radius; dy <= 0; dy++) {
                    if (dx*dx + dy*dy <= radius*radius) {
                        int px = x + width - radius - 1 + dx;
                        int py = y + radius + dy;
                        if (px >= 0 && px < globalCanvas->getWidth() && 
                            py >= 0 && py < globalCanvas->getHeight()) {
                            globalCanvas->getBuffer()[py * globalCanvas->getWidth() + px] = color;
                        }
                    }
                }
            }
            
            // Bottom-left corner
            for (int dx = -radius; dx <= 0; dx++) {
                for (int dy = 0; dy <= radius; dy++) {
                    if (dx*dx + dy*dy <= radius*radius) {
                        int px = x + radius + dx;
                        int py = y + height - radius - 1 + dy;
                        if (px >= 0 && px < globalCanvas->getWidth() && 
                            py >= 0 && py < globalCanvas->getHeight()) {
                            globalCanvas->getBuffer()[py * globalCanvas->getWidth() + px] = color;
                        }
                    }
                }
            }
            
            // Bottom-right corner
            for (int dx = 0; dx <= radius; dx++) {
                for (int dy = 0; dy <= radius; dy++) {
                    if (dx*dx + dy*dy <= radius*radius) {
                        int px = x + width - radius - 1 + dx;
                        int py = y + height - radius - 1 + dy;
                        if (px >= 0 && px < globalCanvas->getWidth() && 
                            py >= 0 && py < globalCanvas->getHeight()) {
                            globalCanvas->getBuffer()[py * globalCanvas->getWidth() + px] = color;
                        }
                    }
                }
            }
        }
        
        void roundedRectBorder(int x, int y, int width, int height, int radius, int borderWidth, uint32_t color) {
            if (!globalCanvas) return;
            
            // Draw outer rounded rect
            roundedRect(x, y, width, height, radius, color);
            
            // Draw inner rounded rect with background color to create border effect
            // This is a simple approach - for better results we'd need a proper border algorithm
            if (width > 2 * borderWidth && height > 2 * borderWidth) {
                // We'll just draw the border as the difference between outer and inner rects
                // Top border
                rect(x + radius, y, width - 2 * radius, borderWidth, color);
                // Bottom border  
                rect(x + radius, y + height - borderWidth, width - 2 * radius, borderWidth, color);
                // Left border
                rect(x, y + radius, borderWidth, height - 2 * radius, color);
                // Right border
                rect(x + width - borderWidth, y + radius, borderWidth, height - 2 * radius, color);
                
                // Corner borders (simplified)
                for (int i = 0; i < borderWidth; i++) {
                    // Top-left
                    circle(x + radius, y + radius, radius - i, color);
                    // Top-right  
                    circle(x + width - radius, y + radius, radius - i, color);
                    // Bottom-left
                    circle(x + radius, y + height - radius, radius - i, color);
                    // Bottom-right
                    circle(x + width - radius, y + height - radius, radius - i, color);
                }
            }
        }
    }
}