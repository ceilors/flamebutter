#ifndef __IMAGE_HPP__
#define __IMAGE_HPP__

#include <cstdio>
#include <cstdint>
#include <stdexcept>
#include <png.h>
#include "structs.hpp"

class Image {
public:
    virtual ~Image() { delete[] raw; }
    virtual Color get_pixel(uint32_t x, uint32_t y) const {
        uint32_t location = (y * width + x) * color_channels;
        return {raw[location + 0], raw[location + 1], raw[location + 2], raw[location + 3]};
    }

    uint8_t * raw;
    uint32_t width;
    uint32_t height;
    uint8_t color_channels;
};

class PNGImage : public Image {
public:
    PNGImage(FILE * f);
    PNGImage(const char * image);
};

class RAWImage : public Image {
public:
    RAWImage(const char * image);
};

#endif