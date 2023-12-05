cmake_minimum_required(VERSION 3.26)
project(synth)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "/cmake/portaudio")

set(CMAKE_CXX_STANDARD 14)

list(APPEND CMAKE_PREFIX_PATH /usr/local/lib/portaudio/)
find_package( portaudio REQUIRED )

include_directories(include)

add_executable(synth
        include/libportaudio.a
        src/main.cpp)

target_include_directories(synth PRIVATE /opt/portaudio/src/common/)
target_link_libraries(synth portaudio)
target_link_libraries(synth ncurses)