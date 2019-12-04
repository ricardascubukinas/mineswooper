/** 
 * Project's goal: A minesweeper implementation
 * Author: Ričardas Čubukinas
 * Requires game_logic.h, con_lib.h, end.h, instance.h, board.h, save.h, utility.h
 * 
 * 
*/

#ifndef SAVE_H
#define SAVE_H

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <direct.h>

#include "con_lib.h"
#include "board.h"
#include "instance.h"

char *selectSaveFile();
char *selectLoadFile();
void saveBoard(Board realBoard, char *fileName);
Board *loadBoard(char *fileName);

#endif