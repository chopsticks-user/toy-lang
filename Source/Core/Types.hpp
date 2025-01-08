#ifndef TOY_LANG_CORE_TYPES_HPP
#define TOY_LANG_CORE_TYPES_HPP

#include <vector>
#include <string>
#include <string_view>
#include <cfloat>
#include <cstdint>
#include <unordered_map>
#include <set>
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
#include <sstream>

namespace rv = std::ranges::views;
namespace rng = std::ranges;
namespace fs = std::filesystem;

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
  using cstr = const char *;
  using b8 = bool;
  using b32 = u32;
  using c8 = char;
  using sz = std::size_t;

  using String = std::string;
  using StringView = std::string_view;
  using Strings = std::vector<std::string>;

  using Str = std::string;
  using StrView = std::string_view;

  using std::literals::operator ""s;
  using std::literals::operator ""sv;

  template<typename T>
  using Vec = std::vector<T>;

  template<typename T, sz N>
  using Arr = std::array<T, N>;

  template<typename T>
  using Ptr = std::unique_ptr<T>;

  template<typename T>
  using SPtr = std::shared_ptr<T>;

  template<typename... T>
  using Poly = std::variant<std::monostate, T...>;

  template<typename T>
  using CRef = const T &;

  template<typename T>
  using Opt = std::optional<T>;

  template<typename T>
  using Queue = std::queue<T>;

  template<typename T>
  using Stack = std::stack<T>;

  template<typename K, typename T>
  using HashMap = std::unordered_map<K, T>;

  template<typename K, typename T>
  using TreeMap = std::map<K, T>;

  template<typename T>
  using HashSet = std::unordered_set<T>;

  template<typename T>
  using TreeSet = std::set<T>;
}

#endif // TOY_LANG_CORE_TYPES_HPP
