#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h.>

#include "con_lib.h"

#define BOARD_SIZE 7
#define MINE_COUNT 2

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
int randInt(int low, int high);
Board *loadSave(char *fileName);
bool checkForKey(int press, int key);
bool inBounds(int coordX, int coordY, int sizeX, int sizeY);
int countNearbyMines(Board tempBoard, int x, int y);

int main()
{
    srand(time(NULL));
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

        return;
    }
    else if (choice == 1)
    {
        //showMenu("Loading the save", fileNames, n, "Choose a save-file to load");
        //*realBoard = loadSave(fileName);

        return;
    }
    else if (choice == 2)
    {
        showScoreboard();
        char *menuScoreboard[1] = {"Go back"};
        showMenu("Welcome to the scoreboard", menuScoreboard, 1, "Pick what you want to do");
        menuInstance();

        return;
    }
    else
    {

        return;
    }

    //con_clear();
}

void gameInstance(Board *realBoard)
{
    //system("pause");
    if (realBoard == NULL)
    {
        setUpBoard(realBoard, BOARD_SIZE, BOARD_SIZE, MINE_COUNT);
    }

    return;
}

int showMenu(char *menuTitle, char *menuOptions[], int menuSize, char *inputMsg)
{
    con_clear();

    int posY = 1, oldY = 0;
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

void setUpBoard(Board *realBoard, int height, int width, int mines)
{
    realBoard = malloc(sizeof(Board));
    realBoard->sizeX = width;
    realBoard->sizeY = height;
    realBoard->cursorX = realBoard->sizeX / 2;
    realBoard->cursorY = realBoard->sizeY / 2;
    //realBoard->cells = (Cell **)malloc(realBoard->sizeY * sizeof(Cell *));
    Cell **temp = (Cell **)malloc(realBoard->sizeY * sizeof(Cell *));
    for (int i = 0; i < realBoard->sizeY; i++)
    {
        temp[i] = (Cell *)malloc(realBoard->sizeX * sizeof(Cell));
    }
    realBoard->cells = temp;
    /*for (int i = 0; i < realBoard->sizeY; i++)
    {
        for (int j = 0; j < realBoard->sizeX; j++)
        {
            realBoard->cells[i][j].cellValue = i * realBoard->sizeY + j;
        }
    }
    for (int i = 0; i < realBoard->sizeY; i++)
    {
        for (int j = 0; j < realBoard->sizeX; j++)
        {
            printf("%i ", realBoard->cells[i][j].cellValue);
        }
        printf("\n");
    }*/
    for (int i = 0; i < realBoard->sizeY; i++)
    {
        for (int j = 0; j < realBoard->sizeX; j++)
        {
            realBoard->cells[i][j].isMined = false;
            realBoard->cells[i][j].isRevealed = false;
            realBoard->cells[i][j].isMarked = false;
        }
    }
    for (int i = 0; i < mines; i++)
    {
        int y = randInt(0, realBoard->sizeY - 1);
        int x = randInt(0, realBoard->sizeX - 1);
        printf("%i %i\n", x, y);
        if (realBoard->cells[y][x].isMined)
        {
            i--;
        }
        else
        {
            realBoard->cells[y][x].isMined = true;
            realBoard->cells[y][x].cellValue = -1;
        }
    }
    for (int i = 0; i < realBoard->sizeY; i++)
    {
        for (int j = 0; j < realBoard->sizeX; j++)
        {
            if (!realBoard->cells[i][j].isMined)
            {
                realBoard->cells[i][j].cellValue = countNearbyMines(*realBoard, i, j);
            }
        }
    }
    scanf("%s");
    return;
}

int randInt(int low, int high)
{
    return rand() % (high - low + 1) + low;
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

bool inBounds(int coordX, int coordY, int sizeX, int sizeY)
{
    if (coordX < 0 || coordX >= sizeY || coordY < 0 || coordY >= sizeX)
    {
        return false;
    }
    else
    {
        return true;
    }
}

int countNearbyMines(Board tempBoard, int coordX, int coordY)
{
    int cellValue = 0;
    if (inBounds(coordX - 1, coordY - 1, tempBoard.sizeX, tempBoard.sizeY) && tempBoard.cells[coordX - 1][coordY - 1].isMined)
    {
        cellValue++;
    }
    if (inBounds(coordX, coordY - 1, tempBoard.sizeX, tempBoard.sizeY) && tempBoard.cells[coordX][coordY - 1].isMined)
    {
        cellValue++;
    }
    if (inBounds(coordX + 1, coordY - 1, tempBoard.sizeX, tempBoard.sizeY) && tempBoard.cells[coordX + 1][coordY - 1].isMined)
    {
        cellValue++;
    }
    if (inBounds(coordX + 1, coordY, tempBoard.sizeX, tempBoard.sizeY) && tempBoard.cells[coordX + 1][coordY].isMined)
    {
        cellValue++;
    }
    if (inBounds(coordX + 1, coordY + 1, tempBoard.sizeX, tempBoard.sizeY) && tempBoard.cells[coordX + 1][coordY + 1].isMined)
    {
        cellValue++;
    }
    if (inBounds(coordX, coordY + 1, tempBoard.sizeX, tempBoard.sizeY) && tempBoard.cells[coordX][coordY + 1].isMined)
    {
        cellValue++;
    }
    if (inBounds(coordX - 1, coordY + 1, tempBoard.sizeX, tempBoard.sizeY) && tempBoard.cells[coordX - 1][coordY + 1].isMined)
    {
        cellValue++;
    }
    if (inBounds(coordX - 1, coordY, tempBoard.sizeX, tempBoard.sizeY) && tempBoard.cells[coordX - 1][coordY].isMined)
    {
        cellValue++;
    }

    return cellValue;
}