#include "../includes/image.hh"
#include "../includes/utils.hh"

Image Image::LSBR(const Message &msg, int n_bits) const
{
    px_buffer hidden_px = ::LSBR<px>(pixels, msg, n_bits);
    return Image(width, height, hidden_px, mode);
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

Image Image::LSBM(const Message &msg) const
{
    if (size < (msg.payload * 8))
        throw std::invalid_argument("Message is too large for the media");

    srand(time(NULL));
    Image hidden = Image(*this);
    int lsb_mask = 0b00000001;
    for (int bit = 0; bit < msg.payload * 8; bit++)
    {
        int msg_pos = bit / 8, remaining = 7 - bit % 8;
        int msg_bit = (msg[msg_pos] >> remaining) & lsb_mask;

        if (msg_bit != (hidden[bit] & lsb_mask))
        {
            if (hidden[bit] == 255)
                hidden[bit] -= 1;
            else if (hidden[bit] == 0)
                hidden[bit] += 1;
            else
                hidden[bit] += ((rand() % 2) * 2) - 1;
        }
    }
    return hidden;
}

Message Image::LSBM_recover(int payload) const
{
    return LSBR_recover(1, payload);
}

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

std::vector<int> Image::compute_dct() const
{
    int nb_channel = 2 * (mode == COLOR_RGB) + 1;

    std::vector<int> dct(size);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width * nb_channel; x++)
            dct[y * width * nb_channel + x] = compute_dct_coef(x, y);
    }
    return dct;
}

px Image::compute_idct_px(const std::vector<int> &dct, int x, int y)
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

Image Image::compute_idct(const std::vector<int> &dct, int width, int height,
                          int mode)
{
    int nb_channel = 2 * (mode == COLOR_RGB) + 1;

    Image img = Image(width, height, mode);
    for (int y = 0; y < height; y++)
    {
        std::cout << "line " << y + 1 << " / " << height << std::endl;
        for (int x = 0; x < width * nb_channel; x++)
            img[y * width * nb_channel + x] = img.compute_idct_px(dct, x, y);
    }
    return img;
}

Image Image::Jsteg(const Message &msg) const
{
    std::vector<int> dct = compute_dct();
    std::cout << "computed dct" << std::endl;
    std::vector<int> hidden_dct = ::LSBR(dct, msg, 1);
    std::cout << "data hided" << std::endl;
    return compute_idct(hidden_dct, width, height, mode);
}
