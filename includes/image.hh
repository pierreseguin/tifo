#pragma once

#include <algorithm>
#include <boost/dynamic_bitset.hpp>
#include <boost/dynamic_bitset/dynamic_bitset.hpp>
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
typedef boost::dynamic_bitset<px> px_buffer;

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

    px get_px(int index) const;
    void set_px(px value, int index);
    void print_chars() const;

    // Steganography methods

    Image LSBR(const Message &msg, int n_bits) const;
    // bytes LSBR_recover(int n_bits) const;
    bool chi_test() const;

public:
    int width;
    int height;
    int size;
    int mode;
    px_buffer pixels;
};
