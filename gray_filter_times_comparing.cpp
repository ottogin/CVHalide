#include "halide_cv_utils.h"

#include  <chrono>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace Halide::Tools;
using namespace Halide;
using namespace std;
using namespace cv;

#define ITERS 1000

int main(int argc, char **argv){
    if(argc == 1){
        printf("Usage : %s src_path\n", argv[0]);
        return 0;
    }

    Mat opencv_input, opencv_output;
    Buffer<uint8_t> halide_input;

    halide_input = load_image(argv[1]);
    opencv_input = imread(argv[1], CV_LOAD_IMAGE_COLOR);

    if(halide_input.channels() != 3){
        printf("Sorry, this script still works only with 3-channel images\n");
        return 0;
    }


    Func res = gray(halide_input);
    Buffer<uint8_t> halide_output (halide_input.width(), halide_input.height());
    res.compile_jit();

    cvtColor(opencv_input, opencv_output, CV_BGR2GRAY );


    //START PROCESSING IMAGE with HALIDE
    auto t1 = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < ITERS; i++){
         res.realize(halide_output);
    }

    auto t2 = std::chrono::high_resolution_clock::now();
    double dt = std::chrono::duration_cast<std::chrono::microseconds>\
                                            (t2 - t1).count() / 1e6 / ITERS;
    printf("Halide: %lfs\n", dt);

    //START PROCESSING IMAGE with OPENCV
    t1 = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < ITERS; i++){
        cvtColor(opencv_input, opencv_output, CV_BGR2GRAY );
    }
    t2 = std::chrono::high_resolution_clock::now();
    dt = std::chrono::duration_cast<std::chrono::microseconds>\
                                            (t2 - t1).count() / 1e6 / ITERS;
    printf("OpenCV: %lfs\n", dt);
}
