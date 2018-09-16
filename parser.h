/** @file parser.h
 *  @brief parser header file.
 *
 *  This header contains the prototypes for parser module.
 *
 *  @author Itay Keren (itaykeren)
 *  @author Rotem Bar (rotembar)
 *
 */

#ifndef SUDOKU_PARSER_H
#define SUDOKU_PARSER_H


/** @brief parsing user command by white spaces.
 *  assigning parsed commands to parsedCommand array.
 *  @param command to be parsed.
 *  @param parsedCommand will be updated to contain commands after being arsed.
 *  @return  amount of strings inserted to parsedCommand.
 */
int parseCommand(char* command, char* parsedCommand[]);

#endif /*SUDOKU_PARSER_H*/
