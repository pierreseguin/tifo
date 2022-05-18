#include "../includes/image.hh"

int main(int argc, char *argv[])
{
    if (argc < 4)
        return 1;

    Image media = Image(argv[1]);
    Image msg = Image(argv[2]);

    Image result = media.LSB_replace(msg, 4);
    result.save(argv[3]);
}
