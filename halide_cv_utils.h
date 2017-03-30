#ifndef HALIDE_CV_UTILS
#define HALIDE_CV_UTILS

#include <Halide.h>
#include "halide_image_io.h"

Halide::Func gray(Halide::Buffer<uint8_t> in);

#endif
