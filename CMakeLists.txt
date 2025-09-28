cmake_minimum_required(VERSION 3.10)
project(sVisualizer C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)

add_executable(sv main.c)

target_include_directories(sv PRIVATE
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
)

target_link_libraries(sv
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
)

install(TARGETS sv DESTINATION /usr/local/bin)
install(FILES assets/sv.png DESTINATION /usr/share/icons)
install(FILES desktop/sVisualizer.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications)

target_compile_options(sv PRIVATE
    ${SDL2_CFLAGS_OTHER}
    ${SDL2_IMAGE_CFLAGS_OTHER}
)
