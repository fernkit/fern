/**
 * @file font.hpp
 * @brief Basic bitmap font rendering utilities
 * 
 * This file provides simple bitmap font rendering functions for basic text display.
 * For more advanced typography features including TTF fonts, see the font/ directory.
 */

#pragma once

#include <cstdint>

namespace Fern {
    /**
     * @namespace DrawText
     * @brief Simple bitmap font text rendering functions
     * 
     * The DrawText namespace provides basic text rendering using a built-in
     * bitmap font. These functions are suitable for simple text display,
     * debugging output, and basic UI labels.
     * 
     * @note For advanced typography, TTF fonts, and text styling, use the
     *       TextWidget class and Font system instead.
     */
    namespace DrawText {
        /**
         * @brief Draw a single character using the built-in bitmap font
         * 
         * Renders a single character at the specified position using the
         * built-in monospace bitmap font.
         * 
         * @param c Character to draw (ASCII)
         * @param x X position for the character
         * @param y Y position for the character
         * @param scale Scaling factor (1 = normal size, 2 = double size, etc.)
         * @param color 32-bit RGBA color value
         * 
         * @example
         * @code
         * DrawText::drawChar('A', 100, 50, 2, Colors::White);
         * @endcode
         */
        void drawChar(char c, int x, int y, int scale, uint32_t color);
        
        /**
         * @brief Draw a text string using the built-in bitmap font
         * 
         * Renders a null-terminated string at the specified position using
         * the built-in monospace bitmap font. Characters are placed horizontally
         * with automatic spacing.
         * 
         * @param text Null-terminated string to draw
         * @param x X position for the first character
         * @param y Y position for the text baseline
         * @param scale Scaling factor (1 = normal size, 2 = double size, etc.)
         * @param color 32-bit RGBA color value
         * 
         * @example
         * @code
         * DrawText::drawText("Hello World", 50, 100, 1, Colors::Green);
         * DrawText::drawText("Large Text", 200, 200, 3, Colors::Red);
         * @endcode
         * 
         * @note For multiline text, word wrapping, or advanced formatting,
         *       use TextWidget instead.
         */
        void drawText(const char* text, int x, int y, int scale, uint32_t color);
    }
}