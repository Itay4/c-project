
#ifndef GAME_H
#define GAME_H
#include "linked_list.h"

cell **generateEmptyBoard();

/*void initializeBoard(cell board[NUM_OF_ROWS][NUM_OF_COLUMNS]);*/

void printBoard(cell **board);

void executeCommand(char *parsedCommand[4], cell **board, char* command, int counter, char mode, list *lst);

void gameOver(cell **board);

bool valInColumn(cell **board, int column, int row, int val);

bool valInRow(cell **board, int column, int row, int val);

bool valInBlock(cell **board, int column, int row, int val);

bool validCheck(cell **board, int column, int row, int val);

int set(cell **board, int column, int row, int val, char mode);

/*void validate(cell **board);*/

bool checkBoardErroneous(cell **board);
void validateRisks(cell **board, int column, int row);
int setInSolveMode(cell **board, int column, int row, int val);
int setInEditMode(cell **board, int column, int row, int val);

void hint(cell **board, int column, int row);

cell **solveCommand(char* parsedCommand[4], char mode);

cell **editCommand(char* parsedCommand[4], char mode);

void markErrorsCommand(char* value);

void saveCommand(cell **board, char *filePath, char mode);

int autoFill(cell **board);

void numSolutions(cell **board);

int countSolutions(cell **board);

#endif /* GAME_H */
