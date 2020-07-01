#ifndef dpc_gdimage_h
#define dpc_gdimage_h

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "dpc_core.h"
#include "dpc_num.h"
#include "dpc_io.h"

static const dpc_int ERR_READ_OK = 0,
                     ERR_READ_SIG = -1,
                     ERR_READ_WIDTH = -2,
                     ERR_READ_HEIGHT = -3,
                     ERR_READ_IS_TC = -4,
                     ERR_READ_TC_TRANS = -5,
                     ERR_READ_P_COUNT = -6,
                     ERR_READ_P_TRANS = -7,
                     ERR_READ_P_COLOR = -8,
                     ERR_READ_INVALID_FILE = -9,
                     ERR_READ_INVALID_HEADER_PTR = -10,
                     ERR_READ_TC_COLOR = -11,
                     ERR_READ_P_INDEX = -12;

static const dpc_int ERR_WRITE_OK = 0,
                     ERR_WRITE_INVALID_FILE = -1,
                     ERR_WRITE_INVALID_HEADER_PTR = -2,
                     ERR_WRITE_SIG = -3,
                     ERR_WRITE_WIDTH = -4,
                     ERR_WRITE_HEIGHT = -5,
                     ERR_WRITE_IS_TC = -6,
                     ERR_WRITE_TC_TRANS = -7,
                     ERR_WRITE_P_IMAGE = -8,
                     ERR_WRITE_TC_COLOR = -9;

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

dpc_int dpc_read_gd_2x_header(dpc_file f, struct dpc_gd_2x_header_t * header);

dpc_int dpc_read_gd_2x_bytes(dpc_file f, struct dpc_gd_2x_header_t * header, dpc_color_32 * body);

dpc_int dpc_write_gd_2x_header(dpc_file f, struct dpc_gd_2x_header_t * header);

dpc_int dpc_write_gd_2x_bytes(dpc_file f, struct dpc_gd_2x_header_t * header, dpc_color_32 * body);

#endif /* dpc_gdimage_h */
