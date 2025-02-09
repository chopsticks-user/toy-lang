#include <iostream>

class Enum {
  enum class ETest {
    v1, v2 = 10, v3, v4,
  };

  using enum ETest;

  static inline const tlc::HashMap<ETest, tlc::Str> _toStrTable = {
    {v1, "v1"}, {v2, "v2"}, {v3, "v3"}, {v4, "v4"},
  };

public:
  using enum ETest;

  constexpr Enum(ETest const value) : _value(value) {
  }

  constexpr operator tlc::i32() const noexcept {
    return static_cast<tlc::i32>(_value);
  }

  explicit constexpr operator tlc::Str() const noexcept {
    return _toStrTable.at(_value);
  }

private:
  ETest _value{};
};

#define Enum(name, ...) \
class E##name { \
enum class _E##name { \
__VA_ARGS__ \
}; \
public: \
using enum _E##name; \
\
constexpr E##name(_E##name const value) : _value(value) { \
} \
\
constexpr operator tlc::i32() const noexcept { \
return static_cast<tlc::i32>(_value); \
} \
\
private: \
_E##name _value{}; \
};

// static constexpr auto enumToStr() -> tlc::Str {
//   switch ()
// }

Enum(Test, v1, v2 = 5, v3 = 10);

int protected_main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  std::cout << "tlc\n";
  ETest test{ETest::v2};
  std::cout << static_cast<tlc::u32>(test) << '\n';
  std::cout << static_cast<tlc::Str>(test) << '\n';
  // std::cout << sizeof Enum::v2 << '\n';
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
