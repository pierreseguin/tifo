#include "../includes/image.hh"
#include "../includes/utils.hh"

void test_LSBR(const Image &media, const Message &msg, char *recover_name)
{
    std::cout << "LSB Replacement testing:" << std::endl;
    int n_bits = 4;
    Image hidden = media.LSBR(msg, n_bits);
    hidden.save("hidden.png");
    Message recovered = hidden.LSBR_recover(n_bits, msg.payload);
    recovered.save(recover_name);

    bool media_res = media.chi_test(n_bits, msg.payload);
    std::cout << "\tMedia is stegano ? " << (media_res ? "yes" : "no")
              << std::endl;
    bool hidden_res = hidden.chi_test(n_bits, msg.payload);
    std::cout << "\tHidden is stegano ? " << (hidden_res ? "yes" : "no")
              << std::endl;

    hidden.save("LSBR_lsbs.png", n_bits);
}

int main(int argc, char *argv[])
{
    if (argc != 4)
        throw std::invalid_argument("Expected 4 arguments");

    Image media = Image(argv[1], Image::COLOR_RGB);
    Message msg = Message(argv[2]);

    test_LSBR(media, msg, argv[3]);

    media.save("test.png", 1);
}
