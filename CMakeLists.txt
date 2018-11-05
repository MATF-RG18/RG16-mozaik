cmake_minimum_required(VERSION 3.10)
project(rg16_mozaik)

set(CMAKE_CXX_STANDARD 14)

link_libraries(GL)
link_libraries(GLEW)
link_libraries(glfw)

add_executable(rg16_mozaik src/main.cpp)
