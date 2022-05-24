#include "../includes/image.hh"
#include "../includes/utils.hh"

void test_LSBR(const Image &media, const Message &msg, std::string media_ext,
               std::string msg_ext)
{
    std::cout << "LSB Replacement testing:" << std::endl;
    int n_bits = 1;

    Image hidden = media.LSBR(msg, n_bits);
    hidden.save("results/lsbr_results/hidden" + media_ext);
    Message recovered = hidden.LSBR_recover(n_bits, msg.payload);
    recovered.save("results/lsbr_results/recovered" + msg_ext);

    bool media_res = media.chi_test(n_bits, msg.payload);
    std::cout << "\tMedia is stegano ? " << (media_res ? "yes" : "no")
              << std::endl;
    bool hidden_res = hidden.chi_test(n_bits, msg.payload);
    std::cout << "\tHidden is stegano ? " << (hidden_res ? "yes" : "no")
              << std::endl;

    hidden.save("results/lsbr_results/lsbs" + media_ext, n_bits);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
        throw std::invalid_argument("Expected 2 arguments");

    Image media = Image(argv[1], Image::COLOR_RGB);
    Message msg = Message(argv[2]);
    std::string media_name = argv[1];
    std::string msg_name = argv[2];

    int ext_position = media_name.find_last_of(".");
    std::string media_ext = media_name.substr(ext_position);
    ext_position = msg_name.find_last_of(".");
    std::string msg_ext = msg_name.substr(ext_position);
    test_LSBR(media, msg, media_ext, msg_ext);

    media.save("results/media_lsbs" + media_ext, 1);
}
