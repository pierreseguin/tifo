#include "../includes/image.hh"

int main(int argc, char *argv[])
{
    if (argc < 3)
        return 1;

    Image media = Image(argv[1], Image::GRAYSCALE);
    Message msg = Message(argv[2]);
    int n_bits = 1;

    Image hidden = media.LSBR(msg, n_bits);
    hidden.save("hidden.png");
}
