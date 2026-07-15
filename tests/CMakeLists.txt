include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# We will create test files later, so we just declare an executable that links gtest_main for now
# We will use main_test.cpp and other test files.
file(GLOB_RECURSE TEST_SOURCES "*.cpp")
add_executable(SmartFileIndexer_tests ${TEST_SOURCES})
target_link_libraries(SmartFileIndexer_tests
  PRIVATE
    SmartFileIndexer_lib
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(SmartFileIndexer_tests)
