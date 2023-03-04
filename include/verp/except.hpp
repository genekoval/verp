#pragma once

#include "scanner.hpp"

#include <fmt/color.h>
#include <stdexcept>

namespace verp {
    class invalid_version : public std::invalid_argument {
        std::string source;
        int pos;
        int len;
    public:
        invalid_version(std::string_view source, detail::token token) :
            std::invalid_argument(token.lexeme.data()),
            source(source),
            pos(token.position),
            len(1)
        {}

        invalid_version(
            std::string_view source,
            detail::token token,
            const char* what_arg
        ) :
            std::invalid_argument(what_arg),
            source(source),
            pos(token.position),
            len(std::max(token.lexeme.size(), 1ul))
        {}

        auto print() const noexcept -> void {
            using namespace fmt::literals;

            fmt::print(stderr, "\n");
            fmt::print(stderr, "{:>{}}\n", source, source.size() + 4);
            fmt::print(
                stderr,
                fmt::fg(fmt::color::red),
                "{:>{start}}{:~<{len}} {}\n",
                "^",
                "",
                what(),
                "start"_a = pos + 5,
                "len"_a = len - 1
            );
            fmt::print(stderr, "\n");
        }

        auto length() const noexcept -> int {
            return len;
        }

        auto position() const noexcept -> int {
            return pos;
        }

        auto source_string() const noexcept -> std::string_view {
            return source;
        }
    };
}
