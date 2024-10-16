#include "CommandParser.hpp"
#include "ConfigParser.hpp"

#include "FrontEnd/FrontEnd.hpp"
#include "Syntax/Syntax.hpp"

#include <iostream>

int protected_main(int argc, char **argv) {
  using namespace tl::syntax;

  [[maybe_unused]] auto commandParser = tl::builder::CommandParser{};
  [[maybe_unused]] auto configParser = tl::builder::ConfigParser{};

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
