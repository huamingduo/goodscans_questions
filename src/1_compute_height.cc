#include <iostream>

double ComputeHeight(const int& X, const int& M, const double& N) {
  if (X > M || M >= 100) {
    std::cout << "illegal X and/or M" << std::endl;
    return -1.;
  }
  int a = (X/10) > 3 ? 9 : 0;
  int b = ((X%10) > 3 ? 1 : 0) + (X/10);
  return static_cast<double>(X - a - b) * N;
}

int main(int argc, char** argv) {
  std::cout << "X = 33, M = 99, N = 3: h = " << ComputeHeight(33, 99, 3) << std::endl;
  std::cout << "X = 78, M = 99, N = 3: h = " << ComputeHeight(78, 99, 3) << std::endl;
  return 0;
}
