#include "../includes/image.hh"
#include "../includes/stb_image.h"
#include "../includes/stb_image_write.h"

std::string toBinary(char c)
{
    std::string binary;
    for (unsigned i = (1 << (8 - 1)); i > 0; i = i / 2)
    {
        binary += (c & i) ? "1" : "0";
    }

    return binary;
}

Image::Image(const std::string &filename, int mode)
{
    int width, height, bpp;
    auto *bytes = stbi_load(filename.c_str(), &width, &height, &bpp, 3);
    *this = Image(width, height, bytes, mode);
    stbi_image_free(bytes);
}

Image::Image(int width, int height, unsigned char *bytes, int mode)
{
    this->width = width;
    this->height = height;
    this->mode = mode;

    if (mode == COLOR_RGB)
    {
        int nb_channels = 1 + 2 * (mode == COLOR_RGB);
        size = width * height * nb_channels;
        pixels = px_buffer(size * 8);

        for (int i = 0; i < size; i++)
            set_px(bytes[i], i);
    }
    else if (mode == GRAYSCALE)
    {
        size = width * height;
        pixels = px_buffer(size * 8);

        for (int i = 0; i < size * 3; i += 3)
        {
            float gray = bytes[i] / 3. + bytes[i + 1] / 3. + bytes[i + 2] / 3.;
            set_px((px)gray, i / 3);
        }
    }
}

Image::Image(const Image &img)
{
    width = img.width;
    height = img.height;
    size = img.size;
    mode = img.mode;
    pixels = px_buffer(img.pixels);
}

void Image::save(const std::string &filename)
{
    uint8_t *bytes;
    if (mode == COLOR_RGB)
    {
        bytes = new uint8_t[size];

        for (int i = 0; i < size; i++)
            bytes[i] = get_px(i);
    }
    else if (mode == GRAYSCALE)
    {
        bytes = new uint8_t[size * 3];

        for (int i = 0; i < size * 3; i++)
            bytes[i] = get_px(i / 3);
    }

    stbi_write_png(filename.c_str(), width, height, 3, bytes, 3 * width);
    delete[] bytes;
}

px Image::get_px(int index) const
{
    px value = 0;
    for (int bit = index * 8; bit < index * 8 + 7; bit++)
    {
        value |= pixels[bit];
        value <<= 1;
    }
    value |= pixels[index * 8 + 7];
    return value;
}

void Image::set_px(px value, int index)
{
    for (int bit = index * 8 + 7; bit >= index * 8; bit--)
    {
        pixels[bit] = value % 2;
        value >>= 1;
    }
}

void Image::print_chars() const
{
    for (int i = 0; i < size; i++)
        std::cout << get_px(i);
}
