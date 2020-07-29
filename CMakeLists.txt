# just need target_compile_options
cmake_minimum_required(VERSION 3.13)

project(perf LANGUAGES CXX)

include_directories(include)

set(PERF_SOURCES
src/perf_counting_event.cpp
src/event_group.cpp
)

add_executable(example1 bin/example1.cpp ${PERF_SOURCES})
add_executable(example2 bin/example2.cpp ${PERF_SOURCES})

macro(add_cxx_opts)
target_compile_options(${ARGV0} PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
endmacro()

add_cxx_opts(example1)
add_cxx_opts(example2)