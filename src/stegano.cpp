#include "../includes/image.hh"

Image Image::LSBR(const Message &msg, int n_bits) const
{
    Image hidden = Image(*this);

    bytes keep_mask(pixels.size());
    bytes leave_mask(pixels.size());
    leave_mask = ~leave_mask;
    for (int bit = 0; bit < n_bits; bit++)
    {
        keep_mask[bit] = 1;
        keep_mask[7 - bit] = 0;
    }

    bytes msg_content = msg.content;
    msg_content.resize(pixels.size());
    for (int i = 0; i < msg.payload; i++)
    {
        bytes lsb = keep_mask & msg_content;
        bytes msb = leave_mask & hidden.pixels;
        hidden.pixels = msb | lsb >> (i * (8 - n_bits) + 8);
    }
    return hidden;
}

/*Image Image::LSBR_recover(int n_bits, int payload) const
{
    Image msg = Image(*this);
    for (int i = 0; i < size; i++)
        msg[i] <<= (8 - n_bits);

    return msg;
}*/

bool Image::chi_test() const
{
    float chi_squared = 0.0;
    unsigned long hist[256];
    for (int i = 0; i < 256; i++)
        hist[i] = 0;
    for (int i = 0; i < size; i++)
        hist[pixels[i]]++;

    for (int color = 0; color < 256; color += 2)
    {
        float nb_expected = (hist[color] + hist[color + 1]) / 2.0;
        if (nb_expected > 0)
            chi_squared += std::pow(hist[color] - nb_expected, 2) / nb_expected;
    }

    chi_squared = (chi_squared * 2) / size;
    std::cout << "chi: " << chi_squared << std::endl;
    return chi_squared > 0.5;
}
