#include "../includes/jsteg_image.hh"
#include "../includes/utils.hh"

void test_Jsteg(const Image &media, const Message &msg, char *recover_name)
{
    std::cout << "Jsteg testing:" << std::endl;

    JstegImage jsteg_media = JstegImage(media);
    JstegImage hidden = jsteg_media.Jsteg_LSBM(msg);
    hidden.save_as_jsteg("hidden.jsteg");
    Message recovered = hidden.Jsteg_recover(msg.payload);
    recovered.save(recover_name);

    std::cout << "\tChi attack on pixels:" << std::endl;
    bool media_res = media.chi_test(1, msg.payload);
    std::cout << "\t\tMedia is stegano ? " << (media_res ? "yes" : "no")
              << std::endl;
    bool hidden_res = hidden.chi_test(1, msg.payload);
    std::cout << "\t\tHidden is stegano ? " << (hidden_res ? "yes" : "no")
              << std::endl;

    std::cout << "\tChi attack on low dct coefs:" << std::endl;
    media_res = jsteg_media.dct_chi_test(msg.payload);
    std::cout << "\t\tMedia is stegano ? " << (media_res ? "yes" : "no")
              << std::endl;
    hidden_res = hidden.dct_chi_test(msg.payload);
    std::cout << "\t\tHidden is stegano ? " << (hidden_res ? "yes" : "no")
              << std::endl;

    hidden.save("jsteg_lsbs.png", 1);
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
