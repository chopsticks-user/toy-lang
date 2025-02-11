#include "driver/driver.hpp"
#include "lex/lex.hpp"
#include "syntax/syntax.hpp"

#include <print>

int protected_main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  std::print("tlc\n");
  return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
  try {
    return protected_main(argc, argv);
  } catch (std::exception &e) {
    std::print(stderr, "{}\n", e.what());
  } catch (...) {
    std::print(stderr, "Uncaught exception\n");
  }
  return EXIT_FAILURE;
}
