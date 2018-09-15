CC = gcc
OBJS = main.o main_aux.o parser.o stack.o game.o solver.o linked_list.o
EXEC = sudoku-console
COMP_FLAG = -ansi -Wall -Wextra \
-Werror -pedantic-errors
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@
main.o: main.c main_aux.h parser.h stack.h game.h solver.h linked_list.h 
	$(CC) $(COMP_FLAG) -c $*.c
main_aux.o: main_aux.c main_aux.h
	$(CC) $(COMP_FLAG) -c $*.c
parser.o: parser.c parser.h
	$(CC) $(COMP_FLAG) -c $*.c
linked_list.o: linked_list.c linked_list.h
	$(CC) $(COMP_FLAG) -c $*.c
stack.o: stack.c stack.h
	$(CC) $(COMP_FLAG) -c $*.c
game.o: game.c game.h
	$(CC) $(COMP_FLAG) -c $*.c
solver.o: solver.c solver.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c

clean:
	rm -f $(OBJS) $(EXEC)

all: $(EXEC)
