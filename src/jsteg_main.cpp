#include "../includes/image.hh"
#include "../includes/utils.hh"

void test_Jsteg(const Image &media, const Message &msg, char *recover_name)
{
    std::cout << "Jsteg testing:" << std::endl;

    Image hidden = media.Jsteg(msg);
    std::cout << "Ended hiding" << std::endl;
    hidden.save("hidden.png");

    bool media_res = media.chi_test(1, msg.payload);
    std::cout << "\tMedia is stegano ? " << (media_res ? "yes" : "no")
              << std::endl;
    bool hidden_res = hidden.chi_test(1, msg.payload);
    std::cout << "\tHidden is stegano ? " << (hidden_res ? "yes" : "no")
              << std::endl;

    hidden.save("Jsteg_lsbs.png", 1);
}

int main(int argc, char *argv[])
{
    if (argc != 4)
        throw std::invalid_argument("Expected 4 arguments");

    Image media = Image(argv[1], Image::COLOR_RGB);
    Message msg = Message(argv[2]);

    test_Jsteg(media, msg, argv[3]);

    media.save("test.png", 1);
}
