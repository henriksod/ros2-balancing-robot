# Copyright (c) 2023, Henrik Söderlund

# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(hello_world)

target_include_directories(app PRIVATE include/)

FILE(GLOB app_sources src/**/**/*.cpp)

option(TESTS "Run tests in test/" OFF)

if(TESTS)
    target_sources(app PRIVATE test/main.cpp)

    # add here all class files for productive code
    target_sources(app PRIVATE ${app_sources})

else()
    target_sources(app PRIVATE src/main.cpp)

    # add here all class files for productive code
    target_sources(app PRIVATE ${app_sources})
endif()
unset(TESTS CACHE)