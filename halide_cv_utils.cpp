#include "halide_cv_utils.h"

uint16_t RGB2Y[3] = {306, 601, 117};

Halide::Func gray(Halide::Func in){
    Halide::Var x("x1"), y("y1"), c("c1");

    Halide::Func casted("casted");
    casted(x, y, c) = Halide::cast<uint32_t>(in(x, y, c));

    Halide::Expr ycomp = 0;
    for(int i = 0; i < 3; i++)
        ycomp += RGB2Y[i] * casted(x, y, i);

    ycomp = ycomp >> 10;//equivalent to multiplying by [0.299, 0.587, 0.114]

    Halide::Func gr("gray");
    gr(x, y) = Halide::cast<uint8_t>(ycomp);


    //optimizing below
    Halide::Var x_outer("x_outer"), y_outer("y_outer"), x_inner("x_inner"), \
                             y_inner("y_inner"), tile_index("tile_index");
    gr.tile(x, y, x_outer, y_outer, x_inner, y_inner, 64, 64)
      .fuse(x_outer, y_outer, tile_index)
      .parallel(tile_index);
    Halide::Var x_inner_outer, y_inner_outer, x_vectors, y_pairs;
    gr.tile(x_inner, y_inner, x_inner_outer, y_inner_outer, x_vectors, y_pairs, 4, 2)
      .vectorize(x_vectors)
      .unroll(y_pairs);
    return gr;
}
