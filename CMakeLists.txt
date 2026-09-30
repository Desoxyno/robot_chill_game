cmake_minimum_required(VERSION 4.3.4)

project(game LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(raylib REQUIRED)

add_executable(game)

target_include_directories(game PRIVATE includes config)

target_sources(game PRIVATE
    main.cpp
)
        
target_link_libraries(game PRIVATE
    raylib X11
)