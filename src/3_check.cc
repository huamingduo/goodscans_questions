#include <iostream>

bool Check(const int& M) {
  int forward{-1}, backward{-1};
  for (int i=0; i<32; ++i) {
    if (forward < 0 && M >> i & 1) {
      forward = i;
    } else if (forward >= 0 && M >> i & 1) {
      ++forward;
    } else if (forward >=0) {
      break;
    }
  }
  for (int i=31; i>-1; --i) {
    if (backward < 0 && M >> i & 1) {
      backward = i;
      break;
    }
  }
  return forward == backward;
}

int main(int argc, char** argv) {
  std::cout << "all ones are continues for 120? " << Check(120) << std::endl;
  std::cout << "all ones are continues for 184? " << Check(184) << std::endl;
  return 0;
}
