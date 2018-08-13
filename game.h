#ifndef GAME_H_
#define GAME_H_

void initializeBoard(cell board[NUM_OF_ROWS][NUM_OF_COLUMNS]);

void printBoard(cell **board, int rows, int cols, int markErrors);

void executeCommand(char *parsedCommand[4], cell **board, char* command, int counter, char mode, size_t *rows, size_t *cols, int *markErrors);

void gameOver(cell **board);

bool valInColumn(cell **board, int column, int val, int numOfRows);

bool valInRow(cell **board, int row, int val, int NumOfCols);

bool valInBlock(cell **board, int column, int row, int val, int numOfRows, int NumOfCols);

bool validCheck(cell **board, int column, int row, int val, int numOfRows, int numOfCols);

void set(cell **board, int column, int row, int val, int numOfRows, int numOfCols);

void validate(cell **board, size_t *rows, size_t *cols);

void hint(cell **board, int column, int row);

cell **solveCommand(char* parsedCommand[4], size_t *rows, size_t *cols, int *markErrors);

cell **editCommand(char* parsedCommand[4], size_t *rows, size_t *cols);

void markErrorsCommand(char* value, int *markErrors);

void saveCommand(cell **board, int rows, int cols, char *filePath);

void autoFill(cell **board, int rows, int cols, int markErrors);

void numSolutions(cell **board, int numOfRows, int numOfCols);

int countSolutions(cell **board, int numOfRows, int numOfCols);

int countSolutionsRec(cell **board, int i, int j, int counter, int numOfRows, int numOfCols);

#endif /* GAME_H_ */
