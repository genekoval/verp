#include "opts.hpp"

namespace commline {
    auto parser<verp::level>::parse(std::string_view argument) -> verp::level {
        if (argument == "major") return verp::level::major;
        if (argument == "minor") return verp::level::minor;
        if (argument == "patch") return verp::level::patch;

        throw cli_error("invalid version level `{}`", argument);
    }

    auto parser<verp::version>::parse(std::string_view argument)
        -> verp::version {
        return verp::version {argument};
    }
}
