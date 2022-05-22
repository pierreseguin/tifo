#include "../includes/image.hh"
#include "../includes/utils.hh"

template <typename T>
std::vector<T> LSBR(const std::vector<T> &media, const Message &msg, int n_bits)
{
    if (media.size() < (msg.payload * 8) / n_bits + 1)
        throw std::invalid_argument("Message is too large for the media");

    std::vector<T> hidden = media;
    T msb_mask = static_cast<T>(~0b0) << n_bits;
    T lsb_mask = ~msb_mask;
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
template px_buffer LSBR<px>(const px_buffer &, const Message &, int);
template dct_coefs LSBR<int>(const dct_coefs &, const Message &, int);

template <typename T>
Message LSBR_recover(const std::vector<T> &hidden, int n_bits, int payload)
{
    Message msg = Message(payload);

    T lsb_mask = ~(static_cast<T>(~0b0) << n_bits);
    for (int bit = 0; bit < payload * 8; bit += n_bits)
    {
        int hid_pos = bit / n_bits, msg_pos = bit / 8, remaining = 8 - bit % 8;
        int to_write = hidden[hid_pos] & lsb_mask;

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
template Message LSBR_recover(const px_buffer &hidden, int n_bits, int payload);
template Message LSBR_recover(const dct_coefs &hidden, int n_bits, int payload);

template <typename T>
std::vector<T> LSBM(const std::vector<T> &media, const Message &msg)
{
    if (media.size() < (msg.payload * 8))
        throw std::invalid_argument("Message is too large for the media");

    srand(time(NULL));
    std::vector<T> hidden = media;
    T lsb_mask = 0b1;
    for (int bit = 0; bit < msg.payload * 8; bit++)
    {
        int msg_pos = bit / 8, remaining = 7 - bit % 8;
        int msg_bit = (msg[msg_pos] >> remaining) & lsb_mask;

        if (msg_bit != (hidden[bit] & lsb_mask))
        {
            if (hidden[bit] == std::numeric_limits<T>::max())
                hidden[bit] -= 1;
            else if (hidden[bit] == std::numeric_limits<T>::min())
                hidden[bit] += 1;
            else
                hidden[bit] += ((rand() % 2) * 2) - 1;
        }
    }
    return hidden;
}
template px_buffer LSBM<px>(const px_buffer &, const Message &);
template dct_coefs LSBM<int>(const dct_coefs &, const Message &);

Image Image::LSBR(const Message &msg, int n_bits) const
{
    px_buffer hidden_px = ::LSBR<px>(pixels, msg, n_bits);
    return Image(width, height, hidden_px, mode);
}

Message Image::LSBR_recover(int n_bits, int payload) const
{
    return ::LSBR_recover(pixels, n_bits, payload);
}

bool Image::chi_test(int n_bits, int payload) const
{
    float chi_squared = 0.0;
    unsigned long hist[256];
    for (int i = 0; i < 256; i++)
        hist[i] = 0;
    for (int i = 0; i < payload / n_bits; i++)
        hist[pixels[i]]++;

    int range_size = 1 << n_bits;
    for (int color = 0; color < 256; color++)
    {
        for (int bit = 0; bit < n_bits; bit++)
        {
            int bit_mask = 1 << bit;
            int complementary =
                (~(color & bit_mask) & bit_mask) | (color & ~bit_mask);
            float nb_exp = (hist[color] + hist[complementary]) / 2.;
            if (nb_exp > 0)
                chi_squared += std::pow(hist[color] - nb_exp, 2) / nb_exp;
        }
    }

    chi_squared = chi_squared / payload;
    return chi_squared < 0.15;
}

Image Image::LSBM(const Message &msg) const
{
    px_buffer hidden_px = ::LSBM<px>(pixels, msg);
    return Image(width, height, hidden_px, mode);
}

Message Image::LSBM_recover(int payload) const
{
    return LSBR_recover(1, payload);
}
