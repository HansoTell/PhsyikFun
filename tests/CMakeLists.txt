file(GLOB TEST_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

foreach(test_file ${TEST_SOURCES})

    get_filename_component(test_name ${test_file} NAME_WE)

    add_executable(${test_name} ${test_file})

    target_link_libraries(${test_name}
        PRIVATE
            MyPhysicsLib
            GTest::gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(${test_name})

endforeach()
