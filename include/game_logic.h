/** 
 * Project's goal: A minesweeper implementation
 * Author: Ričardas Čubukinas
 * Requires game_logic.h, con_lib.h, end.h, instance.h, board.h, save.h, utility.h
 * 
 * 
*/

#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "board.h"
#include "utility.h"
#include "instance.h"
#include "player.h"

Board *setUpBoard(Board *realBoard, int height, int width, int mines);
Board *reveal(Board *tempBoard, bool isFirst);
Board *checkForFails(Board *tempBoard);
int countUnrevealedCells(Board tempBoard);
int countNearbyMines(Board tempBoard, int coordX, int coordY);
bool inBounds(int coordX, int coordY, int sizeX, int sizeY);
bool hasWon(Board tempBoard);
void loseScreen();
void winScreen(int time);

#endif