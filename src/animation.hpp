#ifndef __ANIMATION_HPP__
#define __ANIMATION_HPP__

#include <vector>
#include <chrono>

#include "structs.hpp"
#include "framebuffer.hpp"
#include "image.hpp"

class Animation {
    std::vector<PNGImage *> images = {};
    uint32_t current_frame = 0;
    std::chrono::time_point<std::chrono::system_clock> last_time = std::chrono::system_clock::now();
    void raw_render(FrameBuffer & fb, const Point pos);
public:
    uint32_t width = 0;
    uint32_t height = 0;
    Animation(const char * filename);
    Animation(const char * fmt, uint32_t start, uint32_t stop);
    ~Animation();
    void render(FrameBuffer & fb, const Point pos, long int delay);
    void render(FrameBuffer & fb, const std::vector<Point> vpos, long int delay);
};

#endif