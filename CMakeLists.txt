cmake_minimum_required(VERSION 3.28)
project(http_server VERSION 0.0.1 LANGUAGES C)

# Export Compile Commands for Clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_BUILD_TYPE STREQUAL "")
  message(FATAL_ERROR "Build type not specified!")
endif()

add_library(common_properties INTERFACE)
target_compile_options(common_properties INTERFACE -Wall -Wextra)
target_compile_features(common_properties INTERFACE c_std_17)

enable_testing()
add_subdirectory(src/)
add_subdirectory(example/)
add_subdirectory(test/)
