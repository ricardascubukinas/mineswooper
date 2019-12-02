/** 
 * Project's goal: A minesweeper implementation
 * Author: Ričardas Čubukinas
 * Requires con_lib.h
 * 
 * 
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <direct.h>
#include <windows.h>

#include "con_lib.h"
#include "end.h"
#include "instance.h"
#include "board.h"
#include "save.h"
#include "utility.h"

int main()
{

    setUpVariables();
    menuInstance();

    con_clear();
    return 0;
}
