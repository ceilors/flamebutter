#include "framebuffer.hpp"

FrameBuffer::FrameBuffer(const char *device) {
    fbfd = open(device, O_RDWR);
    if (fbfd == -1) {
        throw std::runtime_error("cannot open framebuffer device");
    }
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        throw std::runtime_error("error reading fixed information");
    }
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        throw std::runtime_error("error reading variable information");
    }
    screen_size = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
    screen_buffer = (char *)mmap(0, screen_size, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if (*(int *)screen_buffer == -1) {
        throw std::runtime_error("failed to map framebuffer device to memory");
    }
    draw_buffer = new char[screen_size];
}

FrameBuffer::~FrameBuffer() {
    delete[] draw_buffer;
    munmap(screen_buffer, screen_size);
    close(fbfd);
}

void FrameBuffer::draw_pixel(const Point point, Color color) {
    if (point.x < 0 || point.y < 0 || point.x > (int32_t)vinfo.xres || point.y > (int32_t)vinfo.yres) {
        return;
    }
    long int location =
        (point.x + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) + (point.y + vinfo.yoffset) * finfo.line_length;
    if (vinfo.bits_per_pixel == 32) {
        if (color.alpha < 255) {
            color.blend(*(draw_buffer + location + 2), *(draw_buffer + location + 1), *(draw_buffer + location + 0));
        }
        *(draw_buffer + location + 0) = color.blue;
        *(draw_buffer + location + 1) = color.green;
        *(draw_buffer + location + 2) = color.red;
        *(draw_buffer + location + 3) = color.alpha;
    } else {
        unsigned short int c = color.red << 11 | color.green << 5 | color.blue;
        *((unsigned short int *)(draw_buffer + location)) = c;
    }
}

void FrameBuffer::draw_rect(Rect rect, Color color) {
    for (int32_t y = rect.p1.y; y < rect.p2.y; ++y) {
        for (int32_t x = rect.p1.x; x < rect.p2.x; ++x) {
            draw_pixel({x, y}, color);
        }
    }
}

void FrameBuffer::draw_line_low(Rect l, Color color) {
    auto dx = (int)l.p2.x - (int)l.p1.x;
    auto dy = (int)l.p2.y - (int)l.p1.y;
    auto yi = 0;
    if (dy < 0) {
        dy *= -1;
        yi = -1;
    } else {
        yi = 1;
    }
    auto d = (dy << 1) - dx;
    auto y = l.p1.y;
    for (auto x = l.p1.x; x < l.p2.x; x++) {
        draw_pixel({x, y}, color);
        if (d > 0) {
            y += yi;
            d -= dx << 1;
        }
        d += dy << 1;
    }
}

void FrameBuffer::draw_line_high(Rect l, Color color) {
    auto dx = (int)l.p2.x - (int)l.p1.x;
    auto dy = (int)l.p2.y - (int)l.p1.y;
    auto xi = 0;
    if (dx < 0) {
        dx *= -1;
        xi = -1;
    } else {
        xi = 1;
    }
    auto d = (dx << 1) - dy;
    auto x = l.p1.x;
    for (auto y = l.p1.y; y < l.p2.y; y++) {
        draw_pixel({x, y}, color);
        if (d > 0) {
            x += xi;
            d -= dy << 1;
        }
        d += dx << 1;
    }
}

void FrameBuffer::draw_line(Rect l, Color color) {
    if (std::abs((int)l.p2.y - (int)l.p1.y) < std::abs((int)l.p2.x - (int)l.p1.x)) {
        if (l.p1.x > l.p2.x) {
            draw_line_low({l.p2.x, l.p2.y, l.p1.x, l.p1.y}, color);
        } else {
            draw_line_low({l.p1.x, l.p1.y, l.p2.x, l.p2.y}, color);
        }
    } else {
        if (l.p1.y > l.p2.y) {
            draw_line_high({l.p2.x, l.p2.y, l.p1.x, l.p1.y}, color);
        } else {
            draw_line_high({l.p1.x, l.p1.y, l.p2.x, l.p2.y}, color);
        }
    }
}

void FrameBuffer::draw_framed_rect(Rect l, Color color) {
    Point p1 = {l.p1.x, l.p1.y};
    Point p2 = {p1.x + l.p2.x, p1.y};
    Point p3 = {p2.x, p1.y + l.p2.y};
    Point p4 = {p1.x, p3.y};

    draw_line({p1, p2}, color);
    draw_line({p2, p3}, color);
    draw_line({p3, p4}, color);
    draw_line({p4, p1}, color);
}

void FrameBuffer::clear_screen(Color color) { draw_rect({0, 0, (int32_t)vinfo.xres, (int32_t)vinfo.yres}, color); }

void FrameBuffer::swap_buffer() { memcpy(screen_buffer, draw_buffer, screen_size); }