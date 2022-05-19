#include "../includes/message.hh"

Message::Message(const std::string &filename)
{
    std::ifstream input(filename, std::ios::binary);

    std::vector<unsigned char> chars((std::istreambuf_iterator<char>(input)),
                                     (std::istreambuf_iterator<char>()));
    payload = chars.size();
    content = bytes(chars.begin(), chars.end());

    input.close();
}
