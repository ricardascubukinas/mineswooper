#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "con_lib.h"

#define BOARD_SIZE 6
#define MINE_COUNT 1

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

int areaX[9] = {0, -1, 0, 1, 1, 1, 0, -1, -1};
int areaY[9] = {0, -1, -1, -1, 0, 1, 1, 1, 0};

bool wasPressed[128];

int showMenu(char *menuTitle, char *menuOptions[], int menuSize, char *inputMsg);
void menuInstance();
void gameInstance(Board *realBoard);
void inGameMenu(Board *realBoard);
Board *reveal(Board *tempBoard, bool isFirst);
Board *checkForFails(Board *tempBoard);
Board *setUpBoard(Board *realBoard, int height, int width, int mines);
Board *setUpSampleBoard();
void loseScreen();
void printBoard(Board tempBoard);
void showScoreboard();
int randInt(int low, int high);
void saveBoard(Board realBoard);
Board *loadSave(char *fileName);
bool checkForKey(int press, int key);
bool inBounds(int coordX, int coordY, int sizeX, int sizeY);
int countNearbyMines(Board tempBoard, int x, int y);
void exitGame(Board *realBoard);

int main()
{
    srand(time(NULL));
    for (int i = 0; i < 128; i++)
    {
        wasPressed[i] = false;
    }
    wasPressed[13] = true;

    menuInstance();
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
        exitGame(NULL);
    }
}

int showMenu(char *menuTitle, char *menuOptions[], int menuSize, char *inputMsg)
{
    con_clear();

    int posY = 1;
    printf("%s\n", menuTitle);
    for (int i = 0; i < menuSize; i++)
    {
        printf("%i. %s\n", i, menuOptions[i]);
    }
    printf("%s\n", inputMsg);
    printf("Controls: w - up, s - down\n");
    con_set_color(COLOR_BLACK, COLOR_GRAY);
    con_set_pos(0, posY);
    fflush(stdout);
    while (1)
    {
        int key = 0;
        while ((key = con_read_key()))
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
            con_set_pos(0, posY);
            fflush(stdout);
        }
    }
}

void gameInstance(Board *realBoard)
{
    if (realBoard == NULL)
    {
        //realBoard = setUpSampleBoard();
        realBoard = setUpBoard(realBoard, BOARD_SIZE, BOARD_SIZE, MINE_COUNT);
        //saveBoard(*realBoard, "save.bin");
        //realBoard = loadSave("save.bin");
        if (realBoard == NULL)
        {
            printf("Save-file was corrupted\n");
        }
        else
        {
        }
    }
    printBoard(*realBoard);
    int posX = 2, posY = 0;
    while (1)
    {
        int key = 0;
        while ((key = con_read_key()))
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
            if (checkForKey(key, 'm'))
            {
                inGameMenu(realBoard);
            }
            if (posY > realBoard->sizeY - 1)
                posY = 0;
            if (posY < 0)
                posY = realBoard->sizeY - 1;

            if (posX < 2)
                posX = (realBoard->sizeX * 3) - 1;
            if (posX > (realBoard->sizeX * 3) - 1)
                posX = 2;

            realBoard->cursorX = (posX - 2) / 3;
            realBoard->cursorY = posY;
            if (checkForKey(key, 13))
            {
                con_clear();
                realBoard = reveal(realBoard, true);
                if (realBoard == NULL)
                {
                    loseScreen();
                    return;
                }
                else
                {
                    printBoard(*realBoard);
                }
            }

            con_set_color(COLOR_BLACK, COLOR_GRAY);
            con_set_pos(posX, posY);
            fflush(stdout);
        }
    }

    return;
}

void inGameMenu(Board *realBoard)
{
    char *gameMenu[4] = {"Save", "Go back", "Quit to main menu", "Quit game"};
    int choice = showMenu("In game menu", gameMenu, 4, "Pick your option");
    if (choice == 0)
    {
        saveBoard(*realBoard);
        system("pause");
        inGameMenu(realBoard);
    }
    else if (choice == 1)
    {
        printBoard(*realBoard);
    }
    else if (choice == 2)
    {
        menuInstance();
        free(realBoard);
        return;
    }
    else
    {
        exitGame(realBoard);
    }
}

Board *reveal(Board *tempBoard, bool isFirst)
{
    int coordX = tempBoard->cursorX, coordY = tempBoard->cursorY;
    if (isFirst && tempBoard->cells[coordY][coordX].isMined)
    {
        return NULL;
    }
    else
    {
        if (tempBoard->cells[coordY][coordX].cellValue == 0)
        {
            for (int i = 0; i < 9; i++)
            {
                coordX = areaX[i] + tempBoard->cursorX;
                coordY = areaY[i] + tempBoard->cursorY;
                if (inBounds(coordX, coordY, tempBoard->sizeX, tempBoard->sizeY))
                {
                    if (!tempBoard->cells[coordY][coordX].isRevealed && !tempBoard->cells[coordY][coordX].isMined)
                    {
                        tempBoard->cells[coordY][coordX].isRevealed = true;
                        if (tempBoard->cells[coordY][coordX].cellValue == 0 && i != 0)
                        {
                            tempBoard->cursorX = coordX;
                            tempBoard->cursorY = coordY;
                            tempBoard = reveal(tempBoard, false);
                        }
                    }
                }
            }
        }
        else
        {
            tempBoard->cells[coordY][coordX].isRevealed = true;
        }

        tempBoard = checkForFails(tempBoard);
        return tempBoard;
    }
}

Board *checkForFails(Board *tempBoard)
{
    for (int i = 0; i < tempBoard->sizeY; i++)
    {
        for (int j = 0; j < tempBoard->sizeX; j++)
        {
            if (!tempBoard->cells[i][j].isRevealed)
            {
                for (int k = 0; k < 9; k++)
                {
                    int tempX = j + areaX[k], tempY = i + areaY[k];
                    if (inBounds(tempX, tempY, tempBoard->sizeX, tempBoard->sizeY))
                    {
                        if (tempBoard->cells[tempY][tempX].cellValue == 0 && tempBoard->cells[tempY][tempX].isRevealed)
                        {
                            tempBoard->cells[i][j].isRevealed = true;
                            break;
                        }
                    }
                }
            }
        }
    }
    return tempBoard;
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

Board *setUpSampleBoard()
{
    Board *realBoard = malloc(sizeof(Board));
    realBoard->mineCount = 1;
    realBoard->sizeX = 6;
    realBoard->sizeY = 6;
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
    realBoard->cells[4][5].isMined = true;
    realBoard->cells[4][5].cellValue = -1;
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

void loseScreen()
{
    printf("You've lost\n");
    system("pause");
    menuInstance();
}

void printBoard(Board tempBoard)
{
    con_clear();
    for (int i = 0; i < tempBoard.sizeY; i++)
    {
        for (int j = 0; j < tempBoard.sizeX; j++)
        {
            if (!tempBoard.cells[i][j].isRevealed || tempBoard.cells[i][j].isMined)
            {
                printf("%3c", '#');
            }
            else if (tempBoard.cells[i][j].isRevealed)
            {
                printf("%3i", tempBoard.cells[i][j].cellValue);
            }
            else if (tempBoard.cells[i][j].isMarked)
            {
                printf("%3c", '*');
            }
        }
        printf("\n");
    }
    printf("  Controls: w - up, s - down, a - left, d - right, m - open menu\n");
    printf("  ENTER - reveal selected cell, SHIFT - mark selected cell\n");
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

void saveBoard(Board realBoard)
{
    char fileName[1000];
    while (1)
    {
        printf("Enter a file name(to save in current directoy) or full file's path, don't type in the extension, maximum length 100 characters\n");
        fileName[0] = '\0';
        fgets(fileName, 1000, stdin);
        if (strlen(fileName) > 100)
        {
            printf("Error! File's name length is too long.\n");
        }
        else if (fileName == NULL || fileName[0] == '\n' || fileName[0] == '\0')
        {
            printf("You can't enter an empty string\n");
        }
        else
        {
            strtok(fileName, "\n");
            printf("Success! Proceeding to save to file '%s.bin'\n", fileName);
            break;
        }
    }
    strcat(fileName, ".bin");
    FILE *fw = fopen(fileName, "wb");
    fwrite(&realBoard, sizeof(Board), 1, fw);

    fclose(fw);
}

Board *loadSave(char *fileName)
{
    Board *loadedBoard = malloc(sizeof(Board));
    FILE *fr = fopen(fileName, "rb");

    if (fread(loadedBoard, sizeof(Board), 1, fr) == 1) // True statement
    {
        fclose(fr);
        return loadedBoard;
    }
    else
    {
        fclose(fr);
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
    for (int i = 1; i < 9; i++)
    {
        int tempX = coordX + areaX[i], tempY = coordY + areaY[i];
        if (inBounds(tempX, tempY, tempBoard.sizeX, tempBoard.sizeY) && tempBoard.cells[tempX][tempY].isMined)
        {
            cellValue++;
        }
    }

    return cellValue;
}

void exitGame(Board *realBoard)
{
    con_clear();
    if (realBoard)
    {
        free(realBoard);
    }

    exit(0);
}