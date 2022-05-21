#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <vector>

#include "message.hh"

typedef unsigned char px;
typedef std::vector<px> px_buffer;
typedef std::vector<int> dct_coefs;

class JstegImage;

class Image
{
public:
    static const int COLOR_RGB = 1;
    static const int GRAYSCALE = 2;

public:
    Image(){};
    Image(int width, int height, px_buffer pixels, int mode)
        : width(width)
        , height(height)
        , size(pixels.size())
        , pixels(pixels)
        , mode(mode){};
    Image(int width, int height, int mode)
        : Image(width, height,
                px_buffer(width * height * (2 * (mode == COLOR_RGB) + 1)),
                mode){};
    Image(const Image &img)
        : Image(img.width, img.height, img.pixels, img.mode){};

    Image(const std::string &filename, int mode);
    Image(int width, int height, unsigned char *pixels, int mode);
    void save(const std::string &filename, int n_lsbs);
    void save(const std::string &filename);

    // Utils

    void print_chars() const;
    px &operator[](int index)
    {
        return pixels[index];
    }
    px operator[](int index) const
    {
        return pixels[index];
    }

    // Steganography methods

    // LSB replacement
    Image LSBR(const Message &msg, int n_bits) const;
    Message LSBR_recover(int n_bits, int payload) const;
    bool chi_test(int n_bits, int payload) const;

    // LSB matching
    Image LSBM(const Message &msg) const;
    Message LSBM_recover(int payload) const;

    // Jsteg
    JstegImage Jsteg(const Message &msg) const;
    dct_coefs compute_dct() const;

private:
    int compute_dct_coef(int x, int y) const;

public:
    int width;
    int height;
    int size;
    int mode;
    px_buffer pixels;
};
