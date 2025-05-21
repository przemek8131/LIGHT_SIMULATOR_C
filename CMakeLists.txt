cmake_minimum_required(VERSION 3.26.1)
project(LIGHT_SIMULATOR C)

set(CMAKE_C_STANDARD 99)

add_executable(${CMAKE_PROJECT_NAME} main.c cross.c)