#include "jsteg_image.hh"

JstegImage::JstegImage(const Image &img)
{
    dct_coefs img_dct = img.compute_dct();
    *this = JstegImage(img, img_dct);
}

JstegImage::JstegImage(const std::string &filename, int mode)
{
    std::ifstream input(filename, std::ios::binary);

    std::string header;
    getline(input, header);
    width = std::stoi(header);
    getline(input, header);
    height = std::stoi(header);
    this->mode = mode;
    size = width * height * 3;

    std::string coef;
    dct = dct_coefs();
    while (std::getline(input, coef))
        dct.push_back(std::stoi(coef));
    pixels = compute_idct();

    if (mode == GRAYSCALE)
    {
        for (int i = 0; i < size; i += 3)
        {
            float gray =
                pixels[i] / 3. + pixels[i + 1] / 3. + pixels[i + 2] / 3.;
            pixels[i / 3] = (px)(gray);
        }
        size = width * height;
        pixels.resize(size);
    }

    input.close();
}

void JstegImage::save_as_jsteg(const std::string &filename)
{
    std::ofstream output(filename, std::ios::binary);

    output << width << "\n" << height << "\n";
    if (mode == COLOR_RGB)
    {
        for (int i = 0; i < size; i++)
            output << dct[i] << "\n";
    }
    else
    {
        for (int i = 0; i < size; i++)
            output << dct[i] << "\n" << dct[i] << "\n" << dct[i] << "\n";
    }

    output.close();
}
