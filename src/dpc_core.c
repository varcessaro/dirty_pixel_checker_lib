#include "dpc_core.h"

// const:

const dpc_percent ONE_THIRD = 1.0 / 3.0,
TWO_THIRDS = 2.0 / 3.0;

// funcs:
dpc_bool dpc_between(long v, long min, long max) {
    return v > min && v < max;
}

dpc_percent hue2rgb(dpc_percent part_1, dpc_percent part_2, dpc_percent hue) {
    if (hue < 0.0) {
        hue = 1.0 - fmodf(-hue, 1.0);
    } else if (hue > 1.0) {
        hue = fmodf(hue, 1.0);
    }
    
    if (1.0 > (6.0 * hue)) {
        return (part_1 + (part_2 - part_1) * 6.0 * hue);
    } else if (1.0 > (2.0 * hue)) {
        return part_2;
    } else if (2.0 > (3.0 * hue)) {
        return (part_1 + (part_2 - part_1) * ((TWO_THIRDS - hue) * 6.0));
    }
    
    return part_1;
}

dpc_percent rgb2delta(dpc_percent max,
                      dpc_percent channel,
                      dpc_percent max_delta,
                      dpc_percent half_max_delta) {
    return (((max - channel) / 6.0) + half_max_delta) / max_delta;
}

size_t get_index(size_t size, size_t x, size_t y) {
    return size * y + x;
}

// region argb methods:
dpc_color_32 dpc_argb_color_to_color_32(struct dpc_argb_color_t * self) {
    dpc_color_32 c = self->a;
    c <<= 8;
    c |= self->r;
    c <<= 8;
    c |= self->g;
    c <<= 8;
    c |= self->b;
    return c;
}

struct dpc_ahsl_color_t dpc_argb_color_to_ahsl_color(struct dpc_argb_color_t * self) {
    dpc_channel a = self->a;
    dpc_percent h, s, l;
    
    if (0 == self->r && 0 == self->g && 0 == self->b) {
        h = s = l = 0.0;
    } else if (self->r == self->g && self->g == self->b) {
        h = s = 0.0;
        l = (((dpc_percent)self->r +
              (dpc_percent)self->g +
              (dpc_percent)self->b) / 3.0) / 255.0;
    } else {
        const dpc_percent red = (dpc_percent)self->r / 255.0,
        green = (dpc_percent)self->g / 255.0,
        blue = (dpc_percent)self->b / 255.0;
                          
        const dpc_percent min = fmin(fmin(red, green), blue),
        max = fmax(fmax(red, green), blue);
                          
        const dpc_percent max_delta = max - min,
        min_max_sum = min + max;
                          
        l = min_max_sum / 2.0;
                          
        if (0.5 > l) {
            s = max_delta / min_max_sum;
        } else {
            s = max_delta / (2.0 - max - min);
        }

        
        const dpc_percent half_max_delta = max_delta / 2.0;
        
        const dpc_percent red_delta = rgb2delta(max, red, max_delta, half_max_delta),
        green_delta = rgb2delta(max, green, max_delta, half_max_delta),
        blue_delta = rgb2delta(max, blue, max_delta, half_max_delta);
                          
        if (max == red) {
            h = blue_delta - green_delta;
        } else if (max == green) {
            h = ONE_THIRD + red_delta - blue_delta;
        } else if (max == blue) {
            h = TWO_THIRDS + green_delta - red_delta;
        } else {
            h = 0.0;
        }
                          
        if (h < 0.0) {
            h = 1.0 - fmodf(-h, 1.0);
        } else if (h > 1.0) {
            h = fmodf(h, 1.0);
        }
    }
    
    return dpc_make_ahsl_color_from_channels(a, h, s, l);
}

dpc_bool dpc_argb_color_equal_to_argb_color(struct dpc_argb_color_t * self, struct dpc_argb_color_t other) {
    return self->a == other.a && self->r == other.r && self->g == other.g && self->b == other.b;
}

dpc_bool dpc_argb_color_equal_to_ahsl_color(struct dpc_argb_color_t * self, struct dpc_ahsl_color_t other) {
    return dpc_argb_color_equal_to_argb_color(self, dpc_ahsl_color_to_argb_color(&other));
}

struct dpc_argb_color_t dpc_make_argb_color_from_channels(dpc_channel a,
                                                          dpc_channel r,
                                                          dpc_channel g,
                                                          dpc_channel b) {
    struct dpc_argb_color_t self;
    self.a = a;
    self.r = r;
    self.g = g;
    self.b = b;
    return self;
}

struct dpc_argb_color_t dpc_make_argb_color_from_color_32(dpc_color_32 argb) {
    dpc_channel * c = (dpc_channel*)&argb;
    return dpc_make_argb_color_from_channels(c[3], c[2], c[1], c[0]);
}

struct dpc_argb_color_t * dpc_free_argb_color(struct dpc_argb_color_t * self) {
    self->a = self->r = self->g = self->b = 0;
    return self;
}

// endregion

// region ahsl
struct dpc_argb_color_t dpc_ahsl_color_to_argb_color(struct dpc_ahsl_color_t * self) {
    dpc_channel a = self->a, r, g, b;
    
    if (0.0 == self->l) {
        r = g = b = 0;
    } else if (0.0 == self->s) {
        r = g = b = (dpc_channel)round(self->l * 255.0);
    } else {
        dpc_percent part_1, part_2;
        
        if (0.5 > self->l) {
            part_2 = self->l * (1.0 + self->s);
        } else {
            part_2 = (self->l + self->s) - (self->s * self->l);
        }
        
        part_1 = 2.0 * self->l - part_2;
        
        r = (dpc_channel)round(255.0 * hue2rgb(part_1, part_2, self->h + ONE_THIRD));
        g = (dpc_channel)round(255.0 * hue2rgb(part_1, part_2, self->h));
        b = (dpc_channel)round(255.0 * hue2rgb(part_1, part_2, self->h - ONE_THIRD));
    }
    
    return dpc_make_argb_color_from_channels(a, r, g, b);
}

dpc_color_32 dpc_ahsl_color_to_color_32(struct dpc_ahsl_color_t * self) {
    struct dpc_argb_color_t c = dpc_ahsl_color_to_argb_color(self);
    return dpc_argb_color_to_color_32(&c);
}

dpc_bool dpc_ahsl_color_equal_to_ahsl_color(struct dpc_ahsl_color_t * self,
                                            struct dpc_ahsl_color_t other) {
    return self->a == other.a && self->h == other.h && self->s == other.s && self->l == other.l;
}

dpc_bool dpc_ahsl_color_equal_to_argb_color(struct dpc_ahsl_color_t * self,
                                            struct dpc_argb_color_t other) {
    return dpc_argb_color_equal_to_ahsl_color(&other, *self);
}

struct dpc_ahsl_color_t dpc_make_ahsl_color_from_channels(dpc_channel a,
                                                          dpc_percent h,
                                                          dpc_percent s,
                                                          dpc_percent l) {
    struct dpc_ahsl_color_t self;
    self.a = a;
    self.h = h;
    self.s = s;
    self.l = l;
    return self;
}

struct dpc_ahsl_color_t * dpc_free_ahsl_color(struct dpc_ahsl_color_t * self) {
    self-> a = 0;
    self->h = self->s = self->l = 0.0;
    return self;
}
// endregion

// region argb matrix
dpc_bool dpc_argb_matrix_has_cell(struct dpc_argb_matrix_t * self,
                                  size_t x,
                                  size_t y) {
    return x < self->size && y < self->size;
}

struct dpc_argb_color_t dpc_argb_matrix_get_cell(struct dpc_argb_matrix_t * self,
                                                  size_t x,
                                                  size_t y) {
    size_t i = get_index(self->size, x, y);
    return self->colors[i];
}

struct dpc_argb_matrix_t * dpc_argb_matrix_set_cell(struct dpc_argb_matrix_t * self,
                                                   size_t x,
                                                   size_t y,
                                                   struct dpc_argb_color_t argb) {
    size_t i = get_index(self->size, x, y);
    self->colors[i] = argb;
    return self;
}

struct dpc_argb_matrix_t dpc_make_argb_matrix_from_size(size_t size) {
    return dpc_make_argb_matrix_from_colors(size, NULL);
}

struct dpc_argb_matrix_t dpc_make_argb_matrix_from_colors(size_t size,
                                                          struct dpc_argb_color_t * colors) {
    struct dpc_argb_matrix_t self;
    size_t dim = size * size * sizeof(struct dpc_argb_color_t);
    self.size = size;
    self.colors = malloc(dim);
    if (colors) {
        memcpy(self.colors, colors, dim);
    } else {
        struct dpc_argb_color_t black = dpc_make_argb_color_from_color_32(0xFF000000);
        for (size_t i = 0; i < dim; ++i) {
            self.colors[i] = black;
        }
    }
    return self;
}

struct dpc_argb_matrix_t * dpc_free_argb_matrix(struct dpc_argb_matrix_t * self) {
    free(self->colors);
    self->size = 0;
    return self;
}
// endregion
