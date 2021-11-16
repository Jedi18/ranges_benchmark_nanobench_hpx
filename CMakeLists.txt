# Copyright (c) Srinivas Vasu 2021
# Copyright (c) Akhil Nair    2021
#
# SPDX-License-Identifier: BSL-1.0
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


cmake_minimum_required(VERSION 3.17)

project(RangesNBenchmark_HPX CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

include(FetchContent)

FetchContent_Declare(
    nanobench
    GIT_REPOSITORY https://github.com/martinus/nanobench.git
    GIT_TAG v4.1.0
    GIT_SHALLOW TRUE)

FetchContent_MakeAvailable(nanobench)

# Setting up dependencies
find_package(HPX REQUIRED)
find_package(range-v3 REQUIRED)

add_executable(main main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/include/nanobench.h)
target_include_directories(main PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(main PUBLIC HPX::hpx HPX::wrap_main range-v3 range-v3-concepts nanobench)
target_compile_options(main PRIVATE)
