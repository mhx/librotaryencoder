#
# Copyright (c) Marcus Holland-Moritz
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
# OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.
#

project(librotaryencoder)

cmake_minimum_required(VERSION 3.10.0)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  add_compile_options(-fdiagnostics-color=always)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  add_compile_options(-fcolor-diagnostics)
endif()

list(
  APPEND
  COMMON_WARNING_FLAGS
  -pedantic
  -Wall
  -Wextra
  -Wcast-qual
  -Wcast-align
  -Wshadow
  -Wstrict-aliasing=1
  -Wswitch-enum
  -Wundef
  -Wpointer-arith
  -Wformat-security
  -Wvla
  -Wformat=2
  -Winit-self
  -Wfloat-equal
  -Wwrite-strings
  -Wredundant-decls
  -Wstrict-overflow=2
)

add_library(rotaryencoder
            src/encoder_internal_update_tt.c
            src/debounced_encoder_full_step.c
            src/debounced_encoder_half_step.c
            src/simple_encoder_full_step.c
            src/simple_encoder_half_step.c
            src/debounced_encoder_full_step_tt.c
            src/debounced_encoder_half_step_tt.c
            src/simple_encoder_full_step_tt.c
            src/simple_encoder_half_step_tt.c)
set_property(TARGET rotaryencoder PROPERTY C_STANDARD 90)

target_include_directories(rotaryencoder PUBLIC include)

target_compile_options(
  rotaryencoder
  PRIVATE ${COMMON_WARNING_FLAGS}
          -Wdeclaration-after-statement
          -Wstrict-prototypes
)

foreach(test debounced_encoder_full_step_test
             debounced_encoder_half_step_test
             simple_encoder_full_step_test
             simple_encoder_half_step_test
             compare_tt_test)
  add_executable(${test} test/${test}.c)
  set_property(TARGET ${test} PROPERTY C_STANDARD 99)

  target_include_directories(${test} PRIVATE include greatest)
  target_link_libraries(${test} rotaryencoder)

  target_compile_options(${test} PRIVATE ${COMMON_WARNING_FLAGS} -Wstrict-prototypes)

  add_test(NAME ${test} COMMAND ${test})
endforeach()

add_executable(cplusplus_test test/cplusplus_test.cpp)
set_property(TARGET cplusplus_test PROPERTY CXX_STANDARD 11)

target_include_directories(cplusplus_test PRIVATE include greatest)
target_link_libraries(cplusplus_test rotaryencoder)

target_compile_options(cplusplus_test PRIVATE ${COMMON_WARNING_FLAGS})

add_test(NAME cplusplus_test COMMAND cplusplus_test)

enable_testing()
