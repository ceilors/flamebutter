#include "framebuffer.hpp"

FrameBuffer::FrameBuffer(const char *device) {
    fbfd = open(device, O_RDWR);
    if (fbfd == -1) {
        std::runtime_error("cannot open framebuffer device");
    }
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        std::runtime_error("error reading fixed information");
    }
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        std::runtime_error("error reading variable information");
    }
    screen_size = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
    buffer = (char *)mmap(0, screen_size, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if (*(int *)buffer == -1) {
        std::runtime_error("failed to map framebuffer device to memory");
    }
}

FrameBuffer::~FrameBuffer() {
    munmap(buffer, screen_size);
    close(fbfd);
}

void FrameBuffer::draw_pixel(Point point, Color color) {
    long int location =
        (point.x + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) + (point.y + vinfo.yoffset) * finfo.line_length;
    if (vinfo.bits_per_pixel == 32) {
        *(buffer + location + 0) = color.blue;
        *(buffer + location + 1) = color.green;
        *(buffer + location + 2) = color.red;
        // no transparency
        *(buffer + location + 3) = 0;
    } else {
        unsigned short int c = color.red << 11 | color.green << 5 | color.blue;
        *((unsigned short int *)(buffer + location)) = c;
    }
}

void FrameBuffer::draw_rectangle(Rect rect, Color color) {
    for (uint32_t y = rect.p1.y; y < rect.p2.y; ++y) {
        for (uint32_t x = rect.p1.x; x < rect.p2.x; ++x) {
            draw_pixel({x, y}, color);
        }
    }
}

void FrameBuffer::draw_image(Point point, const Image &image) {
    for (uint32_t y = 0; y < image.height; ++y) {
        for (uint32_t x = 0; x < image.width; ++x) {
            draw_pixel({point.x + x, point.y + y}, image.get_pixel(x, y));
        }
    }
}