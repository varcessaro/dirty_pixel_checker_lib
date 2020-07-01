#ifndef dpc_contrast_h
#define dpc_contrast_h

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "dpc_core.h"
#include "dpc_gdimage.h"
#include "dpc_num.h"

dpc_byte dpc_perform_contrast(size_t width, size_t height, size_t matrix_size, struct dpc_gd_2x_header_t * header, dpc_color_32 * image, dpc_color_32 * output);

#endif /* dpc_contrast_h */
