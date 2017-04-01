#include "halide_cv_utils.h"

#include <opencv2/highgui/highgui.hpp>

using namespace Halide;
using namespace std;

int main(int argc, char **argv) {
    if(argc == 1){
        printf("Usage : %s src_path\n", argv[0]);
        return 0;
    }

    cv::Mat opencv_input = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);
    Buffer<uint8_t> input = Runtime::Buffer<uint8_t>::make_interleaved(
                                    opencv_input.data, opencv_input.cols,
                                    opencv_input.rows, opencv_input.channels());

    if(input.channels() != 3){
        printf("Sorry, this script still works only with 3-channel images\n");
        return 0;
    }

    Func res = gray(input);
    Buffer<uint8_t> output = res.realize(input.width(), input.height());

    cv::Mat opencv_output(output.height(), output.width(), CV_8UC1, output.data());
    cv::imwrite("images/gray.png", opencv_output);
}
