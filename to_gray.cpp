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

    Func res = gray(input);
    Buffer<uint8_t> output = res.realize(input.width(), input.height());
    save_image(output,"images/gray.png");
}
