#ifndef __IMAGE_HPP__
#define __IMAGE_HPP__
#include <cstdint>
#include <cstdio>
#include <stdexcept>

#include <png.h>

#include "framebuffer.hpp"
#include "structs.hpp"

class Image {
  public:
    virtual ~Image() { delete[] raw; }
    virtual Color get_pixel(uint32_t x, uint32_t y) const {
        uint32_t location = (y * size.x + x) * color_channels;
        return {raw[location + 0], raw[location + 1], raw[location + 2], raw[location + 3]};
    }
    virtual void render(FrameBuffer &fb, const Point &p, bool ignore_alpha = false) {
        for (int32_t y = 0; y < size.y; ++y) {
            for (int32_t x = 0; x < size.x; ++x) {
                Color pixel = get_pixel(x, y);
                if (ignore_alpha && pixel.alpha == 0) {
                    continue;
                }
                fb.draw_pixel({p.x + x, p.y + y}, pixel);
            }
        }
    }

    uint8_t *raw;
    Point size;
    uint8_t color_channels;
};

class PNGImage : public Image {
  public:
    PNGImage(FILE *f);
    PNGImage(const char *image);
};

class RAWImage : public Image {
  public:
    RAWImage(const char *image);
};

#endif