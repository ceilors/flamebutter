#include <cstdint>
#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <linux/fb.h>
#include <png.h>
#include <setjmp.h>
#include <stdexcept>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

struct Point {
    uint32_t x;
    uint32_t y;

    Point(uint32_t _x, uint32_t _y) : x(_x), y(_y) {}
};

struct Rect {
    Point p1;
    Point p2;

    Rect(uint32_t _x1, uint32_t _y1, uint32_t _x2, uint32_t _y2) : p1(Point(_x1, _y1)), p2(Point(_x2, _y2)) {}
    Rect(Point _p1, Point _p2) : p1(_p1), p2(_p2) {}
};

struct Color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;

    Color(uint8_t r, uint8_t g, uint8_t b) : red(r), green(g), blue(b) {}
};

class Image {
  public:
    Image(const char *image);
    Color get_pixel(uint32_t x, uint32_t y) const;
    ~Image();

    png_byte *raw;
    uint32_t width;
    uint32_t height;
    uint8_t color_channels;
};

class FrameBuffer {
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    char *buffer;
    long int screen_size;
    int fbfd;

  public:
    FrameBuffer(const char *device);
    ~FrameBuffer();
    void draw_pixel(Point point, Color color);
    void draw_rectangle(Rect rect, Color color);
    void draw_image(Point point, const Image &image);
};