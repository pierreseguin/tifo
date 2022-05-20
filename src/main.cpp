#include "../includes/image.hh"
#include "../includes/utils.hh"

int main(int argc, char *argv[])
{
    if (argc != 4)
        throw std::invalid_argument("Expected 3 arguments");

    Image media = Image(argv[1], Image::COLOR_RGB);
    Message msg = Message(argv[2]);
    int n_bits = 4;

    Image hidden = media.LSBR(msg, n_bits);
    hidden.save("hidden.png");

    Message recovered = hidden.LSBR_recover(n_bits, msg.payload);
    recovered.save(argv[3]);

    bool media_res = media.chi_test(n_bits, msg.payload);
    std::cout << "media is stegano ? " << (media_res ? "yes" : "no")
              << std::endl;

    bool hidden_res = hidden.chi_test(n_bits, msg.payload);
    std::cout << "hidden is stegano ? " << (hidden_res ? "yes" : "no")
              << std::endl;
}
