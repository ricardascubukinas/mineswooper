#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "board.h"
#include "end.h"

Board *setUpBoard(Board *realBoard, int height, int width, int mines);
Board *reveal(Board *tempBoard, bool isFirst);
Board *checkForFails(Board *tempBoard);
int countUnrevealedCells(Board tempBoard);
int countNearbyMines(Board tempBoard, int coordX, int coordY);
bool inBounds(int coordX, int coordY, int sizeX, int sizeY);

#endif