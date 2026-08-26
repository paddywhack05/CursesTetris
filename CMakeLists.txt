cmake_minimum_required(VERSION 3.10)

project(CursesTetris)
set(CURSES_NEED_NCURSESW TRUE)
set(CURSES_NEED_WIDE TRUE)
find_package(Curses REQUIRED)
# Set compiler flags
add_compile_definitions(NCURSESW_STATIC)

if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/highscore.txt")
file(TOUCH "${CMAKE_CURRENT_SOURCE_DIR}/highscore.txt")
endif()

configure_file(
    "${CMAKE_SOURCE_DIR}/highscore.txt"
    "${CMAKE_BINARY_DIR}/highscore.txt"
    COPYONLY
)
# Create executable
add_executable(tetris main.c)
# Link ncursesw
target_link_libraries(tetris ncursesw)