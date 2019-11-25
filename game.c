#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h.>

#include "con_lib.h"

#define BOARD_SIZE 24
#define MINE_COUNT 99

typedef struct Cell
{
    bool isMined, isRevealed, isMarked;
    int cellValue;
} Cell;

typedef struct Board
{
    int mineCount;
    int sizeX, sizeY;
    int cursorX, cursorY;
    Cell **cells;
} Board;

bool wasPressed[128];

int showMenu(char *menuTitle, char *menuOptions[], int menuSize, char *inputMsg);
void menuInstance();
void gameInstance(Board *realBoard);
void setUpBoard(Board *realBoard, int height, int width, int mines);
void showScoreboard();
int randomInt(int low, int high);
Board *loadSave(char *fileName);
bool checkForKey(int press, int key);

int main()
{
    for (int i = 0; i < 128; i++)
    {
        wasPressed[i] = false;
    }
    wasPressed[13] = true;
    
    menuInstance();

    return 0;
}

void menuInstance()
{
    Board *realBoard = NULL;
    char *menuOptions[4] = {"New Game", "Load Save", "Show Scoreboard", "Quit"};
    int choice = showMenu("Welcome to Minesweeper", menuOptions, 4, "Pick what you want to do");
    if (choice == 0)
    {
        gameInstance(realBoard);
    }
    if (choice == 1)
    {
        //showMenu("Loading the save", fileNames, n, "Choose a save-file to load");
        //*realBoard = loadSave(fileName);

        return;
    }
    if (choice == 2)
    {
        showScoreboard();
        char *menuScoreboard[1] = {"Go back"};
        showMenu("Welcome to the scoreboard", menuScoreboard, 1, "Pick what you want to do");
        menuInstance();

        return;
    }
    if(choice == 3)
    {
        
        return;
    }

    con_clear();
}

void gameInstance(Board *realBoard)
{
    if(realBoard == NULL)
    {
        setUpBoard(realBoard, BOARD_SIZE, BOARD_SIZE, MINE_COUNT);
    }

    return;
}

void setUpBoard(Board *realBoard, int height, int width, int mines)
{
    realBoard = malloc(sizeof(Board));
    realBoard->cells = (Cell **)malloc(sizeof(Cell *) * height + sizeof(Cell) * width * height); 
    realBoard->sizeX = width;
    realBoard->sizeY = height;
    realBoard->cursorX = realBoard->sizeX/2;
    realBoard->cursorY = realBoard->sizeY/2;
    for(int i = 0; i < mines; i++)
    {
        //
    }

    return;
}

int showMenu(char *menuTitle, char *menuOptions[], int menuSize, char *inputMsg)
{
    con_clear();

    int posY = 1, oldY = 0;
    ;
    printf("%s\n", menuTitle);
    for (int i = 0; i < menuSize; i++)
    {
        printf("%i. %s\n", i, menuOptions[i]);
    }
    printf("%s\n", inputMsg);

    while (1)
    {
        int key = 0;
        while (key = con_read_key())
        {
            if (checkForKey(key, 'w'))
            {
                posY--;
            }

            if (checkForKey(key, 's'))
            {
                posY++;
            }

            if (posY > menuSize)
                posY = 1;
            if (posY < 1)
                posY = menuSize;

            if (checkForKey(key, 13))
            {
                con_clear();

                return posY - 1;
            }

            con_set_color(COLOR_BLACK, COLOR_GRAY);
            con_set_pos(0, oldY);

            con_set_color(COLOR_RED, COLOR_BLUE);
            con_set_pos(0, posY);
            fflush(stdout);

            oldY = posY;
        }
    }
}

int randomInt(int low, int high)
{
    srand(time(NULL));

    return rand() % (high - low + 1) + high;
}

void showScoreboard()
{
    /* Not implemented yet */

    return;
}

Board *loadSave(char *fileName)
{
    Board *loadedBoard = malloc(sizeof(Board));
    /* Show menu of files return board through the pointer*/
    if (0) // True statement
    {

        return loadedBoard;
    }
    else
    {

        return NULL;
    }
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