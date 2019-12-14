/** 
 * Project's goal: A minesweeper implementation
 * Author: Ričardas Čubukinas
 * Requires game_logic.h, con_lib.h, end.h, instance.h, board.h, save.h, utility.h
 * 
 * 
*/

#include "game_logic.h"

Board *setUpBoard(Board *realBoard, int height, int width, int mines)
{
    realBoard = malloc(sizeof(Board));
    realBoard->markedCount = 0;
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

bool hasWon(Board tempBoard)
{
    int unrevealedCount = countUnrevealedCells(tempBoard), markedCorrectCount = 0;
    for (int i = 0; i < tempBoard.sizeY; i++)
    {
        for (int j = 0; j < tempBoard.sizeX; j++)
        {
            if (tempBoard.cells[i][j].isMarked && tempBoard.cells[i][j].isMined)
            {
                markedCorrectCount++;
            }
        }
    }
    if (unrevealedCount == tempBoard.mineCount || markedCorrectCount == tempBoard.mineCount)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void loseScreen()
{
    con_clear();
    printf("You've lost\n");
    system("pause");
    mainMenuInstance();
}

void winScreen()
{
    con_clear();
    printf("You've won\n");
    system("pause");
    mainMenuInstance();
}