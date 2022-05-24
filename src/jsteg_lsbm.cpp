#include "../includes/jsteg_image.hh"
#include "../includes/utils.hh"

void test_Jsteg_LSBM(const Image &media, const Message &msg,
                     std::string media_ext, std::string msg_ext)
{
    std::cout << "Jsteg LSBM testing:" << std::endl;

    JstegImage jsteg_media = JstegImage(media);
    JstegImage hidden = jsteg_media.Jsteg_LSBM(msg);
    hidden.save_as_jsteg("results/jsteg_lsbm_results/hidden.jsteg");
    Message recovered = hidden.Jsteg_recover(msg.payload);
    recovered.save("results/jsteg_lsbm_results/recovered" + msg_ext);

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

    hidden.save("results/jsteg_lsbm_results/lsbs" + media_ext);
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
    test_Jsteg_LSBM(media, msg, media_ext, msg_ext);

    media.save("results/media_lsbs" + media_ext, 1);
}
