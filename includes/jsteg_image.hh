#pragma once

#include "image.hh"

class JstegImage : public Image
{
public:
    JstegImage(const Image &img, const dct_coefs &dct)
        : Image(img)
        , dct(dct){};
    JstegImage(const std::string &filename, int mode);
    void save_as_jsteg(const std::string &filename);

    Message Jsteg_recover(int payload) const;
    px_buffer compute_idct() const;

private:
    px compute_idct_px(int x, int y) const;

public:
    dct_coefs dct;
};
