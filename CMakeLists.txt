cmake_minimum_required(VERSION 3.0.0)
project(NewtonIterationFractal VERSION 0.1.0)
add_compile_options("/openmp")
add_compile_options("/O2")

add_executable(NewtonIterationFractal main.cpp)
include_directories(lib)
link_libraries("EasyXa")
link_libraries("EasyXw")
add_compile_options("/openmp")
add_compile_options("/O2")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})