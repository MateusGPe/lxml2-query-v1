cmake_minimum_required(VERSION 3.10)

include(GoogleTest)

set(GTEST_DOWNLOAD_DIR "${CMAKE_CURRENT_BINARY_DIR}/googletest-download")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/gtest.cmake.in" "${GTEST_DOWNLOAD_DIR}/CMakeLists.txt")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/content.html" "${CMAKE_CURRENT_BINARY_DIR}/content.html" COPYONLY)

execute_process(
  COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY "${GTEST_DOWNLOAD_DIR}")

if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY "${GTEST_DOWNLOAD_DIR}")

if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker settings on Windows
set(gtest_force_shared_crt
    ON
    CACHE BOOL "" FORCE)

set(INSTALL_GTEST
    OFF
    CACHE BOOL "" FORCE)

add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build EXCLUDE_FROM_ALL)

file(GLOB_RECURSE TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(lxml2-query-test ${TEST_SOURCES})
target_link_libraries(lxml2-query-test gtest_main lxml2-query)

gtest_discover_tests(lxml2-query-test)
