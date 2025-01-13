#ifndef TOY_LANG_CORE_EXCEPTIONS_HPP
#define TOY_LANG_CORE_EXCEPTIONS_HPP

#include "Types.hpp"

namespace tl {
  // todo: show block of code causing the error
  class ToyLangException : public std::runtime_error {
  public:
    explicit ToyLangException(CRef<String> mesg): std::runtime_error(mesg) {
    }

    explicit ToyLangException(CRef<String> filepath, CRef<String> mesg)
      : std::runtime_error("[" + filepath + " @0:0]" + mesg) {
    }

    ToyLangException(
      CRef<String> filepath, const u64 line, const u64 column, CRef<String> mesg
    ): std::runtime_error(
         "[" + filepath + " @" + std::to_string(line) + ":" +
         std::to_string(column) + "] " + mesg
       ), m_filepath(filepath), m_line(line), m_column(column) {
    }

    auto filepath() const -> CRef<String> {
      return m_filepath;
    }

    auto line() const -> u64 {
      return m_line;
    }

    auto column() const -> u64 {
      return m_column;
    }

  private:
    String m_filepath;
    u64 m_line = 0;
    u64 m_column = 0;
  };

  struct InternalException final : ToyLangException {
    explicit InternalException(CRef<String> mesg) : ToyLangException(mesg) {
    }

    InternalException(CRef<String> filepath, CRef<String> mesg)
      : ToyLangException(filepath, mesg) {
    }

    InternalException(
      CRef<String> filepath, const u64 line, const u64 column, CRef<String> mesg
    ): ToyLangException{filepath, line, column, "internal error: " + mesg} {
    }
  };

  class ExpceptionCollector {
  public:
    auto size() const -> sz {
      return m_collected.size();
    }

    auto empty() const -> sz {
      return m_collected.empty();
    }

    // todo: handle out of memory
    auto add(ToyLangException &&e) -> void {
      m_collected.emplace_back(std::make_unique<ToyLangException>(std::move(e)));
    }

    auto throwAllIfExists() -> void {
      if (empty()) {
        return;
      }

      String combinedMesg;

      // todo: handle out of memory
      for (auto &e: m_collected) {
        combinedMesg += e->what() + "\n"s;
        e.reset();
      }
      m_collected.clear();

      throw ToyLangException(combinedMesg);
    }

  private:
    Vec<Ptr<ToyLangException> > m_collected;
  };
}

#endif // TOY_LANG_CORE_EXCEPTIONS_HPP
