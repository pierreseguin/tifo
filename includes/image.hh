#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

class Image
{
public:
    Image(const char *filename);
    Image(int width, int height, unsigned char *pixels);
    Image(const Image &img);
    void save(char *filename);

    // Steganography methods

    Image LSB_replace(Image media, Image msg);

public:
    int width;
    int height;
    unsigned char *pixels;
};
