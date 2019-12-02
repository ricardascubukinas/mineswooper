#ifndef MENU_H
#define MENU_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "board.h"
#include "end.h"
#include "con_lib.h"
#include "save.h"

bool wasPressed[128];
int showMenu(char *menuTitle, char *menuOptions[], int menuSize, char *inputMsg);
void menuInstance();
void inGameMenu(Board *realBoard, int markedCount);

#endif