CC = gcc
OBJS = main.o parser.o game.o solver.o LP.o doublyLinkedList.o stack.o errors.o board.o
EXEC = sudoku_console
COMP_FLAG = -ansi -O3 -Wall -Wextra -Werror -pedantic-errors
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@ -lm
main.o: main.c parser.h game.h solver.h LP.h cmdType.h doublyLinkedList.h stack.h errors.h def.h board.h status.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
game.o: game.c game.h doublyLinkedList.h board.h solver.h def.h errors.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
parser.o: parser.c parser.h game.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
solver.o: solver.c solver.h stack.h LP.h errors.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
doublyLinkedList.o: doublyLinkedList.c doublyLinkedList.h cmdType.h
	$(CC) $(COMP_FLAG) -c $*.c
LP.o: LP.c LP.h board.h errors.h cmdType.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
errors.o: errors.c errors.h def.h cmdType.h
	$(CC) $(COMP_FLAG) -c $*.c
stack.o: stack.c stack.h
	$(CC) $(COMP_FLAG) -c $*.c
board.o: board.c board.h status.h
	$(CC) $(COMP_FLAG) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)