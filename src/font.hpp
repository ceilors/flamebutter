#ifndef __FONT_HPP__
#define __FONT_HPP__
#include <cstdarg>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "structs.hpp"
#include "util.hpp"

#include "framebuffer.hpp"

class Font {
    FT_Library library;
    FT_Face face;
    FT_GlyphSlot slot;
    Color color;

    bool center_x;

    uint16_t font_dpi;
    uint16_t font_size;
    uint32_t last_y;

    Point render_size(const std::wstring &text);
    void render_line(FrameBuffer &fb, const Point &pos, const std::wstring &text);

  public:
    Font(const char *filename, uint16_t size);
    ~Font();

    void render(FrameBuffer &fb, const Point &pos, const wchar_t *fmt, ...);

    void set_color(const Color &c) { color = c; }
    void set_align(bool s_center_x) { center_x = s_center_x; }
    void reset_y() { last_y = 0; }
};

#endif