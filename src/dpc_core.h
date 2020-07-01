#ifndef dpc_core_h
#define dpc_core_h

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "dpc_num.h"

// region funcs
dpc_bool dpc_between(long v, long min, long max);
// endregion

// region argb
struct dpc_argb_color_t {
    dpc_channel b, g, r, a;
};

struct dpc_argb_color_t dpc_make_argb_color_from_channels(dpc_channel a,
                                                          dpc_channel r,
                                                          dpc_channel g,
                                                          dpc_channel b);

struct dpc_argb_color_t dpc_make_argb_color_from_color_32(dpc_color_32 argb);

dpc_color_32 dpc_argb_color_to_color_32(struct dpc_argb_color_t * self);

struct dpc_ahsl_color_t dpc_argb_color_to_ahsl_color(struct dpc_argb_color_t * self);

dpc_bool dpc_argb_color_equal_to_argb_color(struct dpc_argb_color_t * self, struct dpc_argb_color_t other);

dpc_bool dpc_argb_color_equal_to_ahsl_color(struct dpc_argb_color_t * self, struct dpc_ahsl_color_t other);

struct dpc_argb_color_t * dpc_free_argb_color(struct dpc_argb_color_t * argb);
// endregion

// region ahsl
struct dpc_ahsl_color_t {
    dpc_channel a;
    dpc_percent h, s, l;
};

struct dpc_ahsl_color_t dpc_make_ahsl_color_from_channels(dpc_channel a,
                                                          dpc_percent h,
                                                          dpc_percent s,
                                                          dpc_percent l);

dpc_color_32 dpc_ahsl_color_to_color_32(struct dpc_ahsl_color_t * self);

struct dpc_argb_color_t dpc_ahsl_color_to_argb_color(struct dpc_ahsl_color_t * self);

dpc_bool dpc_ahsl_color_equal_to_argb_color(struct dpc_ahsl_color_t * self, struct dpc_argb_color_t other);

dpc_bool dpc_ahsl_color_equal_to_ahsl_color(struct dpc_ahsl_color_t * self, struct dpc_ahsl_color_t other);

struct dpc_ahsl_color_t * dpc_free_ahsl_color(struct dpc_ahsl_color_t * ahsl);
// endregion

// region argb
struct dpc_argb_matrix_t {
    size_t size;
    struct dpc_argb_color_t * colors;
};

struct dpc_argb_matrix_t dpc_make_argb_matrix_from_size(size_t size);

struct dpc_argb_matrix_t dpc_make_argb_matrix_from_colors(size_t size,
                                                          struct dpc_argb_color_t * colors);

dpc_bool dpc_argb_matrix_has_cell(struct dpc_argb_matrix_t * self,
                                  size_t x,
                                  size_t y);
                                                          
struct dpc_argb_color_t dpc_argb_matrix_get_cell(struct dpc_argb_matrix_t * self,
                                                 size_t x,
                                                 size_t y);
                                                          
struct dpc_argb_matrix_t * dpc_argb_matrix_set_cell(struct dpc_argb_matrix_t * self,
                                                    size_t x,
                                                    size_t y,
                                                    struct dpc_argb_color_t argb);

struct dpc_argb_matrix_t * dpc_free_argb_matrix(struct dpc_argb_matrix_t * argb);
// endregion

#endif /* dpc_core_h */
