#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "board.h"
#include "utility.h"
#include "instance.h"

Board *setUpBoard(Board *realBoard, int height, int width, int mines);
Board *reveal(Board *tempBoard, bool isFirst);
Board *checkForFails(Board *tempBoard);
int countUnrevealedCells(Board tempBoard);
int countNearbyMines(Board tempBoard, int coordX, int coordY);
bool inBounds(int coordX, int coordY, int sizeX, int sizeY);
bool hasWon(Board tempBoard);
void loseScreen();
void winScreen();
void exitGame(Board *realBoard);

#endif