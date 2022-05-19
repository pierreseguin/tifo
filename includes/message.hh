#pragma once
#include <boost/dynamic_bitset.hpp>
#include <boost/dynamic_bitset/dynamic_bitset.hpp>
#include <fstream>
#include <string>

typedef boost::dynamic_bitset<unsigned char> bytes;

class Message
{
public:
    Message(const std::string &filename);

public:
    int payload;
    bytes content;
};
