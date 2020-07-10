#include "dpc_contrast.h"

static const uint8_t kmz_color_masks[][5][5] = {
    {{2,2,2,2,2},
     {2,2,0,2,2},
     {2,0,1,0,2},
     {2,2,0,2,2},
     {2,2,2,2,2}},
    
    {{2,2,0,2,2},
     {2,0,1,0,2},
     {2,0,1,0,2},
     {2,2,0,2,2},
     {2,2,2,2,2}},
    
    {{2,2,2,2,2},
     {2,0,0,2,2},
     {0,1,1,0,2},
     {2,0,0,2,2},
     {2,2,2,2,2}},
     
    {{2,2,0,2,2},
     {2,0,1,0,2},
     {0,1,1,0,2},
     {2,0,0,2,2},
     {2,2,2,2,2}},
      
    {{2,2,0,2,2},
     {2,0,1,0,2},
     {2,0,1,1,0},
     {2,2,0,0,2},
     {2,2,2,2,2}},
       
    {{2,2,2,2,2},
     {2,2,0,0,2},
     {2,0,1,1,0},
     {2,0,1,0,2},
     {2,2,0,2,2}},
        
    {{2,2,2,2,2},
     {2,0,0,2,2},
     {0,1,1,0,2},
     {2,0,1,0,2},
     {2,2,0,2,2}},
    
    {{2,0,0,2,2},
     {0,1,1,0,2},
     {0,1,1,0,2},
     {2,0,0,2,2},
     {2,2,2,2,2}},
     
     {{2,2,0,0,2},
      {2,0,1,1,0},
      {2,0,1,1,0},
      {2,2,0,0,2},
      {2,2,2,2,2}},
      
    {{2,2,2,2,2},
     {2,2,0,0,2},
     {2,0,1,1,0},
     {2,0,1,1,0},
     {2,2,0,0,2}},
    
    {{2,2,2,2,2},
     {2,0,0,2,2},
     {0,1,1,0,2},
     {0,1,1,0,2},
     {2,0,0,2,2}},
    
    {{2,0,0,0,2},
     {0,1,1,1,0},
     {0,1,1,1,0},
     {0,1,1,1,0},
     {2,0,0,0,2}}
};

size_t _dpc_matrix_matches_pattern(const uint8_t mask[5][5], KmzMatrix * m) {
    KmzPoint p = {.x=0, .y=0};
    for (p.y=0; p.y < 5; ++p.y) {
        for (p.x = 0; p.x < 5; ++p.x) {
            KmzArgbColor c = KmzArgbColor__from_color_32(KmzMatrix__get_color_at(m, p));
            switch (mask[p.y][p.x]) {
                case 0:
                    if (c.r || c.g || c.b) {
                        return 0;
                    }
                    break;
                case 1:
                    if (!c.r && !c.g && !c.b) {
                        return 0;
                    }
                    break;
            }
        }
    }
    return 1;
}

size_t _dpc_matrix_matches_any_pattern(KmzMatrix * m) {
    for (size_t i = 0; i < 6; ++i) {
        if (_dpc_matrix_matches_pattern(kmz_color_masks[i], m)) {
            return 1;
        }
    }
    return 0;
}

static const size_t _contrast__TRANSPARENT = 0,
_contrast__DIRTY = 1,
_contrast__DARK = 2,
_contrast__TOO_DARK = 3;

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
            c = KmzArgbColor__DARK_GRAY;
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
