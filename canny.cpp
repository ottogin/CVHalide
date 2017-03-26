#include "halide_cv_utils.h"

using namespace Halide::Tools;
using namespace Halide;
using namespace std;

int main(int argc, char **argv) {
    if(argc == 1){
        printf("Usage : %s src_path\n", argv[0]);
        return 0;
    }

    Buffer<uint8_t> input = load_image(argv[1]);

    if(input.channels() != 3){
        printf("Sorry, this script still works only with 3-channel images\n");
        return 0;
    }

    Var x("x"), y("y"), c("c");
    Func in("in");
    in(x,y,c) = input(clamp(x,0,input.width() - 1) , clamp(y,0,input.height() - 1), c);

    Func res = gray(in);
    Buffer<uint8_t> output = res.realize(input.width(), input.height());
    save_image(output,"images/canny.png");
}
