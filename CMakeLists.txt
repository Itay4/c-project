cmake_minimum_required(VERSION 3.12)
project(NEW_SUDUKO C)

set(CMAKE_C_STANDARD 99)

add_executable(NEW_SUDUKO main.c main_aux.h main_aux.c game.c game.h solver.c solver.h parser.c parser.h stack.c stack.h linked_list.h linked_list.c)