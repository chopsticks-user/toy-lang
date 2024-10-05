#include <spdlog/spdlog.h>

#include <iostream>

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  spdlog::debug("Toy Lang");
  std::cout << "Toy Lang\n";
  return 0;
}
