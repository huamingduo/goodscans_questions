#include <iostream>
#include <array>
#include <algorithm>

int main(int argc, char** argv) {
  std::array<int, 10> digits;
  for (int i=0; i<10; ++i) {
    digits[i] = i;
  }

  for (int iter=0; iter<10; ++iter) {
    std::random_shuffle(digits.begin(), digits.end());
    for (int i=0; i<8; ++i) {
      std::cout << digits[i];
    }
    std::cout << std::endl;
  }
  return 0;
}
