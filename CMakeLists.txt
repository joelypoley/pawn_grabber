cmake_minimum_required(VERSION 3.7)
# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                 ${CMAKE_BINARY_DIR}/googletest-build)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()


#Added by Joel to enable testing
enable_testing()

# Turn on all warnings
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything")


# Now simply link against gtest or gtest_main as needed. Eg
add_executable(board_tests tests/board_tests.cpp board.cpp transposition_table.cpp)
set_property(TARGET board_tests PROPERTY CXX_STANDARD 14)
set_property(TARGET board_tests PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(board_tests gtest_main)
add_test(NAME board_tests_name COMMAND board_tests)