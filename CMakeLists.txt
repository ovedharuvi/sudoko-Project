cmake_minimum_required(VERSION 3.14)
project(sudokoProject C)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -pedantic-errors")
add_executable(sudokoProject main.c parser.c parser.h game.c game.h doublyLinkedList.c doublyLinkedList.h check.h check.c board.c board.h errors.c errors.h stack.c stack.h status.h cmdType.h)