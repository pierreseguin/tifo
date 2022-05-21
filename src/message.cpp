#include "../includes/message.hh"

Message::Message(const std::string &filename)
{
    std::ifstream input(filename, std::ios::binary);
    content = bytes((std::istreambuf_iterator<char>(input)),
                    (std::istreambuf_iterator<char>()));
    payload = content.size();

    input.close();
}

Message::Message(int payload)
{
    this->payload = payload;
    content = bytes(payload);
}

Message::Message(int payload, byte value)
{
    *this = Message(payload);
    for (byte &b : content)
        b = value;
}

void Message::save(const std::string &filename)
{
    std::ofstream output(filename, std::ios::binary);
    output.write((char *)content.data(), payload);

    output.close();
}
