//
// Created by Matthew on 5/16/23.
//

#ifndef SLOTH_RAYTRACER_IMAGE_H
#define SLOTH_RAYTRACER_IMAGE_H

#include <string>

struct Color {
    Color(int r, int g, int b, int a)
    : r(r), g(g), b(b), a(a) {}

    unsigned int r;
    unsigned int g;
    unsigned int b;
    unsigned int a;
};

enum Channel {
    RGB = 3,
    RGBA = 4
};

class Image {
private:
    int width, height;
    Channel channels;
    unsigned char* data;

public:
    Image(int width, int height, Channel channels);
    ~Image();

    int getWidth() const;
    int getHeight() const;

    void setPixel(int x, int y, Color color);

    void flipVertically();
    void writeToDiskAsPNG(const std::string &filename);
};

#endif //SLOTH_RAYTRACER_IMAGE_H
