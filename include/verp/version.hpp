#pragma once

#include "parser.hpp"

namespace verp {
    class version {
        friend struct fmt::formatter<verp::version>;

        verp::core core;
        verp::git git;
    public:
        constexpr version() = default;

        explicit constexpr version(std::string_view version_string) {
            auto parser = detail::parser{version_string};

            parser.parse(core);
            parser.parse(git);

            parser.expect_eof();
        }

        constexpr auto operator<=>(
            const version& other
        ) const noexcept -> std::weak_ordering {
            const auto ordering = core <=> other.core;
            if (ordering != 0) return ordering;

            return git <=> other.git;
        }

        constexpr auto operator==(const version& other) const noexcept -> bool {
            return (*this <=> other) == 0;
        }

        constexpr auto commit() const noexcept -> std::string_view {
            return git.commit;
        }

        constexpr auto commits_ahead() const noexcept -> std::uint32_t {
            return git.commits_ahead;
        }

        constexpr auto increment(level level) noexcept -> version& {
            core.increment(level);
            git = verp::git();

            return *this;
        }

        constexpr auto major() const noexcept -> std::uint32_t {
            return core.major;
        }

        constexpr auto minor() const noexcept -> std::uint32_t {
            return core.minor;
        }
        constexpr auto patch() const noexcept -> std::uint32_t {
            return core.patch;
        }
    };
}

namespace fmt {
    template <>
    struct formatter<verp::version> {
        template <typename ParseContext>
        constexpr auto parse(ParseContext& ctx) const {
            return ctx.begin();
        }

        template <typename FormatContext>
        constexpr auto format(
            const verp::version& version,
            FormatContext& ctx
        ) const {
            auto it = format_to(ctx.out(), "{}", version.core);

            if (version.git.commits_ahead > 0) {
                it = format_to(ctx.out(), "{}", version.git);
            }

            return it;
        }
    };
}
