#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

typedef unsigned char byte;
typedef std::vector<byte> bytes;

class Message
{
public:
    Message(const std::string &filename);
    Message(int payload);
    Message(int payload, byte value);
    void save(const std::string &filename);

    // Utils

    byte &operator[](int index)
    {
        return content[index];
    }
    byte operator[](int index) const
    {
        return content[index];
    }

public:
    int payload;
    bytes content;
};
