#ifndef INSTANCE_H
#define INSTANCE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "board.h"
#include "end.h"
#include "con_lib.h"
#include "save.h"
#include "utility.h"
#include "instance.h"

bool wasPressed[128];
int showMenu(char *menuTitle, char *menuOptions[], int menuSize, char *inputMsg);
void mainMenuInstance();
void inGameMenuInstance(Board *realBoard, int markedCount);
void gameInstance(Board *realBoard);
void scoreboardInstance();

#endif