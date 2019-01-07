#include "animation.hpp"

Animation::Animation(const char *fmt, uint32_t start, uint32_t stop) {
    char img_name[128];
    images.reserve(stop - start + 1);
    for (uint32_t index = start; index <= stop; index++) {
        sprintf(img_name, fmt, index);
        Image * img = new Image(img_name);
        images.push_back(img);
    }
}

Animation::~Animation() {
    for (auto & i : images) {
        delete i;
    }
}

void Animation::raw_render(FrameBuffer & fb, Point pos) {
    fb.draw_image(pos, images[current_frame]);
}

void Animation::render(FrameBuffer & fb, Point pos, long int delay) {
    auto current_time = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = current_time - last_time;
    std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(diff);
    if (duration.count() > delay) {
        last_time = current_time;
        raw_render(fb, pos);
        current_frame += 1;
        if (current_frame >= images.size()) {
            current_frame = 0;
        }
    }
}

void Animation::render(FrameBuffer & fb, std::vector<Point> vpos, long int delay) {
    auto current_time = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = current_time - last_time;
    std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(diff);
    if (duration.count() > delay) {
        last_time = current_time;
        for (auto & pos : vpos) {
            raw_render(fb, pos);
        }
        current_frame += 1;
        if (current_frame >= images.size()) {
            current_frame = 0;
        }
    }
}