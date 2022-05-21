#include "../includes/utils.hh"

#include "../includes/image.hh"

std::string to_binary(char c)
{
    std::string binary;
    for (unsigned i = (1 << (8 - 1)); i > 0; i = i / 2)
        binary += (c & i) ? "1" : "0";

    return binary;
}

std::string to_binary(const std::vector<unsigned char> &vec)
{
    std::string binary;
    for (const unsigned char &c : vec)
    {
        std::string c_binary = to_binary(c);
        binary += c_binary;
    }
    return binary;
}

template <typename T>
std::vector<T> LSBR(const std::vector<T> &media, const Message &msg, int n_bits)
{
    if (media.size() < (msg.payload * 8) / n_bits + 1)
        throw std::invalid_argument("Message is too large for the media");

    std::vector<T> hidden = media;
    T lsb_mask = static_cast<T>(~0b0) >> (8 * sizeof(T) - n_bits);
    T msb_mask = static_cast<T>(~0b0) << n_bits;
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
template std::vector<int> LSBR<int>(const std::vector<int> &, const Message &,
                                    int);
