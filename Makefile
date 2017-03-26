CC = g++
CFLAGS = -g -I $(HALIDE_PATH)/cmake_build/include -I $(HALIDE_PATH)/tools -L \
		 $(HALIDE_PATH)/cmake_build/lib -L $(HALIDE_PATH)/cmake_build/bin -lHalide\
		 `pkg-config --cflags --libs libpng` -L /usr/lib/x86_64-linux-gnu\
		 -ljpeg  -lpthread -ldl

all: canny brightner

canny: canny.cpp
	$(CC) $^ $(CFLAGS) -o $@ -std=c++11

brightner: brightner.cpp
	$(CC) $^ $(CFLAGS) -o $@ -std=c++11

init:
	export HALIDE_PATH=$(HOME)/Proga/GSOC/Halide
	export LD_LIBRARY_PATH=$(HALIDE)/bin

clean:
	rm -rf canny brightner

.PHONY: canny clean brightner init
