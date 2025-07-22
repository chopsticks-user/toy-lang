#ifndef TLC_CORE_TYPE_HPP
#define TLC_CORE_TYPE_HPP

// fundamental types
#include <cfloat>
#include <cstdint>

// containers
#include <vector>
#include <string>
#include <array>
#include <span>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <queue>
#include <stack>

// misc
#include <variant>
#include <optional>
#include <any>
#include <numeric>
#include <sstream>
#include <memory>
#include <functional>
#include <expected>

namespace tlc {
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
    using cstr = char const*;
    using b8 = bool;
    using b32 = u32;
    using c8 = char;
    using szt = std::size_t;

    using Str = std::string;
    using StrV = std::string_view;

    using std::literals::operator ""s;
    using std::literals::operator ""sv;

    template <typename T>
    using Vec = std::vector<T>;

    template <typename T, szt N>
    using Arr = std::array<T, N>;

    template <typename T, szt N = std::dynamic_extent>
    using Span = std::span<T, N>;

    template <typename T>
    using Ptr = std::unique_ptr<T>;

    template <typename T>
    using SPtr = std::shared_ptr<T>;

    template <typename... T>
    using Poly = std::variant<std::monostate, T...>;

    template <typename T>
    using Opt = std::optional<T>;

    template <typename T>
    using Queue = std::queue<T>;

    template <typename T>
    using Stack = std::stack<T>;

    template <typename K, typename T>
    using HashMap = std::unordered_map<K, T>;

    template <typename K, typename T>
    using TreeMap = std::map<K, T>;

    template <typename T>
    using HashSet = std::unordered_set<T>;

    template <typename T>
    using TreeSet = std::set<T>;

    template <typename F, typename S>
    using Pair = std::pair<F, S>;

    template <typename... T>
    using Tpl = std::tuple<T...>;

    template <typename T, typename E>
    using Expected = std::expected<T, E>;

    template <typename E>
    using Unexpected = std::unexpected<E>;

    template <typename S>
    using Fn = std::function<S>;
}

#endif // TLC_CORE_TYPE_HPP
