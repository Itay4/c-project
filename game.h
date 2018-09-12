
#ifndef SUDOKU90_GAME_H
#define SUDOKU90_GAME_H
#include "linked_list.h"
#include "main_aux.h"

extern int rows;
extern int cols;
extern int mark_errors;

cell **generate_empty_board();

void copy_board(cell **source_board, cell **new_board);

void print_board(cell **board);

void execute_command(char *parsedCommand[4], cell **board, char* command, int counter, char mode, list *lst);

void game_over(cell **board);

bool val_in_column(cell **board, int column, int row, int val);

bool val_in_row(cell **board, int column, int row, int val);

bool val_in_block(cell **board, int column, int row, int val);

bool valid_check(cell **board, int column, int row, int val);

int set(cell **board, int column, int row, int val, char mode);

/*void validate(cell **board);*/

bool check_board_erroneous(cell **board);

void validate_risks(cell **board, int column, int row);

void hint(cell **board, int column, int row);

cell **solve_command(char* parsedCommand[4], char mode);

cell **edit_command(char* parsedCommand[4], char mode);

void mark_errors_command(int value);

void save_command(cell **board, char *filePath, char mode);

int auto_fill(cell **board);

void num_solutions(cell **board);

int count_solutions(cell **board);

int count_solutions_rec(cell **board, int i, int j, int counter);

void free_board(cell** board);

void exit_game(char* command);


#endif /*SUDOKU90_GAME_H*/
