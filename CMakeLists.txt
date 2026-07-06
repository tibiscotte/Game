cmake_minimum_required(VERSION 3.10)
project(Jeu1 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)

add_executable(Jeu1
src/main.cpp
includes/var.hpp
includes/fonc.hpp)

target_link_libraries(Jeu1 PRIVATE
SDL2::SDL2
SDL2_image::SDL2_image
SDL2_mixer::SDL2_mixer
)