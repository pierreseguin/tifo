#include "../includes/image.hh"
#include "../includes/stb_image.h"
#include "../includes/stb_image_write.h"

Image::Image(const std::string &filename, int mode)
{
    if (filename.compare(filename.length() - 4, 4, ".txt") == 0)
    {
        std::ifstream input(filename, std::ios::binary);
        std::vector<unsigned char> bytes(
            (std::istreambuf_iterator<char>(input)),
            (std::istreambuf_iterator<char>()));

        int width = std::sqrt(bytes.size());
        int height = (bytes.size() / width) + 1;
        bytes.reserve(width * height);
        *this = Image(width, height, bytes.data(), TXT_IMAGE);
        input.close();
    }
    else
    {
        int width, height, bpp;
        auto *bytes = stbi_load(filename.c_str(), &width, &height, &bpp, 3);
        *this = Image(width, height, bytes, mode);
        stbi_image_free(bytes);
    }
}

Image::Image(int width, int height, unsigned char *bytes, int mode)
{
    this->width = width;
    this->height = height;
    this->mode = mode;

    if (mode == COLOR_RGB || mode == TXT_IMAGE)
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
    if (filename.compare(filename.length() - 4, 4, ".txt") == 0)
    {
        std::ofstream output(filename, std::ios::binary);
        output.write((const char *)pixels.data(), pixels.size());
        output.close();
    }
    else
    {
        uint8_t *bytes;
        if (mode == COLOR_RGB || mode == TXT_IMAGE)
        {
            bytes = new uint8_t[size];

            for (int i = 0; i < size; i++)
                bytes[i] = pixels[i];
        }
        else if (mode == GRAYSCALE)
        {
            bytes = new uint8_t[size * 3];

            for (int i = 0; i < size * 3; i++)
                bytes[i] = pixels[i / 3];
        }

        stbi_write_png(filename.c_str(), width, height, 3, bytes, 3 * width);
        delete[] bytes;
    }
}

void Image::print_chars() const
{
    for (int i = 0; i < size; i++)
        std::cout << pixels[i];
}

void Image::for_each(std::function<px(px)> func)
{
    std::for_each(pixels.begin(), pixels.end(), func);
}
