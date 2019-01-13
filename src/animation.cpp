#include "animation.hpp"

Animation::Animation(const char * filename) {
    std::vector<uint32_t> addr;
    uint32_t length;
    FILE * f = fopen(filename, "r");

    fread(&length, sizeof(uint32_t), 1, f);
    addr.reserve(length);
    for (uint32_t i = 0; i < length; i++) {
        uint32_t tmp;
        fread(&tmp, sizeof(uint32_t), 1, f);
        addr.push_back(tmp);
    }
    images.reserve(length);
    for (auto ptr : addr) {
        fseek(f, ptr, SEEK_SET);
        images.push_back(new PNGImage(f));
    }

    width = images[0]->width;
    height = images[0]->height;

    fclose(f);
}

Animation::Animation(const char *fmt, uint32_t start, uint32_t stop) {
    char img_name[128];
    images.reserve(stop - start + 1);
    for (uint32_t index = start; index <= stop; index++) {
        sprintf(img_name, fmt, index);
        images.push_back(new PNGImage(img_name));
    }

    width = images[0]->width;
    height = images[0]->height;
}

Animation::~Animation() {
    for (auto & i : images) {
        delete i;
    }
}

void Animation::raw_render(FrameBuffer & fb, const Point pos) {
    fb.draw_image(pos, images[current_frame], true);
}

void Animation::render(FrameBuffer & fb, const Point pos, long int delay) {
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

void Animation::render(FrameBuffer & fb, const std::vector<Point> vpos, long int delay) {
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