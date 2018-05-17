#ifndef GAME_H_
#define GAME_H_

void initializeBoard(cell board[NUM_OF_ROWS][NUM_OF_COLUMNS]);

void printBoard(cell board[NUM_OF_ROWS][NUM_OF_COLUMNS]);

void executeCommand(char *parsedCommand[4], cell user_board[NUM_OF_ROWS][NUM_OF_COLUMNS],cell solved_board[NUM_OF_ROWS][NUM_OF_COLUMNS], char* command);

void gameOver(cell board[NUM_OF_ROWS][NUM_OF_COLUMNS]);

bool valInColumn(cell board[NUM_OF_ROWS][NUM_OF_COLUMNS], int column, int val);

bool valInRow(cell board[NUM_OF_ROWS][NUM_OF_COLUMNS], int row, int val);

bool valInBlock(cell board[NUM_OF_ROWS][NUM_OF_COLUMNS], int  column, int row, int val);

bool validCheck(cell board[NUM_OF_ROWS][NUM_OF_COLUMNS], int column, int row, int val);

void set(cell board[NUM_OF_ROWS][NUM_OF_COLUMNS], int column, int row, int val);

void validate(cell user_board[NUM_OF_ROWS][NUM_OF_COLUMNS], cell solved_board[NUM_OF_ROWS][NUM_OF_COLUMNS]);

void hint(cell solved_board[NUM_OF_ROWS][NUM_OF_COLUMNS], int column, int row);

void restart();

#endif /* GAME_H_ */
