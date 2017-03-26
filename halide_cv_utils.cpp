#include "halide_cv_utils.h"

float RGB2Y[3] = {0.299, 0.587, 0.114};

Halide::Func gray(Halide::Func in){
    Halide::Var x("x1"), y("y1"), c("c1");

    Halide::Func casted("casted");
    casted(x, y, c) = Halide::cast<float>(in(x, y, c));
    Halide::Expr ycomp = 0;
    for(int i = 0; i < 3; i++)
        ycomp += RGB2Y[i] * casted(x, y, i);

    Halide::Func gr("gray");
    gr(x, y) = Halide::cast<uint8_t>(ycomp);
    return gr;
}
