#pragma once

#include <verp/verp>

#include <commline/commline>

namespace commline {
    template <>
    struct parser<verp::level> {
        static auto parse(std::string_view argument) -> verp::level;
    };

    template <>
    struct parser<verp::version> {
        static auto parse(std::string_view argument) -> verp::version;
    };
}
