#include "../includes/image.hh"
#include "../includes/stb_image.h"
#include "../includes/stb_image_write.h"

Image::Image(const char *filename)
{
    int width, height, bpp;
    auto *bytes = stbi_load(filename, &width, &height, &bpp, 3);
    *this = Image(width, height, bytes);
    stbi_image_free(bytes);
}

Image::Image(int width, int height, unsigned char *bytes)
{
    r = channel(height, std::vector<unsigned char>(width));
    g = channel(height, std::vector<unsigned char>(width));
    b = channel(height, std::vector<unsigned char>(width));
    this->width = width;
    this->height = height;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            r[i][j] = bytes[i * 3 * width + 3 * j];
            g[i][j] = bytes[i * 3 * width + 3 * j + 1];
            b[i][j] = bytes[i * 3 * width + 3 * j + 2];
        }
    }
}

Image::Image(const Image &img)
{
    width = img.width;
    height = img.height;
    r = img.r;
    g = img.g;
    b = img.b;
}

void Image::save(char *filename)
{
    auto bytes = new uint8_t[3 * width * height];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            bytes[i * 3 * width + j * 3] = r[i][j];
            bytes[i * 3 * width + j * 3 + 1] = g[i][j];
            bytes[i * 3 * width + j * 3 + 2] = b[i][j];
        }
    }

    stbi_write_png(filename, width, height, 3, bytes, 3 * width);
    delete[] bytes;
}
