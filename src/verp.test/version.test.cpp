#include <verp/version.hpp>

#include <gtest/gtest.h>

using namespace std::literals;

using verp::invalid_version;
using verp::level;
using verp::version;

namespace {
    constexpr auto ver = version {"0.2.1-18-gcea375e"};

    static_assert(ver.major() == 0);
    static_assert(ver.minor() == 2);
    static_assert(ver.patch() == 1);
    static_assert(ver.commits_ahead() == 18);
    static_assert(ver.commit() == "cea375e");

    static_assert(version {"1.0.0"} < version {"2.0.0"});
    static_assert(version {"0.1.0"} < version {"0.2.0"});
    static_assert(version {"0.0.1"} < version {"0.0.2"});

    static_assert(version {"2.10.32"} < version {"2.10.32-10-gabcdf"});
    static_assert(version {"0.2.1-8-gcea375e"} < ver);

    static_assert(version {ver}.increment(level::major) == version {"1.0.0"});
    static_assert(version {ver}.increment(level::minor) == version {"0.3.0"});
    static_assert(version {ver}.increment(level::patch) == version {"0.2.2"});
}

TEST(Invalid, LeadingZero) {
    EXPECT_THROW(version {"3.14.015"}, invalid_version);
}
