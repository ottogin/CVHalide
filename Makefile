CC = g++
CFLAGS = -g -Wall -std=c++11

IMGFLAGS = `pkg-config --cflags --libs libpng` -L /usr/lib/x86_64-linux-gnu\
			-ljpeg
HALIDEFLAGS = -I $(HALIDE_PATH)/cmake_build/include -I $(HALIDE_PATH)/tools -L \
		 		$(HALIDE_PATH)/cmake_build/lib -L $(HALIDE_PATH)/cmake_build/bin\
				-lHalide -lpthread -ldl

OPENCVFLAGS = `pkg-config --cflags --libs opencv`

all: canny brightner times

canny: canny.cpp libhalide_cv_utils.a
	$(CC) $^ $(CFLAGS) $(HALIDEFLAGS) $(IMGFLAGS) -o $@

brightner: brightner.cpp
	$(CC) $^ $(CFLAGS) $(HALIDEFLAGS) $(IMGFLAGS) -o $@

times: gray_filter_times_comparing.cpp libhalide_cv_utils.a
	$(CC) $^ $(CFLAGS) $(HALIDEFLAGS) $(IMGFLAGS) $(OPENCVFLAGS) -o $@

libhalide_cv_utils.a: halide_cv_utils.o
			ar rcs $@ $^

halide_cv_utils.o: halide_cv_utils.cpp
	$(CC) $(CFLAGS) $(IMGFLAGS) $(HALIDEFLAGS) -c -o $@ $<

libhalide_cv_utils.a canny.o brightner.o: halide_cv_utils.h



clean:
	rm -rf *.o *.a canny brightner times

.PHONY: canny clean brightner init times
