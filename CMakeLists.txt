cmake_minimum_required(VERSION 3.31.6)
project(GameOfLife)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# gdb
#set(CMAKE_BUILD_TYPE Debug)

# Prevent GLAD from including GLFW
# Otherwise it won't compile if GLAD is included after GLFW
#define GLFW_INCLUDE_NONE
add_compile_definitions(GLFW_INCLUDE_NONE)

find_package(glfw3 REQUIRED)
#find_package(assimp REQUIRED)
include_directories(include)

add_executable(main src/main.cpp src/glad.c src/shaders.cpp)

#target_link_libraries(mapMaker PRIVATE glfw)
target_link_libraries(main PRIVATE glfw)
