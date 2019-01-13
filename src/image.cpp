#include "image.hpp"

PNGImage::PNGImage(FILE * f) {
    color_channels = 0;
    png_byte header[8];
    
    fread(header, 1, 8, f);
    int is_png = !png_sig_cmp(header, 0, 8);
    if (!is_png) {
        fclose(f);
        throw std::runtime_error("is not png");
    }
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(f);
        throw std::runtime_error("png_create_read_struct problem");
    }
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        fclose(f);
        throw std::runtime_error("png_create_info_struct problem");
    }
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        fclose(f);
        throw std::runtime_error("png_create_info_struct problem");
    }
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(f);
        throw std::runtime_error("setjmp problem");
    }

    png_init_io(png_ptr, f);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);

    width = png_get_image_width(png_ptr, info_ptr);
    height = png_get_image_height(png_ptr, info_ptr);
    auto color_type = png_get_color_type(png_ptr, info_ptr);
    auto bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    if (bit_depth == 16) {
        png_set_strip_16(png_ptr);
    }
    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(png_ptr);
    }
    // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
        png_set_expand_gray_1_2_4_to_8(png_ptr);
    }
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
        png_set_tRNS_to_alpha(png_ptr);
    }
    // These color_type don't have an alpha channel then fill it with 0xff.
    if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);
        // use RGBA palette
        color_channels = 4;
    }
    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        png_set_gray_to_rgb(png_ptr);
        // use RGB palette
        color_channels = 3;
    }

    png_read_update_info(png_ptr, info_ptr);
    int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    raw = (uint8_t *) new png_byte[row_bytes * height];
    if (!raw) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(f);
        throw std::runtime_error("empty raw data");
    }
    png_bytepp row_pointers = new png_bytep[height];
    if (!row_pointers) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        delete[] raw;
        fclose(f);
        throw std::runtime_error("row_pointers problem");
    }
    for (uint32_t i = 0; i < height; ++i) {
        row_pointers[i] = raw + i * row_bytes;
    }
    png_read_image(png_ptr, row_pointers);

    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    delete[] row_pointers;
}

PNGImage::PNGImage(const char * image) {
    FILE * f = fopen(image, "r");
    if (!f) {
        throw std::runtime_error("fopen problem");
    }
    new (this) PNGImage(f);
    fclose(f);
}

RAWImage::RAWImage(const char * image) {
    FILE * f = fopen(image, "r");
    if (!f) {
        throw std::runtime_error("fopen problem");
    }
    uint32_t pitch = 0;
    color_channels = 4;
    fread(&pitch, sizeof(uint32_t), 1, f);
    width = pitch / color_channels;
    fread(&height, sizeof(uint32_t), 1, f);
    raw = new uint8_t [pitch * height];
    fread(raw, pitch * height, 1, f);
    fclose(f);
}