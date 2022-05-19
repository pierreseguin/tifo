#include "../includes/image.hh"

int main(int argc, char *argv[])
{
    if (argc < 5)
        return 1;

    Image media = Image(argv[1], Image::GRAYSCALE);
    Image msg = Image(argv[2], Image::GRAYSCALE);
    int n_bits = 1;
    msg.print_chars();

    Image hidden = media.LSBR(msg, n_bits);
    hidden.save(argv[3]);

    bool media_chi_result = media.chi_test();
    std::cout << "Media stegano ? " << media_chi_result << std::endl;
    bool msg_chi_result = msg.chi_test();
    std::cout << "Msg stegano ? " << msg_chi_result << std::endl;
    bool hidden_chi_result = hidden.chi_test();
    std::cout << "Hidden stegano ? " << hidden_chi_result << std::endl;

    Image recovered = hidden.LSBR_recover(n_bits);
    std::cout << recovered.size << std::endl;
    recovered.print_chars();
    recovered.save(argv[4]);
}
