#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

#include <iostream>

using json = nlohmann::json;

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  // create JSON arrays
  json j_no_init_list = json::array();
  json j_empty_init_list = json::array({});
  json j_nonempty_init_list = json::array({1, 2, 3, 4});
  json j_list_of_pairs = json::array({{"one", 1}, {"two", 2}});

  // serialize the JSON arrays
  std::cout << j_no_init_list << '\n';
  std::cout << j_empty_init_list << '\n';
  std::cout << j_nonempty_init_list << '\n';
  std::cout << j_list_of_pairs << '\n';
  return 0;
}
