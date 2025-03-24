if (BUILD_TESTING)
    project(Frontend_test)

    add_executable(Frontend_test
            main.cpp
            LexerTestSuite.cpp
    )

    target_link_libraries(Frontend_test PRIVATE
            Frontend
            gtest
    )
    include(GoogleTest)
    gtest_discover_tests(Frontend_test)
endif ()
