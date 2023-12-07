cmake_minimum_required(VERSION 3.26)
project(synth)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "/cmake/portaudio")

set(CMAKE_CXX_STANDARD 14)

list(APPEND CMAKE_PREFIX_PATH /usr/local/lib/portaudio/)
list(APPEND CMAKE_PREFIX_PATH  /usr/local/lib/wx/)

set(wxWidgets_ROOT_DIR /usr/local/lib/wx/)
file(TO_NATIVE_PATH "${wxWidgets_ROOT_DIR}" wxWidgets_ROOT_DIR)
set(wxWidgets_ROOT_DIR ${wxWidgets_ROOT_DIR} CACHE INTERNAL "wxWidgets installation dir")

set (ENV_WX_CONFIG /usr/local/lib/wx/bin/wx-config)
file (TO_NATIVE_PATH "${ENV_WX_CONFIG}" ENV_WX_CONFIG)
set(ENV_WX_CONFIG ${ENV_WX_CONFIG} CACHE INTERNAL "wx-config dir")

find_package( portaudio REQUIRED )
find_package(wxWidgets REQUIRED COMPONENTS net gl core base)

include_directories(include)

add_executable(synth
        include/libportaudio.a
        src/main.cpp
        src/App.cpp
        src/Frame.cpp
        src/Frame.h
        src/player/NoiseMaker.cpp
        src/player/NoiseMaker.h)

target_include_directories(synth PRIVATE /opt/portaudio/src/common/)
target_link_libraries(synth portaudio)
target_link_libraries(synth ncurses)

if(wxWidgets_FOUND)
    include(${wxWidgets_USE_FILE})
    # and for each of your dependent executable/library targets:
    target_link_libraries(synth ${wxWidgets_LIBRARIES})
endif()