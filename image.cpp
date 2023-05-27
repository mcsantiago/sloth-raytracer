//
// Created by Matthew on 5/16/23.
//


#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "include/stb_image_write.h"
#include "image.h"

Image::Image(int width, int height, Channel channels)
        : width(width), height(height), channels(channels)
{
    data = new uint32_t[width * height];
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
    int index = (cy * width + cx);
    data[index] = 0 + (color.r << 24) + (color.g << 16) + (color.b << 8) + (color.a);
}

void Image::flipVertically() {
    int rowSize = width * static_cast<int>(channels);
    auto tempRow = new byte[rowSize];

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

uint32_t* Image::getData() const {
    return this->data;
}

void Image::writeToDiskAsPNG(const std::string &filename)  {
    int size = width * height * static_cast<int>(channels);
    unsigned char converted_data[size];
    for (int index = 0; index < width * height; index++) {
        int data_idx = index*4;
        converted_data[data_idx + 0] = (data[index] >> 24) & 255;
        converted_data[data_idx + 1] = (data[index] >> 16) & 255;
        converted_data[data_idx + 2] = (data[index] >> 8) & 255;
        converted_data[data_idx + 3] = data[index] & 255;
    }
    stbi_write_png(filename.c_str(), width, height, channels, converted_data, width * channels);
}
