#include "dpc_gdimage.h"

dpc_bool dpc_read_gd_2x_header(dpc_file f, struct dpc_gd_2x_header_t * header) {
    if (f && header) {
        // TODO: Implement a check for file size.
        struct dpc_gd_2x_header_t w_header;
        dpc_bool result = FALSE;
        
        if (dpc_read_short(f, &w_header.s.signature) &&
            dpc_read_short(f, &w_header.s.width) &&
            dpc_read_short(f, &w_header.s.height) &&
            dpc_read_byte(f, (dpc_byte*)&w_header.c.is_truecolor)) {
            switch (w_header.s.signature) {
                case 0xFFFE:
                    if (dpc_read_int(f, &w_header.c.header.truecolor.transparent)) {
                        result = TRUE;
                    }
                    break;
                case 0xFFFF:
                    if (dpc_read_short(f, &w_header.c.header.palette.count) &&
                        dpc_read_int(f, &w_header.c.header.palette.transparent)) {
                        result = TRUE;
                        for (size_t i = 0; i < 256; ++i) {
                            if (!dpc_read_int(f, w_header.c.header.palette.palette + i)) {
                                result = FALSE;
                                break;
                            }
                        }
                    }
                    break;
            }
        }
        
        if (result) {
            memcpy(header, &w_header, sizeof(struct dpc_gd_2x_header_t));
        }
        return result;
    }
    return FALSE;
}

dpc_bool dpc_read_gd_2x_bytes(dpc_file f, struct dpc_gd_2x_header_t * header, dpc_color_32 * body) {
    size_t l = header->s.width * header->s.height;
    dpc_bool is_truecolor = header->s.signature == 0xFFFE;
    header->c.is_truecolor = is_truecolor;
    for (size_t i = 0; i < l; ++i) {
        if (is_truecolor) {
            dpc_color_32 c;
            if (dpc_read_int(f, &c)) {
                body[i] = c;
            } else {
                return FALSE;
            }
        } else {
            dpc_byte c;
            if (dpc_read_byte(f, &c)) {
                body[i] = header->c.header.palette.palette[c];
            } else {
                return FALSE;
            }
        }
    }
    return TRUE;
}

dpc_bool dpc_write_gd_2x_header(FILE * f, struct dpc_gd_2x_header_t * header) {
    if (f && header) {
        if (dpc_write_short(f, header->s.signature) &&
            dpc_write_short(f, header->s.width) &&
            dpc_write_short(f, header->s.height) &&
            dpc_write_byte(f, header->s.signature == 0xFFFE)) {
            switch (header->s.signature) {
                case 0xFFFE:
                    if (dpc_write_int(f, header->c.header.truecolor.transparent)) {
                        return TRUE;
                    }
                    break;
                case 0xFFFF:
                    // Don't support saving as paletted - Header will
                    // always be converted to non-paletted
                    break;
            }
        }
    }
    return FALSE;
}

dpc_bool dpc_write_gd_2x_bytes(FILE * f, struct dpc_gd_2x_header_t * header, dpc_color_32 * bytes) {
    size_t l = header->s.width * header->s.height;
    for (size_t i = 0; i < l; ++i) {
        if (!dpc_write_int(f, bytes[i])) {
            return FALSE;
        }
    }
    return TRUE;
}
