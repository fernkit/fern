/**
 * @file colors.hpp
 * @brief Predefined color constants and color manipulation utilities
 * 
 * This file provides a comprehensive set of predefined color constants and
 * utilities for color manipulation in the Fern framework. All colors use
 * 32-bit RGBA format (0xAABBGGRR) for consistency across the framework.
 */

#pragma once

#include <cstdint>

namespace Fern {
    /**
     * @namespace Colors
     * @brief Predefined color constants for common UI and graphics needs
     * 
     * The Colors namespace provides a comprehensive set of predefined colors
     * in 32-bit RGBA format (0xAABBGGRR). These colors are carefully chosen
     * to provide good contrast and usability for UI applications.
     * 
     * @example Using predefined colors:
     * @code
     * Draw::fill(Colors::Black);
     * button.setBackgroundColor(Colors::Primary);
     * text.setColor(Colors::White);
     * @endcode
     */
    namespace Colors {
        // Basic Colors - Primary color palette
        constexpr uint32_t Black = 0xFF000000;      ///< Pure black (0, 0, 0)
        constexpr uint32_t White = 0xFFFFFFFF;      ///< Pure white (255, 255, 255)
        constexpr uint32_t Red = 0xFFFF0000;        ///< Pure red (255, 0, 0)
        constexpr uint32_t Green = 0xFF00FF00;      ///< Pure green (0, 255, 0)
        constexpr uint32_t Blue = 0xFF0000FF;       ///< Pure blue (0, 0, 255)
        constexpr uint32_t Yellow = 0xFFFFFF00;     ///< Pure yellow (255, 255, 0)
        constexpr uint32_t Cyan = 0xFF00FFFF;       ///< Pure cyan (0, 255, 255)
        constexpr uint32_t Magenta = 0xFFFF00FF;    ///< Pure magenta (255, 0, 255)
        
        // Additional color variations
        constexpr uint32_t DarkCyan = 0xFF008B8B;      ///< Dark cyan (0, 139, 139)
        constexpr uint32_t DarkMagenta = 0xFF8B008B;   ///< Dark magenta (139, 0, 139)
        constexpr uint32_t Teal = 0xFF008080;          ///< Teal (0, 128, 128)
        
        // Grayscale spectrum
        constexpr uint32_t Gray = 0xFF808080;          ///< Medium gray (128, 128, 128)
        constexpr uint32_t DarkGray = 0xFF404040;      ///< Dark gray (64, 64, 64)
        constexpr uint32_t LightGray = 0xFFC0C0C0;     ///< Light gray (192, 192, 192)
        constexpr uint32_t Charcoal = 0xFF202020;      ///< Very dark gray (32, 32, 32)
        
        // Red color family
        constexpr uint32_t DarkRed = 0xFF8B0000;       ///< Dark red (139, 0, 0)
        constexpr uint32_t Crimson = 0xFFDC143C;       ///< Crimson red (220, 20, 60)
        constexpr uint32_t LightRed = 0xFFFF6666;      ///< Light red (255, 102, 102)
        constexpr uint32_t Coral = 0xFFFF7F50;        ///< Coral (255, 127, 80)
        
        // Green color family
        constexpr uint32_t DarkGreen = 0xFF006400;     ///< Dark green (0, 100, 0)
        constexpr uint32_t LightGreen = 0xFF90EE90;    ///< Light green (144, 238, 144)
        constexpr uint32_t Lime = 0xFF32CD32;          ///< Lime green (50, 205, 50)
        constexpr uint32_t Forest = 0xFF228B22;        ///< Forest green (34, 139, 34)
        constexpr uint32_t Olive = 0xFF808000;         ///< Olive (128, 128, 0)
        
        // Blue color family
        constexpr uint32_t DarkBlue = 0xFF00008B;      ///< Dark blue (0, 0, 139)
        constexpr uint32_t LightBlue = 0xFFADD8E6;     ///< Light blue (173, 216, 230)
        constexpr uint32_t SkyBlue = 0xFF87CEEB;       ///< Sky blue (135, 206, 235)
        constexpr uint32_t Navy = 0xFF000080;          ///< Navy blue (0, 0, 128)
        constexpr uint32_t Turquoise = 0xFF40E0D0;     ///< Turquoise (64, 224, 208)
        
        // Warm colors - Yellow/Orange family
        constexpr uint32_t Orange = 0xFFFFA500;        ///< Orange (255, 165, 0)
        constexpr uint32_t Gold = 0xFFFFD700;          ///< Gold (255, 215, 0)
        constexpr uint32_t Amber = 0xFFFFBF00;         ///< Amber (255, 191, 0)
        
        // Cool colors - Purple/Pink family  
        constexpr uint32_t Purple = 0xFF800080;        ///< Purple (128, 0, 128)
        constexpr uint32_t DarkPurple = 0xFF4B0082;    ///< Dark purple/Indigo (75, 0, 130)
        constexpr uint32_t Violet = 0xFF8A2BE2;        ///< Blue violet (138, 43, 226)
        constexpr uint32_t Pink = 0xFFFFC0CB;          ///< Light pink (255, 192, 203)
        constexpr uint32_t HotPink = 0xFFFF69B4;       ///< Hot pink (255, 105, 180)
        
        // Earth tones - Brown family
        constexpr uint32_t Brown = 0xFFA52A2A;         ///< Brown (165, 42, 42)
        constexpr uint32_t Tan = 0xFFD2B48C;           ///< Tan (210, 180, 140)
        constexpr uint32_t SaddleBrown = 0xFF8B4513;   ///< Saddle brown (139, 69, 19)
        
        // UI semantic colors - For consistent interface design
        constexpr uint32_t Success = 0xFF28A745;       ///< Success green (40, 167, 69)
        constexpr uint32_t Warning = 0xFFFFC107;       ///< Warning yellow (255, 193, 7)
        constexpr uint32_t Danger = 0xFFDC3545;        ///< Danger red (220, 53, 69)
        constexpr uint32_t Info = 0xFF17A2B8;          ///< Info blue (23, 162, 184)
        constexpr uint32_t Primary = 0xFF007BFF;       ///< Primary blue (0, 123, 255)
        constexpr uint32_t Secondary = 0xFF6C757D;     ///< Secondary gray (108, 117, 125)
        
        // Transparency colors
        constexpr uint32_t Transparent = 0x00000000;      ///< Fully transparent
        constexpr uint32_t SemiTransparent = 0x80000000;  ///< 50% transparent black
        
        /**
         * @brief Blend two colors with linear interpolation
         * 
         * Performs alpha blending between two colors using linear interpolation.
         * This is useful for creating smooth color transitions and effects.
         * 
         * @param color1 First color (32-bit RGBA)
         * @param color2 Second color (32-bit RGBA)  
         * @param t Interpolation factor (0.0 = color1, 1.0 = color2)
         * @return uint32_t Blended color result
         * 
         * @example
         * @code
         * uint32_t midColor = Colors::blendColors(Colors::Red, Colors::Blue, 0.5f);
         * uint32_t fadeColor = Colors::blendColors(Colors::White, Colors::Black, fadeAmount);
         * @endcode
         */
        uint32_t blendColors(uint32_t color1, uint32_t color2, float t);
    }
    
    /**
     * @brief Represents a color stop in a gradient
     * 
     * A gradient stop defines a color and its position within a gradient.
     * Multiple stops can be combined to create complex color transitions.
     */
    struct GradientStop {
        uint32_t color;     ///< Color at this stop (32-bit RGBA)
        float position;     ///< Position in gradient (0.0 to 1.0)
    };
    
    // Future gradient support (currently commented out)
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