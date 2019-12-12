/** 
 * Project's goal: A minesweeper implementation
 * Author: Ričardas Čubukinas
 * Requires game_logic.h, con_lib.h, end.h, instance.h, board.h, save.h, utility.h
 * 
 * 
*/

#include "utility.h"

int areaX[9] = {0, -1, 0, 1, 1, 1, 0, -1, -1};
int areaY[9] = {0, -1, -1, -1, 0, 1, 1, 1, 0};

void setUpVariables()
{
    srand(time(NULL));
    for (int i = 0; i < 128; i++)
    {
        wasPressed[i] = false;
    }
}

int randInt(int low, int high)
{
    return rand() % (high - low + 1) + low;
}

bool checkForKey(int press, int key)
{
    if (press == key && !wasPressed[press])
    {
        wasPressed[key] = true;

        return true;
    }
    else if (press == key && wasPressed[press])
    {
        wasPressed[key] = false;

        return false;
    }

    return false;
}

void printBoard(Board tempBoard, int markedCount)
{
    /**TODO:
     * Make different colors for each cell
    */
    con_clear();
    for (int i = 0; i < tempBoard.sizeY; i++)
    {
        for (int j = 0; j < tempBoard.sizeX; j++)
        {
            if ((!tempBoard.cells[i][j].isRevealed || tempBoard.cells[i][j].isMined) && !tempBoard.cells[i][j].isMarked)
            {
                printf("%3c", '#');
            }
            else if (tempBoard.cells[i][j].isRevealed)
            {
                printf("%3i", tempBoard.cells[i][j].cellValue);
            }
            else if (tempBoard.cells[i][j].isMarked)
            {
                con_set_color(COLOR_BLACK, COLOR_CYAN);
                printf("%3c", '*');
            }
            con_set_color(COLOR_BLACK, COLOR_GRAY);
        }
        printf("\n");
    }
    printf("  Mines left: %i\n", tempBoard.mineCount - markedCount);
    printf("  Controls: w - up, s - down, a - left, d - right\n");
    printf("  ENTER - reveal a cell, SPACE - mark a cell, m - open menu\n");

    return;
}