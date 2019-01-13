#ifndef __STRUCTS_HPP__
#define __STRUCTS_HPP__

#include <cstdint>

struct Point {
    uint32_t x;
    uint32_t y;

    Point() : x(0), y(0) {}
    Point(uint32_t _x, uint32_t _y) : x(_x), y(_y) {}
};

struct Rect {
    Point p1;
    Point p2;

    Rect(uint32_t _x1, uint32_t _y1, uint32_t _x2, uint32_t _y2) : p1(Point(_x1, _y1)), p2(Point(_x2, _y2)) {}
    Rect(Point _p1, Point _p2) : p1(_p1), p2(_p2) {}

    bool operator == (const Rect & b) {
        return p1.x == b.p1.x && p1.y == b.p1.y && p2.x == b.p2.x && p2.y == b.p2.y;
    }
};

struct Color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;

    Color(uint8_t r, uint8_t g, uint8_t b) : red(r), green(g), blue(b), alpha(255) {}
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : red(r), green(g), blue(b), alpha(a) {}

    // TODO: write normal blend function
    Color blend(uint8_t r, uint8_t g, uint8_t b) const {
        float a = alpha / 255.0;
        r = (uint8_t)((float)red * a + (float)r * (1.0 - a));
        g = (uint8_t)((float)green * a + (float)b * (1.0 - a));
        b = (uint8_t)((float)blue * a + (float)g * (1.0 - a));
        return Color(r, g, b);
    }
};

#endif