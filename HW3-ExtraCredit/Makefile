CC=dpcpp
CFLAGS=-std=c++17 -fsycl -lOpenCL

all: matrixec

matrixec: matrixec.cpp
        $(CC) matrixec.cpp  -o matrixec $(CFLAGS)
run: matrixec
        ./matrixec 1024 sycl
clean:
        rm -rf matrixec