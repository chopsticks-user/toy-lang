#include "driver/driver.hpp"
#include "lex/lex.hpp"
#include "token/token.hpp"

#include <iostream>

int protected_main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  std::cout << "tlc\n";
  return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
  try {
    return protected_main(argc, argv);
  } catch (std::exception &e) {
    std::cerr << e.what() << '\n';
  } catch (...) {
    std::cerr << "Uncaught exception" << '\n';
  }
  return EXIT_FAILURE;
}
