#include "dpc_gdimage.h"

dpc_int dpc_read_gd_2x_header(dpc_file f, struct dpc_gd_2x_header_t * header) {
    if (!f) {
        return ERR_READ_INVALID_FILE;
    }
    if (!header) {
        return ERR_READ_INVALID_HEADER_PTR;
    }

    struct dpc_gd_2x_header_t w_header;

    dpc_int r = dpc_read_short(f, &w_header.s.signature);
    if (0 != r) {
        return ERR_READ_SIG;
    }

    r = dpc_read_short(f, &w_header.s.width);
    if (0 != r) {
        return ERR_READ_WIDTH;
    }

    r = dpc_read_short(f, &w_header.s.height);
    if (0 != r) {
        return ERR_READ_HEIGHT;
    }

    r = dpc_read_byte(f, (dpc_byte*)w_header.c.is_truecolor);
    if (0 != r) {
        return ERR_READ_IS_TC;
    }

    switch (w_header.s.signature) {
        case 0xFFFE:
            r = dpc_read_int(f, &w_header.c.header.truecolor.transparent);
            if (0 != r) {
                return ERR_READ_TC_TRANS;
            }
        break;
        case 0xFFFF:
            r = dpc_read_short(f, &w_header.c.header.palette.count);
            if (0 != r) {
                return ERR_READ_P_COUNT;
            }

            r = dpc_read_int(f, &w_header.c.header.palette.transparent);
            if (0 != r) {
                return ERR_READ_P_TRANS;
            }

            for (ssize_t i = 0; i < 256; ++i) {
                r = dpc_read_int(f, w_header.c.header.palette.palette + i);
                if (0 != r) {
                    return ERR_READ_P_COLOR;
                }
            }
        break;
    }
    memcpy(header, &w_header, sizeof(struct dpc_gd_2x_header_t));
    return ERR_READ_OK;
}

dpc_int dpc_read_gd_2x_bytes(dpc_file f, struct dpc_gd_2x_header_t * header, dpc_color_32 * body) {
    size_t l = header->s.width * header->s.height;
    dpc_int r;
    header->c.is_truecolor = header->s.signature == 0xFFFE;
    for (size_t i = 0; i < l; ++i) {
        if (header->c.is_truecolor) {
            dpc_color_32 c;
            r = dpc_read_int(f, &c);
            if (0 != r) {
                return ERR_READ_TC_COLOR;
            }
            body[i] = c;
        } else {
            dpc_byte c;
            r = dpc_read_byte(f, &c);
            if (0 != r) {
                return ERR_READ_P_INDEX;
            }
            body[i] = header->c.header.palette.palette[c];
        }
    }
    return ERR_READ_OK;
}

dpc_int dpc_write_gd_2x_header(FILE * f, struct dpc_gd_2x_header_t * header) {
    if (!f) {
        return ERR_WRITE_INVALID_FILE;
    }
    if (!header) {
        return ERR_WRITE_INVALID_HEADER_PTR;
    }

    dpc_int r = dpc_write_short(f, header->s.signature);
    if (0 != r) {
        return ERR_WRITE_SIG;
    }

    r = dpc_write_short(f, header->s.width);
    if (0 != r) {
        return ERR_WRITE_WIDTH;
    }

    r = dpc_write_short(f, header->s.height);
    if (0 != r) {
        return ERR_WRITE_HEIGHT;
    }

    r = dpc_write_byte(f, header->s.signature == 0xFFFE);
    if (0 != r) {
        return ERR_WRITE_IS_TC;
    }
    switch (header->s.signature) {
        case 0xFFFE:
            r = dpc_write_int(f, header->c.header.truecolor.transparent);
            if (0 != r) {
                return ERR_WRITE_TC_TRANS;
            }
        break;
        case 0xFFFF:
            return ERR_WRITE_P_IMAGE;
    }
    return ERR_WRITE_OK;
}

dpc_int dpc_write_gd_2x_bytes(FILE * f, struct dpc_gd_2x_header_t * header, dpc_color_32 * bytes) {
    size_t l = header->s.width * header->s.height;
    dpc_int r;
    for (size_t i = 0; i < l; ++i) {
        r = dpc_write_int(f, bytes[i]);
        if (0 != r) {
            return ERR_WRITE_TC_COLOR;
        }
    }
    return ERR_WRITE_OK;
}
