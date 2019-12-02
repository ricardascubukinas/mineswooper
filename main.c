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
#include "menu.h"
#include "board.h"
#include "save.h"

#define BOARD_SIZE 6
#define MINE_COUNT 1

int areaX[9] = {0, -1, 0, 1, 1, 1, 0, -1, -1};
int areaY[9] = {0, -1, -1, -1, 0, 1, 1, 1, 0};

void gameInstance(Board *realBoard);
Board *reveal(Board *tempBoard, bool isFirst);
Board *checkForFails(Board *tempBoard);
Board *setUpBoard(Board *realBoard, int height, int width, int mines);
void printBoard(Board tempBoard, int markedCount);
void showScoreboard();
int randInt(int low, int high);

int countUnrevealedCells(Board tempBoard);
int countNearbyMines(Board tempBoard, int coordX, int coordY);
bool inBounds(int coordX, int coordY, int sizeX, int sizeY);
bool checkForKey(int press, int key);

int main()
{
    srand(time(NULL));
    for (int i = 0; i < 128; i++)
    {
        wasPressed[i] = false;
    }

    menuInstance();

    con_clear();
    return 0;
}

void gameInstance(Board *realBoard)
{
    if (realBoard == NULL)
    {
        //realBoard = setUpSampleBoard();
        char *difficultyMenu[] = {"Easy", "Normal", "Hard"};
        int choice = showMenu("Select a difficulty", difficultyMenu, 3, "Pick your option");
        int width, height, mineCount;
        if (choice == 0)
        {
            realBoard = setUpBoard(realBoard, 10, 10, 10);
        }
        else if (choice == 1)
        {
            realBoard = setUpBoard(realBoard, 15, 15, 30);
        }
        else
        {
            realBoard = setUpBoard(realBoard, 24, 24, 99);
        }
        //saveBoard(*realBoard, "save.bin");
        //realBoard = loadBoard("save.bin");
    }
    int posX = 2, posY = 0, markedCount = 0;
    printBoard(*realBoard, markedCount);

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
            if (checkForKey(key, 'q'))
            {
                return;
            }
            if (checkForKey(key, 'm'))
            {
                inGameMenu(realBoard, markedCount);
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
                if (!realBoard->cells[realBoard->cursorY][realBoard->cursorX].isRevealed)
                {
                    realBoard = reveal(realBoard, true);
                    if (realBoard == NULL)
                    {
                        loseScreen();
                        return;
                    }
                    else
                    {
                        if (hasWon(*realBoard))
                        {
                            winScreen();
                            return;
                        }
                        else
                        {
                            printBoard(*realBoard, markedCount);
                        }
                    }
                }
            }
            if (checkForKey(key, 32) && !realBoard->cells[realBoard->cursorY][realBoard->cursorX].isRevealed)
            {
                if (realBoard->cells[realBoard->cursorY][realBoard->cursorX].isMarked)
                {
                    realBoard->cells[realBoard->cursorY][realBoard->cursorX].isMarked = false;
                    markedCount--;
                }
                else if (!realBoard->cells[realBoard->cursorY][realBoard->cursorX].isMarked && markedCount < realBoard->mineCount)
                {
                    realBoard->cells[realBoard->cursorY][realBoard->cursorX].isMarked = true;
                    markedCount++;
                }
                if (hasWon(*realBoard))
                {
                    winScreen();
                    return;
                }
                else
                {
                    printBoard(*realBoard, markedCount);
                }
            }

            con_set_color(COLOR_BLACK, COLOR_GRAY);
            con_set_pos(posX, posY);
            fflush(stdout);
        }
    }

    return;
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
                    if (!tempBoard->cells[coordY][coordX].isRevealed && !tempBoard->cells[coordY][coordX].isMined && !tempBoard->cells[coordY][coordX].isMarked)
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
            if (!tempBoard->cells[i][j].isRevealed && !tempBoard->cells[i][j].isMarked)
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

void printBoard(Board tempBoard, int markedCount)
{
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
                printf("%3c", '*');
            }
        }
        printf("\n");
    }
    printf("  Mines left: %i\n", tempBoard.mineCount - markedCount);
    printf("  Controls: w - up, s - down, a - left, d - right\n");
    printf("  ENTER - reveal a cell, SPACE - mark a cell, m - open menu\n");

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

int countUnrevealedCells(Board tempBoard)
{
    int unrevealedCount = 0;
    for (int i = 0; i < tempBoard.sizeY; i++)
    {
        for (int j = 0; j < tempBoard.sizeX; j++)
        {
            if (!tempBoard.cells[i][j].isRevealed)
            {
                unrevealedCount++;
            }
        }
    }

    return unrevealedCount;
}
