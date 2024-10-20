include(FetchContent)

set(FMT_INSTALL ON)

FetchContent_Declare(commline
    GIT_REPOSITORY ../commline.git
    GIT_TAG        aea2cd146b654e28db102ba7c6b0489bfbc1ef57 # 0.4.0
)

FetchContent_Declare(fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG        a33701196adfad74917046096bf5a2aa0ab0bb50 # 9.1.0
)

FetchContent_MakeAvailable(commline fmt)

if(PROJECT_TESTING)
    FetchContent_Declare(GTest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        b10fad38c4026a29ea6561ab15fc4818170d1c10 # branch: main
    )

    FetchContent_MakeAvailable(GTest)
endif()
