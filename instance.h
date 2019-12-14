/** 
 * Project's goal: A minesweeper implementation
 * Author: Ričardas Čubukinas
 * Requires game_logic.h, con_lib.h, end.h, instance.h, board.h, save.h, utility.h
 * 
 * 
*/

#ifndef INSTANCE_H
#define INSTANCE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "board.h"
#include "game_logic.h"
#include "con_lib.h"
#include "save.h"
#include "utility.h"
#include "instance.h"

bool wasPressed[128];
int showMenu(char *menuTitle, char *menuOptions[], int menuSize, char *inputMsg);
void mainMenuInstance();
void inGameMenuInstance(Board *realBoard);
void gameInstance(Board *realBoard);
void scoreboardInstance();

#endif