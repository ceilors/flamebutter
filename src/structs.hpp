#ifndef __STRUCTS_HPP__
#define __STRUCTS_HPP__
#include <cstdint>

struct Point {
    int32_t x;
    int32_t y;

    Point() : x(0), y(0) {}
    Point(int32_t _x, int32_t _y) : x(_x), y(_y) {}
};

struct Rect {
    Point p1;
    Point p2;

    Rect(int32_t _x1, int32_t _y1, int32_t _x2, int32_t _y2) : p1(Point(_x1, _y1)), p2(Point(_x2, _y2)) {}
    Rect(Point _p1, Point _p2) : p1(_p1), p2(_p2) {}

    bool operator==(const Rect &b) { return p1.x == b.p1.x && p1.y == b.p1.y && p2.x == b.p2.x && p2.y == b.p2.y; }
};

struct Color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;

    Color() : red(0), green(0), blue(0), alpha(0) {}
    Color(uint8_t r, uint8_t g, uint8_t b) : red(r), green(g), blue(b), alpha(255) {}
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : red(r), green(g), blue(b), alpha(a) {}

    void blend(uint8_t r, uint8_t g, uint8_t b) {
        red = (uint8_t)((red * alpha + r * (255 - alpha)) / 255);
        green = (uint8_t)((green * alpha + g * (255 - alpha)) / 255);
        blue = (uint8_t)((blue * alpha + b * (255 - alpha)) / 255);
    }
};

#endif