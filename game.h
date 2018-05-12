#ifndef GAME_H_
#define GAME_H_

void initializeBoard(cell board[NUM_OF_ROWS][NUM_OF_COLUMNS]);

void printBoard(cell board[NUM_OF_ROWS][NUM_OF_COLUMNS]);

void executeCommand(char *parsedCommand[4], cell board[NUM_OF_ROWS][NUM_OF_COLUMNS], char* command);

#endif /* GAME_H_ */