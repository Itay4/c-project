cmake_minimum_required(VERSION 3.12)
project(Sudoku90 C)

set(CMAKE_C_STANDARD 90)

add_executable(Sudoku90 main.c main_aux.c main_aux.h game.c game.h solver.c solver.h parser.c parser.h linked_list.c linked_list.h stack.c stack.h)