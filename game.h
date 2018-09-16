/** @file game.h
 *  @brief game header file.
 *
 *  This header contains the prototypes for the game module.
 *
 *  @author Itay Keren (itaykeren)
 *  @author Rotem Bar (rotembar)
 *
 */

#ifndef SUDOKU_GAME_H
#define SUDOKU_GAME_H

/* -- Includes -- */
#include "linked_list.h"
#include "main_aux.h"


/** @brief Returns a new empty sudoku board of the appropriate size.
 *  @return an empty sudoku board
 */
cell** generate_empty_board();


/** @brief Copies a given sudoku board.
 *  @param source_board to be copied, remains unchanged.
 *  @param new_board to which source_board will be copied.
 *  @return void.
 */
void copy_board(cell** source_board, cell** new_board);


/** @brief Prints a given sudoku board.
 *  @param board to be printed.
 *  @param mode game mode to print board according to relevant restrictions.
 *  @return void.
 */
void print_board(cell** board, char mode);


/** @brief Returns and prints sudoku board to be edited in edit mode (loaded from file or newly 9X9 generated)
 *  returns NULL if function fails.
 *  @param parsedCommand after user input was parsed.
 *  @param mode game mode to print boart according to relevant restrictions.
 *  @return sudoku board.
 */
cell** edit_command(char* parsedCommand[4], char mode);


/** @brief Returns and prints sudoku board to be solved in solve mode (loaded from file)
 *  returns NULL if function fails.
 *  @param parsedCommand after user input was parsed.
 *  @param mode game mode to print board according to relevant restrictions.
 *  @return sudoku board.
 */
cell** solve_command(char* parsedCommand[4], char mode);


/** @brief frees allocated memory of given sudoku board
 *  @param board sudoku board to be free.
 *  @return void.
 */
void free_board(cell** board);


/** @brief frees given command and exists the game
 *  @param command to be free.
 *  @return void.
 */
void exit_game(char* command);


/** @brief  executes user-entered game command.
 *  prints appropriate massages and updating board and moves list when required.
 *  @param parsedCommand after user input was parsed.
 *  @param board sudoku board to be updated.
 *  @param user command.
 *  @param counter  amount of cells filled with values parsedCommand.
 *  @param mode game mode to executes game commands according to relevant restrictions.
 *  @param lst moves list to be updated after a move which changed board status.
 *  @return void
 */
void execute_command(char* parsedCommand[4], cell** board, char* command, int counter, char mode, list* lst);




#endif /*SUDOKU_GAME_H*/