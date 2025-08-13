#include "driver/driver.hpp"
#include "lex/lex.hpp"
#include "parse/parse.hpp"
#include "syntax/syntax.hpp"
#include "token/token.hpp"
#include "static/static.hpp"

#include <print>
#include <iostream>

int protected_main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    TLC_SCOPE_REPORTER();
    TLC_LOG(tlc::ELogLevel::Warning, "This is a warning");
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    return EXIT_SUCCESS;
}

int main(int argc, char** argv) {
    try {
        return protected_main(argc, argv);
    }
    catch (std::exception& e) {
        std::print(stderr, "{}\n", e.what());
    }
    catch (...) {
        std::print(stderr, "Uncaught exception\n");
    }
    return EXIT_FAILURE;
}
