#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "message.hh"

typedef unsigned char px;
typedef std::vector<px> px_buffer;

class Image
{
public:
    static const int COLOR_RGB = 1;
    static const int GRAYSCALE = 2;

public:
    Image(const std::string &filename, int mode);
    Image(int width, int height, unsigned char *pixels, int mode);
    Image(const Image &img);
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

    Image LSBR(const Message &msg, int n_bits) const;
    Message LSBR_recover(int n_bits, int payload) const;
    bool chi_test(int n_bits, int payload) const;

public:
    int width;
    int height;
    int size;
    int mode;
    px_buffer pixels;
};
