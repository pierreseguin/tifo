#pragma once

#include <algorithm>
#include <boost/dynamic_bitset.hpp>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

typedef unsigned char px;
typedef boost::px_buffer;
typedef std::vector<uint8_t> bytes;

class Image
{
public:
    static const int COLOR_RGB = 1;
    static const int GRAYSCALE = 2;
    static const int TXT_IMAGE = 3;

public:
    Image(const std::string &filename, int mode);
    Image(int width, int height, unsigned char *pixels, int mode);
    Image(const Image &img);
    void save(const std::string &filename);

    // Utils

    void print_chars() const;
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

    Image LSBR(const bytes &msg, int n_bits) const;
    text LSBR_recover(int n_bits) const;
    bool chi_test() const;

public:
    int width;
    int height;
    int size;
    int mode;
    px_buffer pixels;
};
