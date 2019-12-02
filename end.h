#ifndef END_H
#define END_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "con_lib.h"
#include "board.h"
#include "instance.h"
#include "game_logic.h"

bool hasWon(Board tempBoard);
void loseScreen();
void winScreen();
void exitGame(Board *realBoard);

#endif