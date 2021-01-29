#ifndef CELL_H
#define CELL_H

#include <stdbool.h>

typedef struct Cell
{
    bool isMined, isRevealed, isMarked;
    int cellValue;
} Cell;

#endif