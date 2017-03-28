#include <time.h>
#include <sys/times.h>

#include "halide_cv_utils.h"

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

    Buffer<uint8_t> input = load_image(argv[1]);

    Mat image;
    image = imread(argv[1], CV_LOAD_IMAGE_COLOR);

    if(input.channels() != 3){
        printf("Sorry, this script still works only with 3-channel images\n");
        return 0;
    }

    Var x("x"), y("y"), c("c");
    Func in("in");
    in(x,y,c) = input(clamp(x,0,input.width() - 1) , \
                      clamp(y,0,input.height() - 1), c);
    Func res = gray(in);

    struct timespec start_time;
	struct timespec end_time;

    //START PROCESSING IMAGE with HALIDE
    clock_gettime(CLOCK_REALTIME, &start_time);

    for(int i = 0; i < ITERS; i++){
        Buffer<uint8_t> output = res.realize(input.width(), input.height());
    }

    clock_gettime(CLOCK_REALTIME, &end_time);
    double accum = ( (double)end_time.tv_sec - (double)start_time.tv_sec )
                      + ((double)( end_time.tv_nsec - start_time.tv_nsec ))
                                 / 1E9L;

    printf("Halide: %lf\n", accum);

    Mat gray_image;

    //START PROCESSING IMAGE with OPENCV
    clock_gettime(CLOCK_REALTIME, &start_time);
    for(int i = 0; i < ITERS; i++){
        cvtColor(image, gray_image, CV_BGR2GRAY );
    }
    clock_gettime(CLOCK_REALTIME, &end_time);
    accum = ( (double)end_time.tv_sec - (double)start_time.tv_sec )
            + ((double)( end_time.tv_nsec - start_time.tv_nsec )) / 1E9L;

    printf("OpenCV: %lf\n", accum);
}
