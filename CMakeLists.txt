cmake_minimum_required(VERSION 3.29)
project(ece_arena C)

set(CMAKE_C_STANDARD 23)

add_executable(ece_arena main.c
                         map.c
                         header.h)

target_link_libraries(ece_arena -lmingw32 -lalleg44)
