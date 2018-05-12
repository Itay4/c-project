#include "main_aux.h"
#include <stdlib.h>
#include <stdio.h>

bool inputValid(int num) {
	/*
     * Validates user input of number of cells to fill
     */

	if (num >= 0 && num <= 80){
		return true;
	}
	return false;
}

void delFromArr(int position, int numbersLeft, int availableNumbers[]) {
	/*
     * Simulates deletion of elemnt in given position in array by shifting left elements 
     */
    int c;
    for ( c = position - 1 ; c < numbersLeft - 1 ; c++ ) {
         availableNumbers[c] = availableNumbers[c+1];
     }
}
