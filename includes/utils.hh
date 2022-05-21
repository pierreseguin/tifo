#pragma once
#include <string>
#include <vector>

#include "message.hh"

std::string to_binary(char c);
std::string to_binary(const std::vector<unsigned char> &vec);

// Steganography functions
template <typename T>
std::vector<T> LSBR(const std::vector<T> &media, const Message &msg,
                    int n_bits);
template <typename T>
Message LSBR_recover(const std::vector<T> &hidden, int n_bits, int payload);
