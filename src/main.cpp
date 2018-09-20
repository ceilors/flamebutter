#include "framebuffer.hpp"

int main() {
    FrameBuffer fb = FrameBuffer("/dev/fb0");
    Image i = Image("test-img.png");
    fb.draw_image({10, 10}, i);
}
