#pragma once

#include <fmt/format.h>

namespace verp {
    enum class level : std::uint8_t { major, minor, patch };

    struct core {
        std::uint32_t major = 0;
        std::uint32_t minor = 0;
        std::uint32_t patch = 0;

        constexpr auto operator<=>(const core other) const noexcept
            -> std::weak_ordering {
            if (major != other.major) return major <=> other.major;
            if (minor != other.minor) return minor <=> other.minor;

            return patch <=> other.patch;
        }

        constexpr auto operator==(const core other) const noexcept -> bool {
            return (*this <=> other) == 0;
        }

        constexpr auto increment(level level) noexcept -> core& {
            switch (level) {
                case level::major:
                    ++major;
                    minor = 0;
                    patch = 0;
                    break;
                case level::minor:
                    ++minor;
                    patch = 0;
                    break;
                case level::patch: ++patch; break;
            }

            return *this;
        }
    };

    struct git {
        std::uint32_t commits_ahead = 0;
        std::string commit;

        constexpr auto operator<=>(const git& other) const noexcept
            -> std::weak_ordering {
            return commits_ahead <=> other.commits_ahead;
        }

        constexpr auto operator==(const git& other) const noexcept -> bool {
            return (*this <=> other) == 0;
        }
    };
}

namespace fmt {
    template <>
    struct formatter<verp::core> {
        template <typename ParseContext>
        constexpr auto parse(ParseContext& ctx) const {
            return ctx.begin();
        }

        template <typename FormatContext>
        constexpr auto format(verp::core core, FormatContext& ctx) const {
            return fmt::format_to(
                ctx.out(),
                "{}.{}.{}",
                core.major,
                core.minor,
                core.patch
            );
        }
    };

    template <>
    struct formatter<verp::git> {
        template <typename ParseContext>
        constexpr auto parse(ParseContext& ctx) const {
            return ctx.begin();
        }

        template <typename FormatContext>
        constexpr auto format(const verp::git& git, FormatContext& ctx) const {
            return fmt::format_to(
                ctx.out(),
                "-{}-g{}",
                git.commits_ahead,
                git.commit
            );
        }
    };
}
