#include "menu.h"

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