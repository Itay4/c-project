
#ifndef SUDOKU90_GAME_H
#define SUDOKU90_GAME_H
#include "linked_list.h"

cell **generateEmptyBoard();

/*void initializeBoard(cell board[NUM_OF_ROWS][NUM_OF_COLUMNS]);*/

void printBoard(cell **board);

void executeCommand(char *parsedCommand[4], cell **board, char* command, int counter, char mode, list *lst);

void gameOver(cell **board);

bool valInColumn(cell **board, int column, int val);

bool valInRow(cell **board, int row, int val);

bool valInBlock(cell **board, int column, int row, int val);

bool validCheck(cell **board, int column, int row, int val);

int set(cell **board, int column, int row, int val);

/*void validate(cell **board);*/

void hint(cell **board, int column, int row);

cell **solveCommand(char* parsedCommand[4]);

cell **editCommand(char* parsedCommand[4]);

void markErrorsCommand(char* value);

void saveCommand(cell **board, char *filePath);

void autoFill(cell **board);

void numSolutions(cell **board);

int countSolutions(cell **board);

int countSolutionsRec(cell **board, int i, int j, int counter);

#endif /*SUDOKU90_GAME_H*/
