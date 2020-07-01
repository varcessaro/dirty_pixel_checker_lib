#ifndef dpc_gdimage_h
#define dpc_gdimage_h

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "dpc_core.h"
#include "dpc_num.h"
#include "dpc_io.h"

struct dpc_gd_2x_signature_header_t {
    dpc_short signature, width, height;
};

struct dpc_gd_2x_truecolor_header_t {
    dpc_color_32 transparent;
};

struct dpc_gd_2x_palette_header_t {
    dpc_short count;
    dpc_color_32 transparent;
    dpc_color_32 palette[256];
};

struct dpc_gd_2x_color_header_t {
    dpc_bool is_truecolor;
    union {
        struct dpc_gd_2x_truecolor_header_t truecolor;
        struct dpc_gd_2x_palette_header_t palette;
    } header;
};

struct dpc_gd_2x_header_t {
    struct dpc_gd_2x_signature_header_t s;
    struct dpc_gd_2x_color_header_t c;
};

dpc_bool dpc_read_gd_2x_header(dpc_file f, struct dpc_gd_2x_header_t * header);

dpc_bool dpc_read_gd_2x_bytes(dpc_file f, struct dpc_gd_2x_header_t * header, dpc_color_32 * body);

dpc_bool dpc_write_gd_2x_header(dpc_file f, struct dpc_gd_2x_header_t * header);

dpc_bool dpc_write_gd_2x_bytes(dpc_file f, struct dpc_gd_2x_header_t * header, dpc_color_32 * body);

#endif /* dpc_gdimage_h */
