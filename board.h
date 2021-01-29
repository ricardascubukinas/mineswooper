#ifndef BOARD_H
#define BOARD_H

#include "cell.h"

typedef struct Board
{
    int timeSeconds;
    int mineCount, markedCount;
    int sizeX, sizeY;
    int cursorX, cursorY;
    Cell **cells;
} Board;

#endif