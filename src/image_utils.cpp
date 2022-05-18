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
    this->width = width;
    this->height = height;
    size = width * height * 3;
    pixels = px_buffer(size);

    for (int i = 0; i < size; i++)
        pixels[i] = bytes[i];
}

Image::Image(const Image &img)
{
    width = img.width;
    height = img.height;
    size = img.size;
    pixels = px_buffer(img.pixels);
}

void Image::save(char *filename)
{
    auto bytes = new uint8_t[size];
    for (int i = 0; i < size; i++)
        bytes[i] = pixels[i];

    stbi_write_png(filename, width, height, 3, bytes, 3 * width);
    delete[] bytes;
}

void Image::for_each(std::function<px(px)> func)
{
    std::for_each(pixels.begin(), pixels.end(), func);
}
