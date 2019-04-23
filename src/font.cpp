#include "font.hpp"

Font::Font(const char *filename, uint16_t size) {
    font_dpi = 100;
    font_size = size;
    // align params
    center_x = false;
    last_y = 0;

    FT_Init_FreeType(&library);
    FT_New_Face(library, filename, 0, &face);
    FT_Set_Char_Size(face, size * size, 0, font_dpi, 0);
}

Font::~Font() {
    FT_Done_Face(face);
    FT_Done_FreeType(library);
}

Point Font::render_size(const std::wstring &text) {
    Point npos;
    slot = face->glyph;
    for (auto c : text) {
        // load
        FT_UInt glyph_index = FT_Get_Char_Index(face, c);
        if (FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT)) {
            continue;
        }
        // move
        npos.x += (slot->metrics.horiAdvance >> 6);
        npos.y = std::max(npos.y, (int32_t)(slot->metrics.vertAdvance >> 6));
    }
    return npos;
}

void Font::render_line(FrameBuffer &fb, const Point &pos, const std::wstring &text) {
    Point npos = pos;
    slot = face->glyph;
    for (auto c : text) {
        // load
        FT_UInt glyph_index = FT_Get_Char_Index(face, c);
        if (FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT)) {
            continue;
        }
        FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL);
        // draw
        for (uint32_t y = 0; y < slot->bitmap.rows; y++) {
            for (uint32_t x = 0; x < slot->bitmap.width; x++) {
                uint8_t value = slot->bitmap.buffer[y * slot->bitmap.width + x];
                Color curr_c(value * color.red / 255, value * color.green / 255, value * color.blue / 255, value);
                Point curr_p(npos.x + x + slot->bitmap_left,
                             npos.y + y + (slot->metrics.vertAdvance >> 6) - slot->bitmap_top);
                fb.draw_pixel(curr_p, curr_c);
            }
        }
        // move
        npos.x += (slot->metrics.horiAdvance >> 6);
    }
}

void Font::render(FrameBuffer &fb, const Point &pos, const wchar_t *fmt, ...) {
    // prepare text
    wchar_t text[1024] = {};
    va_list arg;
    va_start(arg, fmt);
    vswprintf(text, sizeof(text), fmt, arg);
    va_end(arg);

    std::wstring buffer(text);
    str_index indexes = split_lines(buffer);
    Point fbs = fb.get_size();
    for (uint16_t i = 0; i < indexes.size() - 1; i += 2) {
        std::wstring tmp(indexes[i + 0], indexes[i + 1]);
        if (tmp.size() > 0) {
            Point size = render_size(tmp);
            Point render_pos;
            if (center_x) {
                render_pos.x = (fbs.x - size.x) >> 1;
                render_pos.y = last_y;
            } else {
                render_pos.x = pos.x;
                render_pos.y = last_y;
            }
            render_line(fb, render_pos, tmp);
            last_y += size.y;
        } else {
            last_y += font_size;
        }
    }
}