cmake_minimum_required(VERSION 3.12)
include(pico_sdk_import.cmake)
project(ink)
pico_sdk_init()

# add a compilation subdirectory
add_subdirectory(lib/Config)
add_subdirectory(lib/e-Paper)
add_subdirectory(lib/Fonts)
add_subdirectory(lib/GUI)

# add a header directory
include_directories(./lib/Config)
include_directories(./lib/GUI)

add_library(ink STATIC ${})
target_link_libraries(ink ePaper GUI Fonts Config pico_stdlib hardware_spi)

