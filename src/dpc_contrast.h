#ifndef dpc_contrast_h
#define dpc_contrast_h

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "kmz_core.h"
#include "kmz_color.h"
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define DPC_THREADED 0
#else
#include <pthread.h>
#define DPC_THREADED 1
#endif

void dpc_perform_contrast(KmzImage * img, size_t argc, KmzArgbColor * argv);

void dpc_perform_clean(KmzImage * img);

#endif /* dpc_contrast_h */
