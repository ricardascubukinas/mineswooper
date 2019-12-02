/** 
 * Project's goal: A minesweeper implementation
 * Author: Ričardas Čubukinas
 * Requires game_logic.h, con_lib.h, end.h, instance.h, board.h, save.h, utility.h
 * 
 * 
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <direct.h>
#include <windows.h>

#include "game_logic.h"
#include "con_lib.h"
#include "end.h"
#include "instance.h"
#include "board.h"
#include "save.h"
#include "utility.h"

int main()
{
    setUpVariables();
    mainMenuInstance();

    exitGame(NULL);
}
