#include "../includes/image.hh"
#include "../includes/utils.hh"

Image Image::LSBR(const Message &msg, int n_bits) const
{
    Image hidden = Image(*this);
    if (hidden.size < (msg.payload * 8) / n_bits + 1)
        throw std::invalid_argument("Message is too large for the media");

    int lsb_mask = 0b11111111 >> (8 - n_bits);
    int msb_mask = 0b11111111 << n_bits;
    for (int bit = 0; bit < msg.payload * 8; bit += n_bits)
    {
        int hid_pos = bit / n_bits, msg_pos = bit / 8, remaining = 8 - bit % 8;
        int to_write;

        int additional_shift = n_bits - remaining;
        if (additional_shift > 0)
        {
            to_write = msg[msg_pos] << additional_shift;
            if (msg_pos + 1 < msg.payload)
                to_write |= msg[msg_pos + 1] >> (8 - additional_shift);
        }
        else
            to_write = msg[msg_pos] >> (remaining - n_bits);

        hidden[hid_pos] &= msb_mask;
        hidden[hid_pos] |= to_write & lsb_mask;
    }
    return hidden;
}

Message Image::LSBR_recover(int n_bits, int payload) const
{
    Message msg = Message(payload);

    int lsb_mask = 0b11111111 >> (8 - n_bits);
    for (int bit = 0; bit < payload * 8; bit += n_bits)
    {
        int hid_pos = bit / n_bits, msg_pos = bit / 8, remaining = 8 - bit % 8;
        int to_write = pixels[hid_pos] & lsb_mask;

        int additional_shift = n_bits - remaining;
        if (additional_shift > 0)
        {
            msg[msg_pos] |= to_write >> additional_shift;
            to_write = (lsb_mask >> remaining) & to_write;
            if (msg_pos + 1 < payload)
                msg[msg_pos + 1] |= to_write << (8 - additional_shift);
        }
        else
            msg[msg_pos] |= to_write << (remaining - n_bits);
    }
    return msg;
}

bool Image::chi_test(int n_bits, int payload) const
{
    float chi_squared = 0.0;
    unsigned long hist[256];
    for (int i = 0; i < 256; i++)
        hist[i] = 0;
    for (int i = 0; i < payload / n_bits; i++)
        hist[pixels[i]]++;

    for (int color = 0; color < 256; color += 2)
    {
        float nb_expected = (hist[color] + hist[color + 1]) / 2.0;
        if (nb_expected > 0)
            chi_squared += std::pow(hist[color] - nb_expected, 2) / nb_expected;
    }

    chi_squared = (chi_squared * 2 * n_bits) / payload;
    return chi_squared < 0.15;
}
