#include "window.hpp"
#include "image.hpp"
#include "structs.hpp"

Image i = Image("test-img.png");
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
    fb.draw_image(pos, i);
}

int main() {
    Window w;

    w.register_keyboard(keyboard);
    w.register_render(render);

    w.main_loop();
}
