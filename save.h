#ifndef SAVE_H
#define SAVE_H

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <direct.h>

#include "con_lib.h"
#include "board.h"
#include "menu.h"

char *selectSaveFile();
char *selectLoadFile();
void saveBoard(Board realBoard, char *fileName);
Board *loadBoard(char *fileName);

#endif