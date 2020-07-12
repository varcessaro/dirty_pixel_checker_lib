#include "dpc_contrast.h"
#include "dpc_pattern_table.c"

#define _dpc_matrix_compare(mask, color) (mask == (color != 0) || 2 == mask)

size_t _dpc_matrix_perform_compare(KmzPoint * p, size_t s, size_t e, const uint8_t mask[5][5], KmzMatrix * m) {
    for (p->x = s; p->x < e; ++p->x)
        if (!_dpc_matrix_compare(mask[p->y][p->x], KmzMatrix__get_color_at(m, *p)))
            return 0;
    ++p->y;
    return 1;
}

size_t _dpc_matrix_matches_pattern(const uint8_t mask[5][5], KmzMatrix * m) {
    KmzPoint p = {.x=0, .y=0};
    return _dpc_matrix_perform_compare(&p, 1, 4, mask, m) &&
    _dpc_matrix_perform_compare(&p, 0, 5, mask, m) &&
    _dpc_matrix_perform_compare(&p, 0, 5, mask, m) &&
    _dpc_matrix_perform_compare(&p, 0, 5, mask, m) &&
    _dpc_matrix_perform_compare(&p, 1, 4, mask, m);
}

const uint8_t ** _dpc_matrix_matches_any_pattern(KmzMatrix * m) {
    for (size_t i = 0; i < 256; ++i) {
        if (_dpc_matrix_matches_pattern(dpc__COLOR_MASKS[i], m)) {
            return (const uint8_t **)dpc__COLOR_MASKS[i];
        }
    }
    return NULL;
}

static const size_t _contrast__TRANSPARENT = 0,
_contrast__DIRTY = 1,
_contrast__DARK = 2,
_contrast__TOO_DARK = 3;

static const KmzArgbColor _DPC_DARK_GRAY = {.a=0, .r=64, .g=64, .b=64};

kmz_color_32 _dpc_contrast_matrix(size_t argc, void * argv, KmzMatrix * m) {
    KmzPoint p = {.x=2, .y=2};
    KmzArgbColor * cs = argv;
    KmzArgbColor c = KmzArgbColor__from_color_32(KmzMatrix__get_color_at(m, p));
    
    if (4 == argc) {
        memcpy(cs, argv, argc * sizeof(KmzArgbColor));
    }
    
    if (c.a > 0x00) {
        c = cs[_contrast__TRANSPARENT];
    } else if (!c.r && !c.g && !c.b) {
        c = KmzArgbColor__BLACK;
    } else if (_dpc_matrix_matches_any_pattern(m)) {
        c = cs[_contrast__DIRTY];
    } else {
        uint32_t avg = c.r + c.g + c.b;
        
        if (192 < avg) {
            avg /= 3;
            c = KmzArgbColor__from_channels(0x00, avg, avg, avg);
        } else if (36 < avg) {
            c = _DPC_DARK_GRAY;
        } else if (30 < avg) {
            c = cs[_contrast__DARK];
        } else {
            c = cs[_contrast__TOO_DARK];
        }
    }
    
    return kmz_color_32__from_argb_color(c);
}

void dpc_perform_contrast(KmzImage * img, size_t argc, KmzArgbColor * argv) {
    KmzRectangle area = {.pos={.x=0, .y=0}, .size=img->dimen};
    KmzArgbColor cs[4] = {
        KmzArgbColor__RED,
        KmzArgbColor__YELLOW,
        KmzArgbColor__CYAN,
        KmzArgbColor__GREEN
    };
    if (0 == argc) {
        argc = 4;
        argv = cs;
    }
    KmzImage__apply_filter_with_args_to(img, argc, argv, &_dpc_contrast_matrix, area, 5);
}

kmz_color_32 _dpc_clean_pattern(size_t argc, void * argv, KmzMatrix * m) {
    KmzPoint p = {.x=2, .y=2};
    KmzArgbColor c = KmzArgbColor__from_color_32(KmzMatrix__get_color_at(m, p));
    
    if (c.a) {
        c.a = 0;
    }
    if (!c.r && !c.g && !c.b) {
        c = KmzArgbColor__BLACK;
    } else {
        const uint8_t ** pattern = _dpc_matrix_matches_any_pattern(m);
        if (pattern) {
            c = KmzArgbColor__BLACK;
            for (p.y = 0; p.y < 5; ++p.y) {
                for (p.x = 0; p.x < 5; ++p.x) {
                    // We want to clean the entire pattern match if we encounter one. This prevents possible leaks of pixels.
                    KmzMatrix__set_color_at(m, p, 0x000000);
                }
            }
        }
    }
    
    return kmz_color_32__from_argb_color(c);
}

kmz_color_32 _dpc_clean_matrix(size_t argc, void * argv, KmzMatrix * m) {
    KmzPoint p = {.x=1, .y=1};
    KmzArgbColor c = KmzArgbColor__from_color_32(KmzMatrix__get_color_at(m, p));
    
    if (!c.r || !c.g || !c.b) {
        c = KmzArgbColor__BLACK;
    } else {
        uint32_t avg = c.r + c.g + c.b;
        
        if (36 < avg ) {
            // Do nothing
        } else if (30 < avg) {
            float black_c = 0.;
            float total_c = 0.;
            for (p.y = 0; p.y < 5; ++p.y) {
                for (p.x = 0; p.x < 5; ++p.x) {
                    ++total_c;
                    if (0x000000 == KmzMatrix__get_color_at(m, p)) {
                        ++black_c;
                    }
                }
            }
            if (black_c / total_c > 0.75) {
                c = KmzArgbColor__BLACK;
            }
        } else {
            c = KmzArgbColor__BLACK;
        }
    }
    
    return kmz_color_32__from_argb_color(c);
}

void dpc_perform_clean(KmzImage * img) {
    KmzImage__apply_filter(img, &_dpc_clean_pattern, 5);
}
