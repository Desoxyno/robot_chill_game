cmake_minimum_required(VERSION 4.3.4)

project(game LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native")

find_package(raylib REQUIRED)

add_executable(game)

target_include_directories(game PRIVATE includes config)

target_compile_definitions(game PRIVATE
    $<$<CONFIG:Debug>:DEBUG_BUILD>
)

target_sources(game PRIVATE
    main.cpp
)
        
target_link_libraries(game PRIVATE
    raylib X11
)