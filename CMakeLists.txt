cmake_minimum_required(VERSION 3.10)
project(CursesTetris)

find_package(Curses REQUIRED)

add_executable(tetris main.c)

target_link_libraries(tetris ${CURSES_LIBRARIES})
target_include_directories(tetris PRIVATE ${CURSES_INCLUDE_DIR})
target_compile_definitions(tetris PRIVATE NCURSES_STATIC)