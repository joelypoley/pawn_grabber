cmake_minimum_required(VERSION 3.10)

##############################################################################
# BEGIN: googletest stuff
##############################################################################
# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()
##############################################################################
# END: googletest stuff
##############################################################################

##############################################################################
# BEGIN: abseil-cpp stuff
##############################################################################
#set(CMAKE_CXX_FLAGS "-std=c++11 -Weverything ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

add_subdirectory(lib/abseil-cpp-master)
##############################################################################
# END: abseil-cpp stuff
##############################################################################

# According to cmake I should do this.
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)


#Added by Joel to enable testing
enable_testing()


# Now simply link against gtest or gtest_main as needed. Eg
add_executable(perft src/perft.cc src/board.cc )
target_link_libraries(perft absl::strings absl::base absl::algorithm absl::optional absl::algorithm)

add_executable(board_test src/board_test.cc src/board.cc )
target_link_libraries(board_test gtest_main absl::strings absl::base absl::algorithm absl::optional)
add_test(NAME board_test COMMAND board_test)