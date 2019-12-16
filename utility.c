/** 
 * Project's goal: A minesweeper implementation
 * Author: Ričardas Čubukinas
 * Requires game_logic.h, con_lib.h, end.h, instance.h, board.h, save.h, utility.h
 * 
 * 
*/

#include "utility.h"

const int areaX[9] = {0, -1, 0, 1, 1, 1, 0, -1, -1};
const int areaY[9] = {0, -1, -1, -1, 0, 1, 1, 1, 0};
clock_t programStart;

void setUpVariables()
{
    srand(time(NULL));
    for (int i = 0; i < 128; i++)
    {
        wasPressed[i] = false;
    }
}

void freeUpVariables(Board *realBoard)
{
    con_clear();
    if (!realBoard)
    {
        free(realBoard);
    }

    return;
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

void printBoard(Board tempBoard)
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
    printf("  Mines left: %i\n", tempBoard.mineCount - tempBoard.markedCount);
    printf("  Controls: w - up, s - down, a - left, d - right\n");
    printf("  ENTER - reveal a cell, SPACE - mark a cell, m - open menu\n");
    con_set_color(COLOR_BLACK, COLOR_GRAY);
    con_set_pos(tempBoard.cursorX * 3 + 2, tempBoard.cursorY);
    fflush(stdout);
    printTime(tempBoard);

    return;
}

void printTime(Board tempBoard)
{
    con_set_pos(0, tempBoard.sizeY + 3);
    printf("  Time passed: %i\n", tempBoard.timeSeconds);
    con_set_pos(tempBoard.cursorX * 3 + 2, tempBoard.cursorY);
    con_set_color(COLOR_BLACK, COLOR_GRAY);
    fflush(stdout);
}

void logStart()
{
    programStart = clock();
    FILE *fw = fopen("log.txt", "a");
    if (fw != NULL)
    {
        time_t t;
        time(&t);
        fprintf(fw, "The program has started, current date and time: %s", ctime(&t));

        fclose(fw);
    }
    else
    {
        printf("Failed to log to file\n");
        system("pause");
    }
}

void logEnd()
{
    clock_t programEnd = clock();
    double executeTime = (double)(programEnd - programStart) / CLOCKS_PER_SEC;
    FILE *fw = fopen("log.txt", "a");
    if (fw != NULL)
    {
        fprintf(fw, "The execute time was: %lf\n", executeTime);

        fclose(fw);
    }
    else
    {
        printf("Failed to log to file\n");
        system("pause");
    }
}

Player *sortScoreboard(Player *scoreboard, int size)
{

    for (int i = 0; i < size - 1; i++)
    {
        bool isSwapped = false;

        for (int j = 0; j < size - i - 1; j++)
        {
            if (scoreboard[j].time > scoreboard[j + 1].time)
            {
                swap(&scoreboard[j], &scoreboard[j + 1]);
                isSwapped = true;
            }
        }
        if (!isSwapped)
        {
            break;
        }
    }

    return scoreboard;
}

void swap(Player *a, Player *b)
{
    Player temp = *a;
    *a = *b;
    *b = temp;
}

void testFunctions()
{
    assert(inBounds(-5, -10, 5, 10) == false);
    FILE *fr = fopen("count.txt", "r");
    int count;
    fscanf(fr, "%d", &count);
    assert(count == getCount());
    assert(randInt(1, 10) >= 1);
    assert(randInt(1, 10) <= 10);

    return;
}