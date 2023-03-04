#pragma once

#include "components.hpp"
#include "except.hpp"
#include "scanner.hpp"

#include <stdexcept>

namespace verp::detail {
    class parser {
        detail::scanner scanner;
        token current = {};
        token previous = {};

        constexpr auto advance() -> void {
            previous = current;
            current = scanner.scan_token();

            if (current.type == token_type::error) {
                throw invalid_version(scanner.source_string(), current);
            }
        }

        constexpr auto alphanumeric(const char* error) -> std::string_view {
            if (check(token_type::alphanumeric) || check(token_type::numeric)) {
                advance();
                return previous.lexeme;
            }

            this->error(error);
        }

        constexpr auto consume(token_type type, const char* message) -> void {
            if (check(type)) {
                advance();
                return;
            }

            error(message);
        }

        constexpr auto check(token_type type) const noexcept -> bool {
            return current.type == type;
        }

        [[noreturn]]
        auto error(const char* message) const -> void {
            throw invalid_version(scanner.source_string(), current, message);
        }

        [[noreturn]]
        auto error_prev(const char* message) const -> void {
            throw invalid_version(scanner.source_string(), previous, message);
        }

        constexpr auto match(token_type type) -> bool {
            if (!check(type)) return false;
            advance();
            return true;
        }

        constexpr auto numeric(const char* error) -> std::uint32_t {
            consume(token_type::numeric, error);

            const auto token = previous.lexeme;

            auto it = token.rbegin();
            const auto end = token.rend();

            std::uint32_t result = 0;
            unsigned long multiplier = 1;

            while (it != end) {
                const auto current = *it++;
                const auto value = current - '0';
                result += value * multiplier;
                multiplier *= 10;
            }

            return result;
        }
    public:
        explicit constexpr parser(std::string_view source) : scanner(source) {
            advance();
        }

        constexpr auto expect_eof() -> void {
            if (match(token_type::eof)) return;
            error("unexpected token");
        }

        constexpr auto parse(core& core) -> void {
            core.major = numeric("expect major version");
            consume(token_type::dot, "expect '.' after major version");

            core.minor = numeric("expect minor version");
            consume(token_type::dot, "expect '.' after minor version");

            core.patch = numeric("expect patch version");
        }

        constexpr auto parse(git& git) -> void {
            if (match(detail::token_type::hyphen)) {
                git.commits_ahead = numeric("expect git commit count");

                consume(
                    token_type::hyphen,
                    "expect '-' separating git commit count and hash"
                );

                const auto commit = alphanumeric("expect git commit hash");

                if (!commit.starts_with('g')) {
                    error_prev("expect git commit hash prefixed by 'g'");
                }

                if (commit.size() == 1) {
                    error("git commit hash must not be empty");
                }

                git.commit = commit.substr(1);
            }
        }
    };
}
