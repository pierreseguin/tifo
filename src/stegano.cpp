#include "../includes/image.hh"

Image Image::LSBR(const bytes &msg, int n_bits) const
{
    Image hidden = Image(*this);
    if (hidden.size < (msg.size() * 8) / n_bits + 1)
        throw std::invalid_argument("Message is too large for the media");

    int lsb_mask = 0b11111111 >> (8 - n_bits);
    int msb_mask = 0b11111111 << n_bits;
    for (int bit = 0; bit < msg.size() * 8 + n_bits; bit += n_bits)
    {
        int hid_pos = bit / n_bits;
        int msg_pos = bit / 8;
        int remaining = bit % 8;
        int to_write = msg[msg_pos] >> (8 - remaining);

        int additional_shift = n_bits - remaining;
        if (additional_shift > 0)
        {
            to_write <<= additional_shift;
            if (msg_pos + 1 < msg.size())
                to_write |= msg[msg_pos + 1] >> (8 - additional_shift);
        }

        hidden[hid_pos] &= msb_mask;
        hidden[hid_pos] |= to_write & lsb_mask;
    }
    return hidden;
}

Image Image::LSBR_recover(int n_bits, int payload) const
{
    Image msg = Image(*this);
    for (int i = 0; i < size; i++)
        msg[i] <<= (8 - n_bits);

    return msg;
}

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
