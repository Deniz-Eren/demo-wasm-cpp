##
# \file     CMakeLists.txt
#
# Copyright (C) 2025 Deniz Eren (deniz.eren@outlook.com)
#
# This is free and unencumbered software released into the public domain.
#
# Anyone is free to copy, modify, publish, use, compile, sell, or
# distribute this software, either in source code form or as a compiled
# binary, for any purpose, commercial or non-commercial, and by any
# means.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
#
# For more information, please refer to <https://unlicense.org/>
#

cmake_minimum_required(VERSION 3.13)
project(DrawCanvas CXX)

set(CMAKE_CXX_STANDARD 17)

# Source files
set(SOURCES
    draw.cpp
)

# Build target
add_executable(draw ${SOURCES})

# Use .js suffix instead of .html
set_target_properties(draw PROPERTIES
    COMPILE_FLAGS ""  # no compiler flags
    LINK_FLAGS "-O2 --bind -s NO_EXIT_RUNTIME=1 -lembind"
    RUNTIME_OUTPUT_NAME "draw"
    SUFFIX ".js"
)

# Copy index.html to the build folder
configure_file(
    ${CMAKE_SOURCE_DIR}/index.html  # source
    ${CMAKE_BINARY_DIR}/index.html  # destination
    COPYONLY                        # just copy, no variable replacement
)

