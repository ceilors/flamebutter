#include "window.hpp"
#include "image.hpp"
#include "structs.hpp"
#include "font.hpp"

Font f = Font("resource/FiraMono-Regular.ttf", 50);
Image i = Image("resource/test-img.png");
Point pos = {0, 0};

void keyboard(Window * w, char key) {
    switch (key) {
        case 'w': {
            pos.y -= 5;
            break;
        }
        case 's': {
            pos.y += 5;
            break;
        }
        case 'a': {
            pos.x -= 5;
            break;
        }
        case 'd': {
            pos.x += 5;
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
    fb.draw_rectangle({pos.x - 5, pos.y - 5, pos.x + i.width + 5, pos.y + i.height + 5}, {0, 0, 0});
    fb.draw_image(pos, i);
    f.render(fb, {10, 500}, "OMG! It's FreeType!");
}

int main() {
    Window w;

    w.register_keyboard(keyboard);
    w.register_render(render);

    w.main_loop();
}
