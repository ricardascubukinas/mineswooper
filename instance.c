/** 
 * Project's goal: A minesweeper implementation
 * Author: Ričardas Čubukinas
 * Requires game_logic.h, con_lib.h, end.h, instance.h, board.h, save.h, utility.h
 * 
 * 
*/

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

void mainMenuInstance()
{
    int choice;
    Board *realBoard = NULL;
    while (1)
    {

        char *menuOptions[4] = {"New Game", "Load Save", "Show Scoreboard", "Quit"};
        choice = showMenu("Welcome to Minesweeper", menuOptions, 4, "Pick what you want to do");
        if (choice == 0)
        {
            break;
        }
        else if (choice == 1)
        {
            char *fileName = selectLoadFile();
            if (fileName != NULL)
            {
                realBoard = loadBoard(fileName);
                break;
            }
        }
        else if (choice == 2)
        {
            scoreboardInstance();
        }
        else
        {
            return;
        }
    }
    gameInstance(realBoard);

    return;
}

bool inGameMenuInstance(Board *realBoard)
{
    char *gameMenu[4] = {"Save", "Go back", "Quit to main menu", "Quit game"};
    int choice = showMenu("In game menu", gameMenu, 4, "Pick your option");
    if (choice == 0)
    {
        saveBoard(*realBoard, selectSaveFile());
        system("pause");
        printBoard(*realBoard);

        return true;
    }
    else if (choice == 1)
    {
        printBoard(*realBoard);

        return true;
    }
    else if (choice == 2)
    {
        mainMenuInstance();

        return false;
    }
    else
    {
        return false;
    }
}

void gameInstance(Board *realBoard)
{
    int choice;
    if (realBoard == NULL)
    {
        //realBoard = setUpSampleBoard();
        char *difficultyMenu[] = {"Easy", "Normal", "Hard"};
        choice = showMenu("Select a difficulty", difficultyMenu, 3, "Pick your option");
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
    int posX = realBoard->cursorX * 3 + 2, posY = realBoard->cursorY;
    printBoard(*realBoard);
    clock_t gameStart = clock();
    int oldTime = 0;
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
            if (checkForKey(key, 'm'))
            {
                if (!inGameMenuInstance(realBoard))
                {
                    freeUpVariables(realBoard);

                    return;
                }
                //gameStart = clock() + realBoard->timeSeconds;
            }
            if (posY > realBoard->sizeY - 1)
            {
                posY = 0;
            }
            if (posY < 0)
            {
                posY = realBoard->sizeY - 1;
            }

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
                        freeUpVariables(realBoard);

                        return;
                    }
                    else
                    {
                        if (hasWon(*realBoard))
                        {
                            winScreen();
                            freeUpVariables(realBoard);

                            return;
                        }
                        else
                        {
                            printBoard(*realBoard);
                        }
                    }
                }
            }
            if (checkForKey(key, 32) && !realBoard->cells[realBoard->cursorY][realBoard->cursorX].isRevealed)
            {
                if (realBoard->cells[realBoard->cursorY][realBoard->cursorX].isMarked)
                {
                    realBoard->cells[realBoard->cursorY][realBoard->cursorX].isMarked = false;
                    realBoard->markedCount--;
                }
                else if (!realBoard->cells[realBoard->cursorY][realBoard->cursorX].isMarked && realBoard->markedCount < realBoard->mineCount)
                {
                    realBoard->cells[realBoard->cursorY][realBoard->cursorX].isMarked = true;
                    realBoard->markedCount++;
                }
                if (hasWon(*realBoard))
                {
                    winScreen();
                    freeUpVariables(realBoard);

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
        if ((int)(clock() - gameStart) / CLOCKS_PER_SEC > 0)
        {
            gameStart = clock();
            oldTime = realBoard->timeSeconds;
            realBoard->timeSeconds++;
            printTime(*realBoard);
        }
    }

    return;
}
