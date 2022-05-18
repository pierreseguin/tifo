#include "../includes/image.hh"

Image Image::LSB_replace(const Image &msg, int n_bits) const
{
    Image result = Image(*this);
    for (int i = 0; i < size; i++)
    {
        result[i] &= 0b1111111 << n_bits;
        result[i] |= msg[i] >> (8 - n_bits);
    }
    return result;
}
