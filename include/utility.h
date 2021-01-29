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
#include <assert.h>

#include "board.h"
#include "player.h"
#include "con_lib.h"
#include "save.h"
#include "scoreboard.h"

const int areaX[9];
const int areaY[9];
clock_t programStart;

void setUpVariables();
void freeUpVariables(Board *realBoard);
int randInt(int low, int high);
bool checkForKey(int press, int key);
void printBoard(Board tempBoard);
void logStart();
void logEnd();
void printTime(Board tempBoard);
Player *sortScoreboard(Player *scoreboard, int size);
void swap(Player *a, Player *b);
void testFunctions();

#endif