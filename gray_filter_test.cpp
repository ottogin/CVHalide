#include "halide_cv_utils.h"

#include <chrono>
#include <cmath>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace Halide::Tools;
using namespace Halide;
using namespace std;
using namespace cv;

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

    //PROCESSING IMAGE with HALIDE
    Func res = gray(halide_input);
    Buffer<uint8_t> halide_output (halide_input.width(), halide_input.height());
    res.realize(halide_output);

    //PROCESSING IMAGE with OPENCV
    cvtColor(opencv_input, opencv_output, CV_BGR2GRAY );

    //CHECK

    #define RESET "\033[0m"
    #define RED "\033[1;31m"
    #define GREEN "\033[1;32m"

    int w = halide_output.width();
    int h = halide_output.height();
    if(w != opencv_output.cols || h != opencv_output.rows){
        printf("%sFAILED: %s", RED, RESET);
        printf("different sizes\n");
        return 0;
    }

    int bad_pixels = 0;
    int max_diff = 0;
    for(int i = 0; i < w; i++)
        for(int j = 0; j < h; j++)
            if (opencv_output.data[i * h + j] != halide_output
                                                            .data()[i * h + j]){
                bad_pixels++;
                max_diff = max(max_diff,   abs(opencv_output.data[i * h + j] -
                                            halide_output .data()[i * h + j]));
            }

    if(max_diff > 1){
        printf("%sFAILED: %s", RED, RESET);
        printf("bad pixels: %d%%  max_diff: %d\n", (int)(100*bad_pixels/h/w), max_diff);
        return 0;
    }

    printf("%sSUCCESS: %s", GREEN, RESET);
    printf("the images are the same\n");
    return 0;
}
