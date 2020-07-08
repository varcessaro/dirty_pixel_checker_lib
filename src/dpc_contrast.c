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
    
    {{2,0,0,0,2},
     {0,1,1,1,0},
     {0,1,1,1,0},
     {0,1,1,1,0},
     {2,0,0,0,2}}
};

size_t _dpc_matrix_matches_pattern(const uint8_t mask[5][5], KmzMatrix * m) {
    kmz_point p = {.x=0, .y=0};
    for (p.y=0; p.y < 5; ++p.y) {
        for (p.x = 0; p.x < 5; ++p.x) {
            kmz_argb_color c = kmz_argb_color__from_color_32(KmzMatrix__get_color_at(m, p));
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

kmz_color_32 _dpc_contrast_matrix(KmzMatrix * m) {
    kmz_point p = {.x=2, .y=2};
    kmz_argb_color c = kmz_argb_color__from_color_32(KmzMatrix__get_color_at(m, p));
    
    if (c.a > 0x00) {
        c = kmz_argb_color__RED;
    } else if (!c.r && !c.g && !c.b) {
        c = kmz_argb_color__BLACK;
    } else if (_dpc_matrix_matches_any_pattern(m)) {
        c = kmz_argb_color__FUSCIA;
    } else {
        uint32_t avg = c.r + c.g + c.b;
        
        if (192 < avg) {
            avg /= 3;
            c = kmz_argb_color__from_channels(0x00, avg, avg, avg);
        } else if (36 < avg) {
            c = kmz_argb_color__GREY;
        } else if (30 < avg) {
            c = kmz_argb_color__CYAN;
        } else {
            c = kmz_argb_color__LIME;
        }
    }
    
    return kmz_color_32__from_argb_color(c);
}

void dpc_perform_contrast(KmzImage * img) {
    kmz_apply_matrix_transform(&_dpc_contrast_matrix, img, 5);
}
