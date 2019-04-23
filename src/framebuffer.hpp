#ifndef __FRAMEBUFFER_HPP__
#define __FRAMEBUFFER_HPP__
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>

#include <fcntl.h>
#include <linux/fb.h>
#include <setjmp.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

#include "structs.hpp"

class FrameBuffer {
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    char *draw_buffer;
    char *screen_buffer;
    long int screen_size;
    int fbfd;
    uint8_t buffer_index = 0;

    void draw_line_low(Rect l, Color color);
    void draw_line_high(Rect l, Color color);

  public:
    FrameBuffer(const char *device);
    ~FrameBuffer();

    Point get_size() { return Point(vinfo.xres, vinfo.yres); }

    void draw_pixel(const Point point, Color color);
    void draw_rect(Rect rect, Color color);
    void draw_line(Rect l, Color color);
    void draw_framed_rect(Rect l, Color color);

    void clear_screen(Color color);

    void swap_buffer();
};

#endif