#ifndef dpc_contrast_h
#define dpc_contrast_h

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "kmz_core.h"
#include "kmz_color.h"

void dpc_perform_contrast(KmzImage * img, size_t argc, KmzArgbColor * argv);

#endif /* dpc_contrast_h */
