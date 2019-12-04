#include "end.h"

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

void exitGame(Board *realBoard)
{
    con_clear();
    if (!realBoard)
    {
        free(realBoard);
    }
    exit(0);
}