#ifndef __FRAMEBUFFER_HPP__
#define __FRAMEBUFFER_HPP__

#include <cstdint>
#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <setjmp.h>
#include <stdexcept>
#include <unistd.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include "image.hpp"
#include "structs.hpp"

class FrameBuffer {
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    char *buffer;
    long int screen_size;
    int fbfd;

  public:
    FrameBuffer() {}
    FrameBuffer(const char *device);
    ~FrameBuffer();

    void draw_pixel(Point point, Color color);
    void draw_rectangle(Rect rect, Color color);
    void draw_image(Point point, const Image &image);
};

#endif