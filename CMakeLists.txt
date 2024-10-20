cmake_minimum_required(VERSION 3.27)

include(cmake/modules.cmake)
include(GitVersion)

project(verp
    VERSION ${GIT_VERSION_ABBREV}
    DESCRIPTION "C++ version parsing library and command line utility"
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS NO)

include(ProjectTesting)
include(packages.cmake)

add_library(verp INTERFACE)
add_library(verp::verp ALIAS verp)

add_executable(verp-cli "")

target_sources(verp PUBLIC FILE_SET HEADERS BASE_DIRS include)
target_link_libraries(verp
    INTERFACE
        fmt::fmt
)

configure_file(src/verp/main.hpp.in main.hpp @ONLY)
set_target_properties(verp-cli PROPERTIES RUNTIME_OUTPUT_NAME verp)
target_include_directories(verp-cli PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
target_link_libraries(verp-cli
    PRIVATE
        commline::commline
        verp
)

if(PROJECT_TESTING)
    add_executable(verp.test "")

    target_link_libraries(verp.test
        PRIVATE
            GTest::gtest_main
            verp
    )

    add_test("Unit Tests" verp.test)
endif()

add_subdirectory(include)
add_subdirectory(src)

include(Sanitizers)
enable_asan()

include(EasyInstall)
easy_install(
    TARGETS
        verp
        verp-cli
    DEPENDENCIES
        fmt
)

if(PROJECT_IS_TOP_LEVEL)
    add_custom_target(docker
        docker build --progress=plain --no-cache .
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
        USES_TERMINAL
    )
endif()
