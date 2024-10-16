#ifndef TOY_LANG_CORE_TYPES_HPP
#define TOY_LANG_CORE_TYPES_HPP

#include <vector>
#include <string>
#include <string_view>
#include <cfloat>
#include <cstdint>
#include <unordered_map>
#include <variant>
#include <functional>
#include <numeric>
#include <array>
#include <unordered_set>
#include <regex>
#include <optional>
#include <ranges>
#include <fstream>
#include <queue>
#include <stack>
#include <thread>
#include <memory_resource>
#include <source_location>
#include <filesystem>

namespace tl {
  using i64 = std::int64_t;
  using i32 = std::int32_t;
  using i16 = std::int16_t;
  using i8 = std::int8_t;

  using u64 = std::uint64_t;
  using u32 = std::uint32_t;
  using u16 = std::uint16_t;
  using u8 = std::uint8_t;

  using f32 = float;
  using f64 = double;
  using cstr = char *;
  using b8 = bool;
  using b32 = u32;
  using c8 = char;
  using sz = std::size_t;

  using String = std::string;
  using StringView = std::string_view;
  using Strings = std::vector<std::string>;

  using std::literals::operator ""s;
  using std::literals::operator ""sv;
}

#endif // TOY_LANG_CORE_TYPES_HPP
