#include "instance.h"

bool wasPressed[128];

int showMenu(char *menuTitle, char *menuOptions[], int menuSize, char *inputMsg)
{
    con_clear();
    wasPressed[13] = true;
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
            con_set_pos(0, posY);
            fflush(stdout);
        }
    }
}

void menuInstance()
{
    Board *realBoard = NULL;
    char *menuOptions[4] = {"New Game", "Load Save", "Show Scoreboard", "Quit"};
    int choice = showMenu("Welcome to Minesweeper", menuOptions, 4, "Pick what you want to do");
    if (choice == 0)
    {
        gameInstance(realBoard);

        exitGame(realBoard);
    }
    else if (choice == 1)
    {
        char *fileName = selectLoadFile();
        if (fileName == NULL)
        {
            menuInstance();
        }
        else
        {
            realBoard = loadBoard(fileName);
            gameInstance(realBoard);
        }

        return;
    }
    else if (choice == 2)
    {
        showScoreboard();
        char *menuScoreboard[1] = {"Go back"};
        showMenu("Welcome to the scoreboard", menuScoreboard, 1, "Pick what you want to do");
        menuInstance();

        exitGame(realBoard);
    }
    else
    {

        exitGame(realBoard);
    }
}

void inGameMenu(Board *realBoard, int markedCount)
{
    char *gameMenu[4] = {"Save", "Go back", "Quit to main menu", "Quit game"};
    int choice = showMenu("In game menu", gameMenu, 4, "Pick your option");
    if (choice == 0)
    {
        saveBoard(*realBoard, selectSaveFile());
        system("pause");
        printBoard(*realBoard, markedCount);
    }
    else if (choice == 1)
    {
        printBoard(*realBoard, markedCount);
    }
    else if (choice == 2)
    {
        menuInstance();
        exitGame(realBoard);
    }
    else
    {
        exitGame(realBoard);
    }

    return;
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

void showScoreboard()
{
    /* Not implemented yet */

    return;
}