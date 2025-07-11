#ifndef TLC_CORE_FILE_HPP
#define TLC_CORE_FILE_HPP

#include "type.hpp"

#include <fstream>
#include <filesystem>

namespace tlc {
    namespace fs = std::filesystem;

    class FileReader final {
    public:
        FileReader() = default;

        explicit FileReader(fs::path const& filepath) {
            auto m_ifs = std::make_unique<std::ifstream>();
            m_ifs->open(filepath);

            // todo: specific exception
            if (!m_ifs->is_open()) {
                throw std::runtime_error("Failed to open " + filepath.string());
            }

            m_is = std::move(m_ifs);
        }

        explicit FileReader(std::istringstream iss)
            : m_is(std::make_unique<std::istringstream>(std::move(iss))) {}

        auto skipLine() const {
            std::string dummy;
            m_is->ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        auto advance() const -> c8 {
            return static_cast<c8>(m_is->get());
        }

        auto revert() const -> void {
            m_is->unget();
        }

        auto peek() const -> c8 {
            return static_cast<c8>(m_is->peek());
        }

        auto eof() const -> bool {
            return m_is->eof();
        }

    private:
        Ptr<std::istream> m_is;
    };
};


#endif // TLC_CORE_FILE_HPP
