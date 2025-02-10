#include "driver/driver.hpp"
#include "lex/lex.hpp"

#include <iostream>

using tlc::sz;
using tlc::Str;
using tlc::StrV;
using tlc::Vec;
using tlc::Stack;
using tlc::Pair;
using tlc::Opt;

enum class EToken {
  IntL, FloatL, VarId, TypeId, Identifier,
  Invalid,
};

struct Token {
  EToken type;
  Str str;
};

class Stream {
public:
  using CondFn = bool (*)(char c);

  Stream(Str filename, Str source)
    : _filename(std::move(filename)), _source(std::move(source)) {
  }

  auto done() const noexcept -> bool {
    return _current >= _source.size();
  }

  auto current() const noexcept -> sz {
    return _current;
  }

  auto advanceUnless(CondFn const &condFn) -> sz {
    while (!done() && !condFn(_source[_current])) {
      ++_current;
    }
    return _current;
  }

  auto range(sz const start, sz const end) const -> Str {
    // todo: StrV
    return _source.substr(start, end - start);
  }

private:
  Str _filename;
  Str _source;
  sz _current{};
};

class Config {
public:
private:
  Str _holder;
};

class Context {
public:
  auto enter(EToken state, Stream const &stream) -> void {
    _state.emplace(state, stream.current());
  }

  auto exit(Stream const &stream) -> Opt<Token> {
    using enum EToken;

    auto const [state, start] = _state.top();
    auto const end = stream.current();
    _state.pop();

    if (start == end) {
      return {};
    }

    switch (state) {
      case IntL:
        return Token{.type = state, .str = stream.range(start, end)};
      default:
        return {};
    }
  }

private:
  Stack<Pair<EToken, sz> > _state;
  Vec<Token> _tokens;
};

class Panic {
};

class Lex {
public:
  Lex(Stream stream, Config config)
    : _stream(std::move(stream)), _config(std::move(config)) {
  }

  auto operator()() -> Vec<Token> {
    auto tokens = Vec<Token>{};

    _stream.advanceUnless(
      [](char const c) -> bool { return !std::isspace(c); }
    );
    while (!_stream.done()) {
      // todo: panic
      if (auto &&token = lexIntegerLiteral().value_or(
        lexIdentifier().value_or(
          Token{.type = Invalid, .str = ""}
        )
      ); token.type != Invalid) {
        tokens.emplace_back(token);
      }

      _stream.advanceUnless(
        [](char const c) -> bool { return !std::isspace(c); }
      );
    }
    return tokens;
  }

protected:
  using enum EToken;

  auto lexIntegerLiteral() -> Opt<Token> {
    _context.enter(IntL, _stream);
    _stream.advanceUnless(
      [](char const c) -> bool { return !std::isdigit(c); }
    );
    return _context.exit(_stream);
  }

  auto lexIdentifier() -> Opt<Token> {
    _context.enter(Identifier, _stream);
    _stream.advanceUnless(
      [](char const c) -> bool { return !std::isalnum(c); }
    );
    return _context.exit(_stream);
  }

private:
  Stream _stream;
  Config _config;
  Context _context{};
  Panic _panic{};
};

int protected_main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  std::cout << "tlc\n";

  auto lex = Lex{{"main.toy", "12346x   987"}, {}};
  auto const tokens = lex();

  std::cout << tokens.size() << '\n';
  std::cout << tokens[0].str << '\n';

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
//
// Created by frost on 2/10/25.
//
