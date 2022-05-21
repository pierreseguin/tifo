#include "../includes/jsteg_image.hh"
#include "../includes/utils.hh"

void test_LSBR(const Image &media, const Message &msg, char *recover_name)
{
    std::cout << "LSB Replacement testing:" << std::endl;
    int n_bits = 1;
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

void test_Jsteg(const Image &media, const Message &msg, char *recover_name)
{
    std::cout << "Jsteg testing:" << std::endl;

    JstegImage hidden = media.Jsteg(msg);
    hidden.save("hidden.png");
    hidden.save_as_jsteg("hidden.jsteg");
    Message recovered = hidden.Jsteg_recover(msg.payload);
    recovered.save(recover_name);

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
    if (argc != 5)
        throw std::invalid_argument("Expected 4 arguments");

    Image media = Image(argv[2], Image::COLOR_RGB);
    Message msg = Message(argv[3]);

    if (std::string("LSBR").compare(argv[1]) == 0)
        test_LSBR(media, msg, argv[4]);
    if (std::string("LSBM").compare(argv[1]) == 0)
        test_LSBM(media, msg, argv[4]);
    if (std::string("Jsteg").compare(argv[1]) == 0)
        test_Jsteg(media, msg, argv[4]);

    media.save("test.png", 1);
}
