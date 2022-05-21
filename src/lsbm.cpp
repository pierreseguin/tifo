#include "../includes/image.hh"
#include "../includes/utils.hh"

void test_LSBM(const Image &media, const Message &msg, char *recover_name)
{
    std::cout << "LSB Matching testing:" << std::endl;

    Image hidden = media.LSBM(msg);
    hidden.save("hidden.png");
    Message recovered = hidden.LSBM_recover(msg.payload);
    recovered.save(recover_name);

    bool media_res = media.chi_test(1, msg.payload);
    std::cout << "\tMedia is stegano ? " << (media_res ? "yes" : "no")
              << std::endl;
    bool hidden_res = hidden.chi_test(1, msg.payload);
    std::cout << "\tHidden is stegano ? " << (hidden_res ? "yes" : "no")
              << std::endl;

    hidden.save("LSBM_lsbs.png", 1);
}

int main(int argc, char *argv[])
{
    if (argc != 4)
        throw std::invalid_argument("Expected 4 arguments");

    Image media = Image(argv[1], Image::COLOR_RGB);
    Message msg = Message(argv[2]);

    test_LSBM(media, msg, argv[3]);

    media.save("test.png", 1);
}
