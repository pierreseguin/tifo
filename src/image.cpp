#include "../includes/image.hh"

#include "../includes/stb_image.h"
#include "../includes/stb_image_write.h"

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
        pixels = px_buffer(size);

        for (int i = 0; i < size; i++)
            pixels[i] = bytes[i];
    }
    else if (mode == GRAYSCALE)
    {
        size = width * height;
        pixels = px_buffer(size);

        for (int i = 0; i < size * 3; i += 3)
        {
            float gray = bytes[i] / 3. + bytes[i + 1] / 3. + bytes[i + 2] / 3.;
            pixels[i / 3] = (px)(gray);
        }
    }
}

void Image::save(const std::string &filename, int n_lsbs)
{
    uint8_t *bytes;
    if (mode == COLOR_RGB)
    {
        bytes = new uint8_t[size];

        for (int i = 0; i < size; i++)
            bytes[i] = pixels[i] << (8 - n_lsbs);
    }
    else if (mode == GRAYSCALE)
    {
        bytes = new uint8_t[size * 3];

        for (int i = 0; i < size * 3; i++)
            bytes[i] = pixels[i / 3] << (8 - n_lsbs);
    }

    stbi_write_png(filename.c_str(), width, height, 3, bytes, 3 * width);
    delete[] bytes;
}

void Image::save(const std::string &filename)
{
    save(filename, 8);
}

void Image::print_chars() const
{
    for (int i = 0; i < size; i++)
        std::cout << pixels[i];
}
