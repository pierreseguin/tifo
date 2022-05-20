#include "../includes/utils.hh"

std::string to_binary(char c)
{
    std::string binary;
    for (unsigned i = (1 << (8 - 1)); i > 0; i = i / 2)
        binary += (c & i) ? "1" : "0";

    return binary;
}

std::string to_binary(const std::vector<unsigned char> &vec)
{
    std::string binary;
    for (const unsigned char &c : vec)
    {
        std::string c_binary = to_binary(c);
        binary += c_binary;
    }
    return binary;
}
