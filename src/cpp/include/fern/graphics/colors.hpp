#pragma once

#include <cstdint>

namespace Fern {
    namespace Colors {
        // Basic Colors
        constexpr uint32_t Black = 0xFF000000;
        constexpr uint32_t White = 0xFFFFFFFF;
        constexpr uint32_t Red = 0xFFFF0000;
        constexpr uint32_t Green = 0xFF00FF00;
        constexpr uint32_t Blue = 0xFF0000FF;
        constexpr uint32_t Yellow = 0xFFFFFF00;
        constexpr uint32_t Cyan = 0xFF00FFFF;
        constexpr uint32_t Magenta = 0xFFFF00FF;
        
        // Additional color variations
        constexpr uint32_t DarkCyan = 0xFF008B8B;
        constexpr uint32_t DarkMagenta = 0xFF8B008B;
        constexpr uint32_t Teal = 0xFF008080;
        
        // Grayscale
        constexpr uint32_t Gray = 0xFF808080;
        constexpr uint32_t DarkGray = 0xFF404040;
        constexpr uint32_t LightGray = 0xFFC0C0C0;
        constexpr uint32_t Charcoal = 0xFF202020;
        
        // Red Variations
        constexpr uint32_t DarkRed = 0xFF8B0000;
        constexpr uint32_t Crimson = 0xFFDC143C;
        constexpr uint32_t LightRed = 0xFFFF6666;
        constexpr uint32_t Coral = 0xFFFF7F50;
        
        // Green Variations
        constexpr uint32_t DarkGreen = 0xFF006400;
        constexpr uint32_t LightGreen = 0xFF90EE90;
        constexpr uint32_t Lime = 0xFF32CD32;
        constexpr uint32_t Forest = 0xFF228B22;
        constexpr uint32_t Olive = 0xFF808000;
        
        // Blue Variations
        constexpr uint32_t DarkBlue = 0xFF00008B;
        constexpr uint32_t LightBlue = 0xFFADD8E6;
        constexpr uint32_t SkyBlue = 0xFF87CEEB;
        constexpr uint32_t Navy = 0xFF000080;
        constexpr uint32_t Turquoise = 0xFF40E0D0;
        
        // Yellow/Orange
        constexpr uint32_t Orange = 0xFFFFA500;
        constexpr uint32_t Gold = 0xFFFFD700;
        constexpr uint32_t Amber = 0xFFFFBF00;
        
        // Purple/Pink
        constexpr uint32_t Purple = 0xFF800080;
        constexpr uint32_t Violet = 0xFF8A2BE2;
        constexpr uint32_t Pink = 0xFFFFC0CB;
        constexpr uint32_t HotPink = 0xFFFF69B4;
        
        // Brown Tones
        constexpr uint32_t Brown = 0xFFA52A2A;
        constexpr uint32_t Tan = 0xFFD2B48C;
        constexpr uint32_t SaddleBrown = 0xFF8B4513;
        
        // UI Special Colors
        constexpr uint32_t Success = 0xFF28A745;
        constexpr uint32_t Warning = 0xFFFFC107;
        constexpr uint32_t Danger = 0xFFDC3545;
        constexpr uint32_t Info = 0xFF17A2B8;
        constexpr uint32_t Primary = 0xFF007BFF;
        constexpr uint32_t Secondary = 0xFF6C757D;
        
        // Transparent colors (with alpha)
        constexpr uint32_t Transparent = 0x00000000;
        constexpr uint32_t SemiTransparent = 0x80000000;
        
        // Color blending function
        uint32_t blendColors(uint32_t color1, uint32_t color2, float t);
    }
    
    // Rest of your gradient code remains the same
    struct GradientStop {
        uint32_t color;
        float position;  // 0-1
    };
    
    // class LinearGradient {
    // public:
    //     LinearGradient(GradientStop* stops, int stopCount, bool vertical = false);
    //     uint32_t colorAt(float position) const;
        
    //     bool isVertical() const { return vertical_; }
        
    // private:
    //     GradientStop* stops_;
    //     int stopCount_;
    //     bool vertical_;
    // };
}