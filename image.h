//
// Created by Matthew on 5/16/23.
//

#ifndef SLOTH_RAYTRACER_IMAGE_H
#define SLOTH_RAYTRACER_IMAGE_H

#ifndef __cpp_lib_byte
typedef unsigned char byte;
#endif

#include <string>

struct Color {
    Color(byte r, byte g, byte b, byte a)
    : r(r), g(g), b(b), a(a) {}

    byte r;
    byte g;
    byte b;
    byte a;
};

enum Channel {
    RGB = 3,
    RGBA = 4
};

class Image {
private:
    int width, height;
    Channel channels;
    uint32_t* data;

public:
    Image(int width, int height, Channel channels);
    ~Image();

    int getWidth() const;
    int getHeight() const;
    void setPixel(int x, int y, Color color);
    uint32_t* getData() const;

    void flipVertically();
    void writeToDiskAsPNG(const std::string &filename);
};

#endif //SLOTH_RAYTRACER_IMAGE_H
