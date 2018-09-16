#include "framebuffer.hpp"

Image::Image(const char * image) {
    FILE *f;
    png_byte header[8];

    if ( !( f = fopen(image, "r" ) ) ) {
        std::runtime_error("fopen problem");
    }
    fread( header, 1, 8, f );
    int is_png = !png_sig_cmp( header, 0, 8 );
    if ( !is_png ) {
        fclose( f );
        std::runtime_error("is not png");
    }
    png_structp png_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
    if ( !png_ptr ) {
        fclose( f );
        std::runtime_error("png_create_read_struct problem");
    }
    png_infop info_ptr = png_create_info_struct( png_ptr );
    if ( !info_ptr ) {
        png_destroy_read_struct( &png_ptr, (png_infopp) NULL, (png_infopp) NULL );
        fclose( f );
        std::runtime_error("png_create_info_struct problem");
    }
    png_infop end_info = png_create_info_struct( png_ptr );
    if ( !end_info ) {
        png_destroy_read_struct( &png_ptr, (png_infopp) NULL, (png_infopp) NULL );
        fclose( f );
        std::runtime_error("png_create_info_struct problem");
    }
    if ( setjmp( png_jmpbuf( png_ptr ) ) ) {
        png_destroy_read_struct( &png_ptr, &info_ptr, &end_info );
        fclose( f );
        std::runtime_error("setjmp problem");
    }
    int color_type;
    png_init_io( png_ptr, f );
    png_set_sig_bytes( png_ptr, 8 );
    png_read_info( png_ptr, info_ptr );
    png_get_IHDR( png_ptr, info_ptr, &width, &height, &depth, &color_type, NULL, NULL, NULL );
    png_read_update_info( png_ptr, info_ptr );
    int row_bytes = png_get_rowbytes( png_ptr, info_ptr );
    raw = new png_byte [row_bytes * height];
    if ( !raw ) {
        png_destroy_read_struct( &png_ptr, &info_ptr, &end_info );
        fclose( f );
        std::runtime_error("empty raw data");
    }
    png_bytepp row_pointers = new png_bytep [height];
    if ( !row_pointers ) {
        png_destroy_read_struct( &png_ptr, &info_ptr, &end_info );
        delete [] raw;
        fclose( f );
        std::runtime_error("row_pointers problem");
    }
    for ( int i = 0; i < height; ++i ) {
        // row_pointers[height - 1 - i] = raw + i * row_bytes;
        row_pointers[i] = raw + i * row_bytes;
    }
    png_read_image( png_ptr, row_pointers );
    // support only RGB images
    if (png_get_color_type( png_ptr, info_ptr ) != PNG_COLOR_TYPE_RGB) {
        std::runtime_error("color type not supported!");
    }
    png_destroy_read_struct( &png_ptr, &info_ptr, &end_info );
    delete [] row_pointers;
    fclose( f );
}

Color Image::get_pixel(uint32_t x, uint32_t y) const {
    uint32_t location = (y * width + x) * 4;
    return {raw[location + 0], raw[location + 1], raw[location + 2]};
}

Image::~Image() {
    delete [] raw;
}

FrameBuffer::FrameBuffer(const char * device) {
    fbfd = open(device, O_RDWR);
    if (fbfd == -1) {
        std::runtime_error("cannot open framebuffer device");
    }
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        std::runtime_error("error reading fixed information");
    }
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        std::runtime_error("error reading variable information");
    }
    screen_size = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
    buffer = (char *) mmap(0, screen_size, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if (*(int *)buffer == -1) {
        std::runtime_error("failed to map framebuffer device to memory");
    }
}

FrameBuffer::~FrameBuffer() {
    munmap(buffer, screen_size);
    close(fbfd);
}

void FrameBuffer::draw_pixel(Point point, Color color) {
    long int location = (point.x + vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                        (point.y + vinfo.yoffset) * finfo.line_length;
    if (vinfo.bits_per_pixel == 32) {
        *(buffer + location + 0) = color.blue;
        *(buffer + location + 1) = color.green;
        *(buffer + location + 2) = color.red;
        // no transparency
        *(buffer + location + 3) = 0;
    } else {
        unsigned short int c = color.red << 11 | color.green << 5 | color.blue;
        *((unsigned short int *)(buffer + location)) = c;
    }
}

void FrameBuffer::draw_rectangle(Rect rect, Color color) {
    // TODO: написать нормальный код!
    for (uint32_t y = rect.p1.y; y < rect.p2.y; ++y) {
        for (uint32_t x = rect.p1.x; x < rect.p2.x; ++x) {
            draw_pixel({x, y}, color);
        }
    }
}

void FrameBuffer::draw_image(Point point, const Image & image) {
    for (uint32_t y = 0; y < image.height; ++y) {
        for (uint32_t x = 0; x < image.width; ++x) {
            draw_pixel({point.x + x, point.y + y}, image.get_pixel(x, y));
        }
    }
}