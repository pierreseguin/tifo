#pragma once

#include <algorithm>
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <vector>

typedef unsigned char px;
typedef std::vector<px> px_buffer;

class Image
{
public:
    Image(const char *filename);
    Image(int width, int height, unsigned char *pixels);
    Image(const Image &img);
    void save(char *filename);

    // Utils

    void for_each(std::function<px(px)> func);
    px &operator[](int index)
    {
        return pixels[index];
    }
    px operator[](int index) const
    {
        return pixels[index];
    }

    // Steganography methods

    Image LSB_replace(const Image &msg, int n_bits) const;

public:
    int width;
    int height;
    int size;
    px_buffer pixels;
};
