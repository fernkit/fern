/**
 * @file primitives.hpp
 * @brief Low-level drawing primitives for the Fern framework
 * 
 * This file provides basic drawing functions for rendering shapes, lines, and fills.
 * These functions operate directly on the canvas and are used by higher-level widgets.
 * 
 * All drawing operations use 32-bit RGBA color values in the format 0xAABBGGRR.
 */

#pragma once

#include "../core/canvas.hpp"
#include <cstdint>

namespace Fern {
    /**
     * @namespace Draw
     * @brief Low-level drawing operations
     * 
     * The Draw namespace contains all primitive drawing functions for rendering
     * basic shapes and graphics. These functions provide the foundation for all
     * higher-level UI widgets and graphics operations.
     */
    namespace Draw {
        /**
         * @brief Fill the entire canvas with a solid color
         * 
         * @param color 32-bit RGBA color value (0xAABBGGRR format)
         * 
         * @example
         * @code
         * Draw::fill(Colors::Black);        // Fill with black
         * Draw::fill(0xFF0000FF);          // Fill with red
         * Draw::fill(0x80FFFFFF);          // Fill with semi-transparent white
         * @endcode
         */
        void fill(uint32_t color);
        
        /**
         * @brief Draw a filled rectangle
         * 
         * @param x Left edge x-coordinate
         * @param y Top edge y-coordinate 
         * @param width Rectangle width in pixels
         * @param height Rectangle height in pixels
         * @param color 32-bit RGBA color value
         * 
         * @example
         * @code
         * Draw::rect(100, 50, 200, 100, Colors::Blue);
         * @endcode
         */
        void rect(int x, int y, int width, int height, uint32_t color);
        
        /**
         * @brief Draw a filled rounded rectangle
         * 
         * @param x Left edge x-coordinate
         * @param y Top edge y-coordinate
         * @param width Rectangle width in pixels  
         * @param height Rectangle height in pixels
         * @param radius Corner radius in pixels
         * @param color 32-bit RGBA color value
         * 
         * @example
         * @code
         * Draw::roundedRect(50, 50, 150, 75, 10, Colors::Green);
         * @endcode
         */
        void roundedRect(int x, int y, int width, int height, int radius, uint32_t color);
        
        /**
         * @brief Draw a rounded rectangle border/outline
         * 
         * @param x Left edge x-coordinate
         * @param y Top edge y-coordinate
         * @param width Rectangle width in pixels
         * @param height Rectangle height in pixels  
         * @param radius Corner radius in pixels
         * @param borderWidth Border thickness in pixels
         * @param color 32-bit RGBA color value
         * 
         * @example
         * @code
         * Draw::roundedRectBorder(10, 10, 100, 50, 5, 2, Colors::White);
         * @endcode
         */
        void roundedRectBorder(int x, int y, int width, int height, int radius, int borderWidth, uint32_t color);
        
        /**
         * @brief Draw a filled circle
         * 
         * @param cx Center x-coordinate
         * @param cy Center y-coordinate
         * @param radius Circle radius in pixels
         * @param color 32-bit RGBA color value
         * 
         * @example
         * @code
         * Draw::circle(200, 150, 25, Colors::Red);
         * @endcode
         */
        void circle(int cx, int cy, int radius, uint32_t color);
        
        /**
         * @brief Draw a line between two points
         * 
         * @param x1 Start point x-coordinate
         * @param y1 Start point y-coordinate
         * @param x2 End point x-coordinate
         * @param y2 End point y-coordinate
         * @param thickness Line thickness in pixels
         * @param color 32-bit RGBA color value
         * 
         * @example
         * @code
         * Draw::line(0, 0, 100, 100, 3, Colors::Yellow);
         * @endcode
         */
        void line(int x1, int y1, int x2, int y2, int thickness, uint32_t color);
    }
}