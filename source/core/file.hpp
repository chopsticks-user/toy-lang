#ifndef TLC_CORE_FILE_HPP
#define TLC_CORE_FILE_HPP

#include "type.hpp"

#include <fstream>
#include <filesystem>

namespace tlc {
  namespace fs = std::filesystem;

  class FileReader final {
  public:
    explicit FileReader(std::filesystem::path filepath)
      : m_filepath(std::move(filepath)) {
      m_fs.open(m_filepath);

      if (!m_fs.is_open()) {
        throw std::runtime_error("Failed to open " + filepath.string());
      }
    }

    auto skipLine() {
      std::string dummy;
      m_fs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    auto advance() -> c8 {
      return static_cast<c8>(m_fs.get());
    }

    auto revert() -> void {
      m_fs.unget();
    }

    auto peek() -> c8 {
      return static_cast<c8>(m_fs.peek());
    }

    auto match(c8 expected) -> bool {
      if (eof() || m_fs.peek() != expected) {
        return false;
      }
      advance();
      return true;
    }

    auto match(c8 expected1, c8 expected2) -> bool {
      auto firstMatched = match(expected1);
      auto secondMatched = match(expected2);

      if (firstMatched && secondMatched) {
        return true;
      }

      if (firstMatched || secondMatched) {
        m_fs.unget();
      }

      return false;
    }

    auto isOpen() -> bool {
      return m_fs.is_open();
    }

    auto eof() -> bool {
      return m_fs.eof();
    }

  private:
    std::filesystem::path m_filepath;
    std::fstream m_fs;
  };
};


#endif // TLC_CORE_FILE_HPP
