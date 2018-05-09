typedef int bool;
#define true 1
#define false 0
#include <stdio.h>
typedef struct  {
	int number;
	bool isFixed;
} cell;
#define NUM_OF_LINES 9
#define NUM_OF_ROWS 9

void printSeperator() {
	printf("----------------------------------\n");
}

void printBoard(cell board[NUM_OF_LINES][NUM_OF_ROWS]) {
	int i,j;
	printSeperator();
	for (i=0; i < NUM_OF_LINES; i++) {
		if (i == 3 || i == 6){
	    	printSeperator();
		}
		printf("| ");
		for (j=0; j < NUM_OF_ROWS; j++) {
			if (j == 3 || j == 6) {
				printf("| ");
			}
			if (board[i][j].isFixed) {
				printf(".");
			} else {
				printf(" ");
			}
			printf("%d ", board[i][j].number);
		}
		printf("|\n");
		
	}
	printSeperator();
}




