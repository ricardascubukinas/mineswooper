#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h.>

#include "con_lib.h"

#define BOARD_SIZE 10
#define MINE_COUNT 10

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
Board *setUpBoard(Board *realBoard, int height, int width, int mines);
void showScoreboard();
int randInt(int low, int high);
void saveBoard(Board realBoard, char *fileName);
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

    system("pause");
    con_clear();

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

void gameInstance(Board *realBoard)
{
    if (realBoard == NULL)
    {
        realBoard = setUpBoard(realBoard, BOARD_SIZE, BOARD_SIZE, MINE_COUNT);
        saveBoard(*realBoard, "save.bin");
    }
    for (int i = 0; i < realBoard->sizeY; i++)
    {
        for (int j = 0; j < realBoard->sizeX; j++)
        {
            if (!realBoard->cells[i][j].isMined)
                printf("%3i", realBoard->cells[i][j].cellValue);
            else
            {
                printf("%3c", '#');
            }
        }
        printf("\n");
    }
    int posY = 0, oldY = 0;
    int posX = 2, oldX = 2;
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

            if (checkForKey(key, 'a'))
            {
                posX -= 3;
            }
            if (checkForKey(key, 'd'))
            {
                posX += 3;
            }

            if (posY > 9)
                posY = 0;
            if (posY < 0)
                posY = 9;

            if (posX < 2)
                posX = (realBoard->sizeX) * 3 - 1;
            if (posX > (realBoard->sizeX * 3) - 1)
                posX = 2;

            if (checkForKey(key, 13))
            {
                con_clear();

                return;
            }

            con_set_color(COLOR_BLACK, COLOR_GRAY);
            con_set_pos(oldX, oldY);

            con_set_color(COLOR_RED, COLOR_BLUE);
            con_set_pos(posX, posY);
            fflush(stdout);

            oldY = posY;
            oldX = posX;
        }
    }
    return;
}

Board *setUpBoard(Board *realBoard, int height, int width, int mines)
{
    realBoard = malloc(sizeof(Board));
    realBoard->mineCount = mines;
    realBoard->sizeX = width;
    realBoard->sizeY = height;
    realBoard->cursorX = realBoard->sizeX / 2;
    realBoard->cursorY = realBoard->sizeY / 2;
    Cell **temp = (Cell **)malloc(realBoard->sizeY * sizeof(Cell *));
    for (int i = 0; i < realBoard->sizeY; i++)
    {
        temp[i] = (Cell *)malloc(realBoard->sizeX * sizeof(Cell));
    }
    realBoard->cells = temp;
    for (int i = 0; i < realBoard->sizeY; i++)
    {
        for (int j = 0; j < realBoard->sizeX; j++)
        {
            realBoard->cells[i][j].isMined = false;
            realBoard->cells[i][j].isRevealed = false;
            realBoard->cells[i][j].isMarked = false;
        }
    }
    for (; mines > 0; mines--)
    {
        int y = randInt(0, realBoard->sizeY - 1), x = randInt(0, realBoard->sizeX - 1);
        if (realBoard->cells[y][x].isMined)
        {
            mines++;
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
    return realBoard;
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

void saveBoard(Board realBoard, char *fileName)
{
    FILE *fw = fopen(fileName, "wb");
    fwrite(&realBoard.mineCount, sizeof(int), 1, fw);
    fwrite(&realBoard.cursorX, sizeof(int), 1, fw);
    fwrite(&realBoard.cursorY, sizeof(int), 1, fw);
    fwrite(&realBoard.sizeX, sizeof(int), 1, fw);
    fwrite(&realBoard.sizeY, sizeof(int), 1, fw);
    for(int i = 0; i < realBoard.sizeY; i++)
    {
        fwrite(&realBoard.cells[i], sizeof(Cell), realBoard.sizeX, fw);
    }
    fclose(fw);
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