#include "opts.hpp"

#include <algorithm>
#include <commline/commline>
#include <verp/verp>

namespace ranges = std::ranges;

using namespace commline;

namespace {
    namespace internal {
        auto increment(
            const app& app,
            verp::level level,
            verp::version&& version
        ) -> void {
            version.increment(level);
            fmt::print("{}\n", version);
        }

        auto main(
            const app& app,
            bool reverse,
            std::vector<verp::version>&& versions
        ) -> void {
            if (versions.empty()) return;

            if (reverse) ranges::sort(versions, ranges::greater());
            else ranges::sort(versions);

            fmt::print("{}\n", fmt::join(versions, "\n"));
        }

        auto handle_error(std::exception_ptr eptr) -> void {
            try {
                if (eptr) std::rethrow_exception(eptr);
            }
            catch (const verp::invalid_version& ex) {
                fmt::print(stderr, "invalid version:\n");
                ex.print();
            }
            catch (const std::exception& ex) {
                fmt::print(stderr, "{}\n", ex.what());
            }
        }
    }
}

auto main(int argc, char** argv) -> int {
    using namespace commline;

    std::locale::global(std::locale(""));

    auto app = application(
        NAME,
        VERSION,
        "Validates and prints the given versions sorted by version precedence",
        options(flag({"r", "reverse"}, "Print versions in descending order")),
        arguments(variadic<verp::version>("versions")),
        internal::main
    );

    app.subcommand(command(
        "increment",
        "Increment a version by the specified level. "
        "Level can be one of: major, minor, or patch.",
        options(),
        arguments(
            required<verp::level>("level"),
            required<verp::version>("version")
        ),
        internal::increment
    ));

    app.on_error(&internal::handle_error);

    return app.run(argc, argv);
}
