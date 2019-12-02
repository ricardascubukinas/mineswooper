#ifndef END_H
#define END_H

#include "con_lib.h"
#include "board.h"
#include "menu.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool hasWon(Board tempBoard);
void loseScreen();
void winScreen();
void exitGame(Board *realBoard);

#endif