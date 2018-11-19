cmake_minimum_required(VERSION 3.10)
project(rg16_mozaik)

set(CMAKE_CXX_STANDARD 14)

link_libraries(GL)
link_libraries(GLEW)
link_libraries(glfw)

add_executable(rg16_mozaik src/main.cpp src/color_sphere.cpp src/color_sphere.hpp src/grid.hpp src/grid.cpp src/mozaik_globals.hpp src/geometry.cpp src/geometry.hpp)
