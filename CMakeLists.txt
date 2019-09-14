cmake_minimum_required(VERSION 3.14)
project(sudokoProject C)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -pedantic-errors")
add_executable(sudokoProject main.c new_parser.c new_parser.h game.c game.h solver.c solver.h doublyLinkedList.c doublyLinkedList.h stack.c stack.h board.c board.h LP.c LP.h errors.c errors.h)