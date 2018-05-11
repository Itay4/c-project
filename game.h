typedef struct  {
	int number;
	bool isFixed;
} cell;
#define NUM_OF_ROWS 9
#define NUM_OF_COLUMNS 9

void initializeBoard(cell board[NUM_OF_ROWS][NUM_OF_COLUMNS]);

void printBoard(cell board[NUM_OF_ROWS][NUM_OF_COLUMNS]);