typedef int bool;
#define true 1
#define false 0
#include <stdio.h>
typedef struct  {
	int number;
	bool isFixed;
} cell;
#define NUM_OF_ROWS 9
#define NUM_OF_COLUMNS 9

void initializeBoard(cell board[NUM_OF_ROWS][NUM_OF_COLUMNS]) {
	int i,j;
	for (i=0; i < 10; i++){
	   for (j=0; j < 10; j++){
		   board[i][j].isFixed = false;
		   board[i][j].number = 0;
	   }
	}
}

void printSeperator() {
	printf("----------------------------------\n");
}

void printBoard(cell board[NUM_OF_ROWS][NUM_OF_COLUMNS]) {
	int i,j;
	printSeperator();
	for (i=0; i < NUM_OF_ROWS; i++) {
		if (i == 3 || i == 6){
	    	printSeperator();
		}
		printf("| ");
		for (j=0; j < NUM_OF_COLUMNS; j++) {
			if (j == 3 || j == 6) {
				printf("| ");
			}
			if (board[i][j].isFixed) {
				printf(".");
				printf("%d", board[i][j].number);
			} else {
				printf("  ");
			}
			printf(" ");
		}
		printf("|\n");
		
	}
	printSeperator();
}




