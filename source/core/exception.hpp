#ifndef TLC_CORE_EXCEPTION_HPP
#define TLC_CORE_EXCEPTION_HPP

#include "type.hpp"
#include "singleton.hpp"

namespace tlc {
  // todo: show block of code causing the error
  class Exception : public std::runtime_error {
  public:
    explicit Exception(Str const &mesg): std::runtime_error(mesg) {
    }

    explicit Exception(Str const &filepath, Str const &mesg)
      : std::runtime_error("[" + filepath + " @0:0]" + mesg) {
    }

    Exception(
      Str const &filepath, const u64 line, const u64 column, Str const &mesg
    ): std::runtime_error(
         "[" + filepath + " @" + std::to_string(line) + ":" +
         std::to_string(column) + "] " + mesg
       ), m_filepath(filepath), m_line(line), m_column(column) {
    }

    auto filepath() const -> StrV {
      return m_filepath;
    }

    auto line() const -> u64 {
      return m_line;
    }

    auto column() const -> u64 {
      return m_column;
    }

  private:
    Str m_filepath;
    u64 m_line = 0;
    u64 m_column = 0;
  };

  struct InternalException final : Exception {
    explicit InternalException(Str const &mesg) : Exception(mesg) {
    }

    InternalException(Str const &filepath, Str const &mesg)
      : Exception(filepath, mesg) {
    }

    InternalException(
      Str const &filepath, const u64 line, const u64 column, Str const &mesg
    ): Exception{filepath, line, column, "internal error: " + mesg} {
    }
  };

  class GlobalExpceptionCollector : public Singleton<GlobalExpceptionCollector> {
  public:
    auto size() const -> sz {
      return m_collected.size();
    }

    auto empty() const -> sz {
      return m_collected.empty();
    }

    // todo: handle out of memory, !InternalException
    auto add(Exception e) -> void {
      m_collected.emplace_back(std::make_unique<Exception>(std::move(e)));
    }

    auto throwAllIfExists() -> void {
      if (empty()) {
        return;
      }

      Str combinedMesg;

      // todo: handle out of memory
      for (auto &e: m_collected) {
        combinedMesg += e->what() + "\n"s;
        e.reset();
      }
      m_collected.clear();

      throw Exception(combinedMesg);
    }

  protected:
    GlobalExpceptionCollector() = default;

  private:
    Vec<Ptr<Exception> > m_collected;
  };
}

#endif // TLC_CORE_EXCEPTION_HPP
