cmake_minimum_required(VERSION 3.20)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(
    FATAL_ERROR
      "Do not build in-source. Please remove CMakeCache.txt and the CMakeFiles/ directory. Then build out-of-source."
  )
endif()

project(
  "lxml2-query"
  VERSION 0.3.0
  LANGUAGES C CXX)

set(PROJECT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(PROJECT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source")

include(CTest)
include(CMakeDependentOption)
include(GNUInstallDirs)
include(InstallRequiredSystemLibraries)
include(CMakePackageConfigHelpers)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(LTO)
include(Warnings)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

find_lto(CXX)

add_library(
  ${PROJECT_NAME}
  ${PROJECT_SOURCE_DIR}/AttributeSelector.cpp
  ${PROJECT_SOURCE_DIR}/BinarySelector.cpp
  ${PROJECT_SOURCE_DIR}/Document.cpp
  ${PROJECT_SOURCE_DIR}/Lexer.cpp
  ${PROJECT_SOURCE_DIR}/Node.cpp
  ${PROJECT_SOURCE_DIR}/Parser.cpp
  ${PROJECT_SOURCE_DIR}/QueryResult.cpp
  ${PROJECT_SOURCE_DIR}/Selector.cpp
  ${PROJECT_SOURCE_DIR}/TagSelector.cpp
  ${PROJECT_SOURCE_DIR}/TextSelector.cpp
  ${PROJECT_SOURCE_DIR}/UnarySelector.cpp)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_compile_options(
  ${PROJECT_NAME}
  PRIVATE $<$<CONFIG:Debug>:-O0;-g;-Wall;-Wextra>
          $<$<CONFIG:Release>:-O3;-DNDEBUG>
          $<$<CONFIG:RelWithDebInfo>:-O2;-g;-DNDEBUG;-Wall;-Wextra>
          $<$<CONFIG:MinSizeRel>:-Os;-DNDEBUG;-Wall;-Wextra>)

target_include_directories(${PROJECT_NAME}
                           PUBLIC $<BUILD_INTERFACE:${PROJECT_INCLUDE_DIR}>)

set(LXQ_PUBLIC_HEADERS
    "${PROJECT_INCLUDE_DIR}/AttributeSelector.h"
    "${PROJECT_INCLUDE_DIR}/BinarySelector.h"
    "${PROJECT_INCLUDE_DIR}/Document.h"
    "${PROJECT_INCLUDE_DIR}/Helpers.h"
    "${PROJECT_INCLUDE_DIR}/Lexer.h"
    "${PROJECT_INCLUDE_DIR}/lxml2-query.h"
    "${PROJECT_INCLUDE_DIR}/Node.h"
    "${PROJECT_INCLUDE_DIR}/Parser.h"
    "${PROJECT_INCLUDE_DIR}/QueryResult.h"
    "${PROJECT_INCLUDE_DIR}/Selector.h"
    "${PROJECT_INCLUDE_DIR}/TagSelector.h"
    "${PROJECT_INCLUDE_DIR}/TextSelector.h"
    "${PROJECT_INCLUDE_DIR}/UnarySelector.h")

set_target_properties(
  ${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${LXQ_PUBLIC_HEADERS}" DEBUG_POSTFIX
                                                                   "d")

find_package(LibXml2 REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC LibXml2::LibXml2)

add_library(mgp::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

option(LXML2_QUERY_INSTALL "Enable installation" OFF)

if(${PROJECT_NAME}_INSTALL)
  set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
  include(CPack)

  set(CPACK_GENERATOR "ZIP")
  set(CPACK_PACKAGE_NAME "lxml2-query")
  set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
  set(CPACK_PACKAGE_DESCRIPTION "lxml2-query library")
  set(CPACK_PACKAGE_INSTALL_DIRECTORY "lxml2-query")
  set(CPACK_SOURCE_IGNORE_FILES "/.git/" "/build/" "/\\.vscode/")

  set(${PROJECT_NAME}_INSTALL_CMAKEDIR
      "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
      CACHE STRING "Path to install ${PROJECT_NAME} Config*.cmake files to.")
  set(${PROJECT_NAME}_MODULE_INSTALL_DIR
      "${CMAKE_INSTALL_LIBDIR}/cmake"
      CACHE STRING "Path to install ${PROJECT_NAME}'s .cmake module files to.")

  write_basic_package_version_file(
    "${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion)

  configure_package_config_file(
    "${CMAKE_MODULE_PATH}/${PROJECT_NAME}Config.cmake.in"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION "${${PROJECT_NAME}_INSTALL_CMAKEDIR}"
    PATH_VARS ${PROJECT_NAME}_MODULE_INSTALL_DIR)

  install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}_Targets
    INCLUDES
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

  install(
    EXPORT ${PROJECT_NAME}_Targets
    NAMESPACE ${PROJECT_NAME}::
    FILE "${PROJECT_NAME}Targets.cmake"
    DESTINATION "${${PROJECT_NAME}_INSTALL_CMAKEDIR}")

  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
                "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
          DESTINATION "${${PROJECT_NAME}_INSTALL_CMAKEDIR}")
endif()

option(LXML2_QUERY_TESTS "Enable tests" OFF)

if(LXML2_QUERY_TESTS)
  add_subdirectory(tests)
endif()
