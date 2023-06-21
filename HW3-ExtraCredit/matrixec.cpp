#include <CL/sycl.hpp>

#include <chrono>
#include <cmath>
#include <ctime>
#include <iostream>

using namespace cl::sycl;

class mxm_kernel;

void dis_mat(float* m, int matrixN);
void blk_host(float* M1, float* M2, float* M3, int matrixN);

//ptwo
inline int prevPT(int x) {
  if (x < 0) {
    return 0;
  }
  --x;
  x |= x >> 1;
  x |= x >> 2;
  x |= x >> 4;
  x |= x >> 8;
  x |= x >> 16;
  return x - (x >> 1);
}

inline bool isPT(int x) { return (x & (x - 1)) == 0; }

//
void usage(std::string programName) {
  std::cout << " Incorrect parameters " << std::endl;
  std::cout << " Usage: " << std::endl;
  std::cout << programName << " [matrix size] " << std::endl;
  std::cout << "[matrix size] :"
            << std::endl;
  std::cout << "[omp] "
            << " Default " << std::endl;
}

//mainfunction

int main(int argc, char* argv[]) {
  float* M1;
  float* M2;
  float* M3;
  bool sycl = true;
  bool omp = true;
  bool error = false;

  if (argc != 2 && argc != 3) {
    usage(argv[0]);
    return 1;
  }

  int matrixN = 0;
  try {
    matrixN = std::stoi(argv[1]); } catch (...) { usage(argv[0]); return 1; }

  if (matrixN < 32) { usage(argv[0]); return 1; }
if (argc == 3) { if (std::string(argv[2]) == "omp") { omp = true; }  else { usage(argv[0]); } }

  M1 = new float[matrixN * matrixN]; M2 = new float[matrixN * matrixN]; M3 = new float[matrixN * matrixN];

#pragma omp parallel for collapse(2)
  for (int i = 0; i < matrixN; i++) for (int j = 0; j < matrixN; j++) {
      M1[i * matrixN + j] = 0.0f; if (i == j) { M1[i * matrixN + j] = 1.0f; } M2[i * matrixN + j] = 2.0f; M3[i * matrixN + j] = 0.0f; }

  std::cout << " Input matrix " << std::endl;
  
  
  dis_mat(M1, matrixN); dis_mat(M2, matrixN); dis_mat(M3, matrixN);

  if (omp) {
#if defined(_OPENMP)
    std::cout << "OpenMP: ";
#else
    std::cout << "C++: ";
#endif

    {
      auto start = std::chrono::steady_clock::now();
      blk_host(M1, M2, M3, matrixN);
      auto end = std::chrono::steady_clock::now();
      auto time =
          std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
              .count();
      std::cout << "Time: " << time << std::endl;
      float flops =
          (2.0f * matrixN * matrixN * matrixN / (time / 1000.0f)) * 1.0e-9f;
      std::cout << "GFLOPs: " << flops << std::endl;

      bool error = false;
      // Testing
      for (int i = 0; i < matrixN; i++) for (int j = 0; j < matrixN; j++) {
          
          if (std::fabs(M3[i * matrixN + j] - M2[i * matrixN + j]) > 1e-8) {
            std::cout << " Pos " << i << ", " << j << " dif: " << M3[i * matrixN + j] << " != " << M2[i * matrixN + j] << std::endl;
            error = true; } } if (!error) { std::cout << "Success" << std::endl; } else { std::cout << " Error in the computation " << std::endl; } } }

 



  delete[] M1; delete[] M2; delete[] M3;

  return error ? 1 : 0;
}

//function to display matrices
void dis_mat(float* m, int matrixN) {
  if (matrixN > 16) {
    return; }

  std::cout << "=======" << std::endl;
  for (int i = 0; i < matrixN; i++) { for (int j = 0; j < matrixN; j++) { std::cout << m[i * matrixN + j] << " ";}
    std::cout << std::endl;} std::cout << "=======" << std::endl; }

void blk_host(float* M1, float* M2, float* M3, int matrixN) {
  int blk_s = 32; int nblks = blk_s / matrixN;
  int ecBlkLen = blk_s % matrixN; nblks = ecBlkLen ? (nblks + 1) : (nblks);

#pragma omp parallel for num_threads(2) collapse(2)
  for (int bII = 0; bII < matrixN; bII += blk_s)
    for (int bIJ = 0; bIJ < matrixN; bIJ += blk_s)
      for (int bIK = 0; bIK < matrixN; bIK += blk_s) {
        int i = bII;
        int j = bIJ;
        int k = bIK;
        for (int bi = i; bi < std::min(i + blk_s, matrixN); bi++)
          for (int bj = j; bj < std::min(j + blk_s, matrixN); bj++)
            for (int bk = k; bk < std::min(k + blk_s, matrixN); bk++) {
              M3[bi * matrixN + bj] +=
                  M1[bi * matrixN + bk] * M2[bk * matrixN + bj];
            }
      }
}