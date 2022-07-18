cmake_minimum_required(VERSION 3.18)
include(cmake/ClangFormat.cmake)

# Basic project settings
project(rastrum VERSION 0.1
                DESCRIPTION "A toy software rasterizer."
                LANGUAGES CXX)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Configure clang tidy to run if it is installed
find_program(
  CLANG_TIDY_EXE
  NAMES "clang-tidy"
  DOC "Path to clang-tidy executable"
)
if (CLANG_TIDY_EXE)
  set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE};-checks=bugprone-*,clang-analyzer-*,coreguidelines-*,misc-*,modernize-*,performance-*,portability-*,readability-*")
else()
  message(WARNING "clang-tidy not found, it won't be run.")
endif()

# Add the project is split into the rastrum library (src) and
# example applications (apps).
add_subdirectory(apps)
add_subdirectory(src)
