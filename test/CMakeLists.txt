option(ENABLE_TESTING "Enable or disable tests with GoogleTest" OFF)
if(ENABLE_TESTING)
    include(FetchContent)
    FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/e2239ee6043f73722e7aa812a459f54a28552929.zip)
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()

    include_directories(../src ../src/drone)

    add_executable(library_test test.cpp)
    target_link_libraries(library_test
        gtest_main
        Bateria
        Drone
        S1000
    )

    include(GoogleTest)
    gtest_discover_tests(library_test)

    include(CTest)
    add_test(NAME BateriaTest
        COMMAND pytest test_bateria.py
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    )
    add_test(NAME DroneTest
        COMMAND pytest test_drone.py
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    )
    add_test(NAME S1000Test
        COMMAND pytest test_s1000.py
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    )

endif()
