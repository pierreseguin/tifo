#include "../includes/jsteg_image.hh"
#include "../includes/utils.hh"

const float inv_sqrt_of_2 = 1 / std::sqrt(2);

int Image::compute_dct_coef(int x, int y) const
{
    int nb_channel = 2 * (mode == COLOR_RGB) + 1;

    float sum = 0;
    int block_y = y - y % 8;
    int block_x = x - x % (8 * nb_channel) + x % nb_channel;
    int ry = y % 8;
    int rx = (x / nb_channel) % 8;

    for (int i = block_y; i < block_y + 8; i++)
    {
        for (int j = block_x; j < block_x + 8 * nb_channel; j += nb_channel)
        {
            int ri = i % 8, rj = (j / nb_channel) % 8;
            float i_factor = std::cos((2 * ri + 1) * ry * M_PI / (2. * 8));
            float j_factor = std::cos((2 * rj + 1) * rx * M_PI / (2. * 8));

            sum += pixels[i * width * nb_channel + j] * i_factor * j_factor;
        }
    }
    float Cx = rx == 0 ? inv_sqrt_of_2 : 1;
    float Cy = ry == 0 ? inv_sqrt_of_2 : 1;

    return std::round(2.0 / 8 * Cx * Cy * sum);
}

dct_coefs Image::compute_dct() const
{
    int nb_channel = 2 * (mode == COLOR_RGB) + 1;

    dct_coefs dct(size);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width * nb_channel; x++)
            dct[y * width * nb_channel + x] = compute_dct_coef(x, y);
    }
    return dct;
}

px JstegImage::compute_idct_px(int x, int y) const
{
    int nb_channel = 2 * (mode == COLOR_RGB) + 1;

    float sum = 0;
    int block_y = y - y % 8;
    int block_x = x - x % (8 * nb_channel) + x % nb_channel;
    int ry = y % 8;
    int rx = (x / nb_channel) % 8;

    for (int i = block_y; i < block_y + 8; i++)
    {
        for (int j = block_x; j < block_x + 8 * nb_channel; j += nb_channel)
        {
            int ri = i % 8, rj = (j / nb_channel) % 8;
            float Ci = ri == 0 ? inv_sqrt_of_2 : 1;
            float Cj = rj == 0 ? inv_sqrt_of_2 : 1;

            float i_factor = Ci * std::cos((2 * ry + 1) * ri * M_PI / (2. * 8));
            float j_factor = Cj * std::cos((2 * rx + 1) * rj * M_PI / (2. * 8));

            sum += dct[i * width * nb_channel + j] * i_factor * j_factor;
        }
    }

    return std::clamp(std::round(2.0 / 8 * sum), 0., 255.);
}

px_buffer JstegImage::compute_idct() const
{
    int nb_channel = 2 * (mode == COLOR_RGB) + 1;

    px_buffer pixels(size);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width * nb_channel; x++)
            pixels[y * width * nb_channel + x] = compute_idct_px(x, y);
    }
    return pixels;
}

JstegImage Image::Jsteg_LSBR(const Message &msg) const
{
    dct_coefs dct = compute_dct();
    dct_coefs hidden_dct = ::LSBR(dct, msg, 1);
    return JstegImage(*this, hidden_dct);
}

JstegImage JstegImage::Jsteg_LSBR(const Message &msg) const
{
    dct_coefs hidden_dct = ::LSBR(dct, msg, 1);
    return JstegImage(*this, hidden_dct);
}

Message JstegImage::Jsteg_recover(int payload) const
{
    return ::LSBR_recover(dct, 1, payload);
}

JstegImage Image::Jsteg_LSBM(const Message &msg) const
{
    dct_coefs dct = compute_dct();
    dct_coefs hidden_dct = ::LSBM(dct, msg);
    return JstegImage(*this, hidden_dct);
}

JstegImage JstegImage::Jsteg_LSBM(const Message &msg) const
{
    dct_coefs hidden_dct = ::LSBM(dct, msg);
    return JstegImage(*this, hidden_dct);
}

bool JstegImage::dct_chi_test(int payload) const
{
    int nb_zeros = 0, nb_ones = 0;
    for (int i = 0; i < payload; i++)
    {
        nb_zeros += dct[i] == 0;
        nb_ones += dct[i] == 1;
    }

    float nb_expected = (nb_zeros + nb_ones) / 2.0;
    if (nb_expected <= 0)
        return true;

    float dev = std::pow(nb_zeros - nb_expected, 2);
    float chi_squared = dev / nb_expected;
    chi_squared = (chi_squared * 2) / payload;

    return chi_squared < 0.15;
}
