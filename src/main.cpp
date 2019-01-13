#include "window.hpp"
#include "image.hpp"
#include "structs.hpp"
#include "font.hpp"
#include "animation.hpp"

const uint16_t font_size = 50;
Font f = Font("resource/FiraMono-Regular.ttf", font_size);
PNGImage i = PNGImage("resource/test-img.png");
Animation a = Animation("resource/rick.pngz");
Point ani_pos = {10, 10};
Point fnt_pos = {10, a.height + font_size + 10};
Point img_pos = {10, fnt_pos.y + font_size};

void keyboard(Window * w, char key) {
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

void render(FrameBuffer & fb) {
    fb.draw_rectangle({img_pos.x - 5, img_pos.y - 5, img_pos.x + i.width + 5, img_pos.y + i.height + 5}, {0, 0, 0});
    fb.draw_image(img_pos, &i);
    f.render(fb, fnt_pos, "OMG! It's FreeType!");
    a.render(fb, ani_pos, 25);
}

int main() {
    Window w;

    w.register_keyboard(keyboard);
    w.register_render(render);

    w.main_loop();
}
