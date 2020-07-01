#include "dpc_contrast.h"

const static struct dpc_argb_color_t COLOR_RED = {.a=0x7F, .r=0xFF, .g=0x00, .b=0x00},
                                     COLOR_BLACK = {.a=0x7F, .r=0x00, .g=0x00, .b=0x00},
                                     COLOR_FUSCIA = {.a=0x7F, .r=0xFF, .g=0x00, .b=0xFF},
                                     COLOR_CYAN = {.a=0x7F, .r=0x00, .g=0x8B, .b=0x8B},
                                     COLOR_LIME = {.a=0x7F, .r=0x00, .g=0xFF, .b=0x00},
                                     COLOR_GREY = {.a=0x7F, .r=0x40, .g=0x40, .b=0x40};

struct dpc_argb_color_t dpc_contrast_matrix(size_t half_size, struct dpc_gd_2x_header_t * header, struct dpc_argb_matrix_t * matrix) {
    struct dpc_argb_color_t c = dpc_argb_matrix_get_cell(matrix, half_size, half_size);
    if (c.a > 0x00) {
        return COLOR_RED;
    } else if (!c.r && !c.g && !c.b) {
        return COLOR_BLACK;
    }
    
    dpc_channel avg = (dpc_channel)round(((dpc_percent)(c.r + c.g + c.b)) / 3.0);
    
    if (64.0 < avg) {
        return dpc_make_argb_color_from_channels(0x7F, avg, avg, avg);
    } else if (12.0 < avg) {
        return COLOR_GREY;
    } else if (10.0 < avg) {
        return COLOR_CYAN;
    }
    return COLOR_LIME;
}

dpc_byte dpc_perform_contrast(size_t width,
                              size_t height,
                              size_t matrix_size,
                              struct dpc_gd_2x_header_t * header,
                              dpc_color_32 * image,
                              dpc_color_32 * output) {
    struct dpc_argb_matrix_t m = dpc_make_argb_matrix_from_size(matrix_size);
    size_t half_size = matrix_size / 2;
    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            for (size_t y_cell = 0; y_cell < matrix_size; ++y_cell) {
                ssize_t y_offset = y_cell - half_size;
                ssize_t y_image = y + y_offset;
                for (size_t x_cell = 0; x_cell < matrix_size; ++x_cell) {
                    ssize_t x_offset = x_cell - half_size;
                    ssize_t x_image = x + x_offset;
                    
                    if (dpc_between(y_image, -1, height) && dpc_between(x_image, -1, width) && dpc_argb_matrix_has_cell(&m, x_cell, y_cell)) {
                        dpc_argb_matrix_set_cell(&m, x_cell, y_cell, dpc_make_argb_color_from_color_32(image[width * y_image + x_image]));
                    } else {
                        dpc_argb_matrix_set_cell(&m, x_cell, y_cell, COLOR_BLACK);
                    }
                }
            }
            struct dpc_argb_color_t cc = dpc_contrast_matrix(half_size, header, &m);
            output[y * width + x] = dpc_argb_color_to_color_32(&cc);
        }
    }
    dpc_free_argb_matrix(&m);
    return 1;
}
