#include "../../include/fern/graphics/colors.hpp"

namespace Fern {
    namespace Colors {
        uint32_t blendColors(uint32_t color1, uint32_t color2, float t) {
            uint8_t r1 = (color1 >> 16) & 0xFF;
            uint8_t g1 = (color1 >> 8) & 0xFF;
            uint8_t b1 = color1 & 0xFF;
            
            uint8_t r2 = (color2 >> 16) & 0xFF;
            uint8_t g2 = (color2 >> 8) & 0xFF;
            uint8_t b2 = color2 & 0xFF;
            
            uint8_t r = r1 + (r2 - r1) * t;
            uint8_t g = g1 + (g2 - g1) * t;
            uint8_t b = b1 + (b2 - b1) * t;
            
            return 0xFF000000 | (r << 16) | (g << 8) | b;
        }
    }
    
    // LinearGradient::LinearGradient(GradientStop* stops, int stopCount, bool vertical)
    //     : stops_(stops), stopCount_(stopCount), vertical_(vertical) {}
    
    // uint32_t LinearGradient::colorAt(float position) const {
    //     if (position <= stops_[0].position) return stops_[0].color;
    //     if (position >= stops_[stopCount_-1].position) return stops_[stopCount_-1].color;
        
    //     for (int i = 0; i < stopCount_ - 1; i++) {
    //         if (position >= stops_[i].position && position <= stops_[i+1].position) {
    //             float local_pos = (position - stops_[i].position) / 
    //                              (stops_[i+1].position - stops_[i].position);
    //             return Colors::blendColors(stops_[i].color, stops_[i+1].color, local_pos);
    //         }
    //     }
    //     return 0xFF000000;
    // }
}