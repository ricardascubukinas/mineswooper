/** 
 * Project's goal: A minesweeper implementation
 * Author: Ričardas Čubukinas
 * Requires game_logic.h, con_lib.h, end.h, instance.h, board.h, save.h, utility.h
 * 
 * 
*/

#ifndef UTILITY_H
#define UTILITY_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "board.h"
#include "con_lib.h"
#include "save.h"

int areaX[9];
int areaY[9];
clock_t programStart;

void setUpVariables();
void freeUpVariables(Board *realBoard);
int randInt(int low, int high);
bool checkForKey(int press, int key);
void printBoard(Board tempBoard);
void logStart();
void logEnd();

#endif