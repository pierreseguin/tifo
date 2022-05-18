#include "../includes/image.hh"

int main(int argc, char *argv[])
{
    if (argc < 3)
        return 1;

    Image img = Image(argv[1]);
    img.save(argv[2]);
}
