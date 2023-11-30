#pragma once

#include <string_view>

namespace verp::detail {
    enum class token_type { numeric, alphanumeric, dot, hyphen, eof, error };

    struct token {
        token_type type;
        std::string_view lexeme;
        std::size_t position;
    };

    class scanner {
        static constexpr auto is_alphanumeric(char c) noexcept -> bool {
            return is_digit(c) || (c >= 'a' && c <= 'z') ||
                   (c >= 'A' && c <= 'Z');
        }

        static constexpr auto is_digit(char c) noexcept -> bool {
            return c >= '0' && c <= '9';
        }

        std::string_view source;
        const char* start;
        const char* current;

        constexpr auto advance() noexcept -> char { return *current++; }

        constexpr auto alphanumeric() noexcept -> token {
            while (is_alphanumeric(peek())) advance();
            return make_token(token_type::alphanumeric);
        }

        constexpr auto error_token(std::string_view message) noexcept -> token {
            return token {
                .type = token_type::error,
                .lexeme = message,
                .position = position()};
        }

        constexpr auto is_at_end() const noexcept -> bool {
            return current == source.end();
        }

        constexpr auto length() const noexcept -> std::size_t {
            return current - start;
        }

        constexpr auto make_token(token_type type) noexcept -> token {
            return token {
                .type = type,
                .lexeme = std::string_view(start, current),
                .position = position()};
        }

        constexpr auto numeric() noexcept -> token {
            while (is_digit(peek())) advance();
            if (is_alphanumeric(peek())) return alphanumeric();

            if (length() > 1 && *start == '0')
                return error_token(
                    "numeric identifiers must not contain leading zeroes"
                );

            return make_token(token_type::numeric);
        }

        constexpr auto peek() const noexcept -> char { return *current; }

        constexpr auto position() const noexcept -> std::size_t {
            return start - source.begin();
        }
    public:
        explicit constexpr scanner(std::string_view source) :
            source(source),
            start(source.begin()),
            current(start) {
            if (*start == 'v') advance();
        }

        constexpr auto scan_token() noexcept -> token {
            start = current;

            if (is_at_end()) return make_token(token_type::eof);

            const char c = advance();

            if (is_digit(c)) return numeric();
            if (is_alphanumeric(c)) return alphanumeric();

            switch (c) {
                case '.': return make_token(token_type::dot);
                case '-': return make_token(token_type::hyphen);
            }

            return error_token("unexpected character");
        }

        constexpr auto source_string() const noexcept -> std::string_view {
            return source;
        }
    };
}
