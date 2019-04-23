#include "animation.hpp"
#include "font.hpp"
#include "image.hpp"
#include "structs.hpp"
#include "window.hpp"

const uint16_t font_size = 50;
Font f = Font("resource/FiraMono-Regular.ttf", font_size);
PNGImage i = PNGImage("resource/test-img.png");
PNGImage n = PNGImage("resource/test-img-2.png");
Animation a = Animation("resource/rick.pngz");
Point ani_pos = {10, 10};
Point fnt_pos = {a.size.x + 20, a.size.y + font_size + 10};
Point img_p_pos = {10, a.size.y + 40};
Point img_pos = {10, a.size.y + 40};
Rect fnt_bak = {fnt_pos.x, 10, fnt_pos.x + 350, 300};

void keyboard(Window *w, char key) {
    switch (key) {
    case 'w': {
        img_pos.y -= 5;
        break;
    }
    case 's': {
        img_pos.y += 5;
        break;
    }
    case 'a': {
        img_pos.x -= 5;
        break;
    }
    case 'd': {
        img_pos.x += 5;
        break;
    }
    case 'q': {
        w->send_close();
        break;
    }
    default: {
        break;
    }
    }
}

void render(FrameBuffer &fb) {
    fb.draw_rect({img_pos.x - 5, img_pos.y - 5, img_pos.x + i.size.x + 5, img_pos.y + i.size.y + 5}, {0, 0, 0});
    fb.draw_rect(fnt_bak, Color(50, 50, 50));
    i.render(fb, img_p_pos);
    n.render(fb, img_pos, false);
    wchar_t app_info_fmt[] = L"Multiline\nstring\nдля теста\n%d";
    f.set_color({255, 127, 0});
    f.render(fb, fnt_pos, app_info_fmt, 42);
    f.reset_y();
    a.render(fb, ani_pos, 25);
}

int main() {
    Window w;

    w.register_keyboard(keyboard);
    w.register_render(render);

    w.main_loop();
}