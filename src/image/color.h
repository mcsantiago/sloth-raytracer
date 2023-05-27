/*
 * Created by Matthew 5/27/2023
 */

#ifndef IMAGE_COLOR_H
#define IMAGE_COLOR_H

#include <algorithm>

class Color {
public:
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;

    Color() : r(0), g(0), b(0), a(0) {}
    Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : r(r), g(g), b(b), a(a) {}
    Color(unsigned char r, unsigned char g, unsigned char b) : r(r), g(g), b(b), a(255) {}

    Color CombineColor(Color other, float color_factor=0.5) const {
        return {
            static_cast<unsigned char>(std::min(int((r * (1. - color_factor)) + ((float)other.r * color_factor)), 255)),
            static_cast<unsigned char>(std::min(int((g * (1. - color_factor)) + ((float)other.g * color_factor)), 255)),
            static_cast<unsigned char>(std::min(int((b * (1. - color_factor)) + ((float)other.b * color_factor)), 255)),
            255
        };
    }

    Color MultiplyByIntensity(float intensity, bool include_alpha=false) const {
        return {
            static_cast<unsigned char>(std::min((int)((float)r * intensity), 255)),
            static_cast<unsigned char>(std::min((int)((float)g * intensity), 255)),
            static_cast<unsigned char>(std::min((int)((float)b * intensity), 255)),
            (include_alpha) ? static_cast<unsigned char>(std::min((int)((float)a * intensity), 255)) : a
        };
    }
};

#endif // IMAGE_COLOR_H
