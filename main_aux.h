#ifndef MAIN_AUX_H_
#define MAIN_AUX_H_

typedef int bool;
#define true 1
#define false 0

/* Constants */
#define MAX_CMD_SIZE 1024
#define NUM_OF_ROWS 9
#define NUM_OF_COLUMNS 9

/* Types */
typedef struct  {
	int number;
	bool isFixed;
} cell;

bool inputValid(int num);

void delFromArr(int position, int numbersLeft, int availableNumbers[]);

void exitGame(char* command);

void memoryError(char* func);

#endif /* MAIN_AUX_H_ */

