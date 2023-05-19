//
// Created by Matthew on 5/16/23.
//


#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "include/stb_image_write.h"
#include "image.h"

Image::Image(int width, int height, Channel channels)
        : width(width), height(height), channels(channels)
{
    data = new unsigned char[width * height * static_cast<int>(channels)];
}

Image::~Image() {
    delete [] data;
}

int Image::getWidth() const {
    return width;
}

int Image::getHeight() const {
    return height;
}

void Image::setPixel(int x, int y, Color color) {
    if (x <= -1*width / 2 || x >= width / 2 || y <= -1*height / 2 || y >= height / 2) {
        return;
    }
    int cx = x - (width / 2);
    int cy = (height / 2) - y;
    int index = (cy * width + cx) * channels;
    data[index + 0] = color.r;
    data[index + 1] = color.g;
    data[index + 2] = color.b;
    data[index + 3] = color.a;
}

void Image::flipVertically() {
    int rowSize = width * static_cast<int>(channels);
    auto tempRow = new unsigned char[rowSize];

    for (int y = 0; y < height / 2; ++y) {
        int topRowIndex = y * rowSize;
        int bottomRowIndex = (height - 1 - y) * rowSize;

        // Swap rows using a temporary row buffer
        std::memcpy(tempRow, data + topRowIndex, rowSize);
        std::memcpy(data + topRowIndex, data + bottomRowIndex, rowSize);
        std::memcpy(data + bottomRowIndex, tempRow, rowSize);
    }

    delete[] tempRow;
}

void Image::writeToDiskAsPNG(const std::string &filename)  {
    stbi_write_png(filename.c_str(), width, height, channels, data, width * channels);
}