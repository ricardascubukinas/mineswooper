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
void inGameMenu(Board *realBoard, int markedCount);
Board *reveal(Board *tempBoard, bool isFirst);
Board *checkForFails(Board *tempBoard);
Board *setUpBoard(Board *realBoard, int height, int width, int mines);
void loseScreen();
void winScreen();
void printBoard(Board tempBoard, int markedCount);
void showScoreboard();
int randInt(int low, int high);
void saveBoard(Board realBoard, char *fileName);
char *selectSaveFile();
Board *loadSave(char *fileName);
bool checkForKey(int press, int key);
bool inBounds(int coordX, int coordY, int sizeX, int sizeY);
int countNearbyMines(Board tempBoard, int x, int y);
int countUnrevealedCells(Board tempBoard);
void exitGame(Board *realBoard);
bool hasWon(Board tempBoard);

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
        char *fileName = selectSaveFile();
        if (fileName == NULL)
        {
            menuInstance();
        }
        else
        {
            realBoard = loadSave(fileName);
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
                con_clear();
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
            if (checkForKey(key, 32))
            {
                if (realBoard->cells[realBoard->cursorY][realBoard->cursorX].isMarked)
                {
                    realBoard->cells[realBoard->cursorY][realBoard->cursorX].isMarked = false;
                    markedCount--;
                }
                else if ((!realBoard->cells[realBoard->cursorY][realBoard->cursorX].isMarked && !realBoard->cells[realBoard->cursorY][realBoard->cursorX].isRevealed) && markedCount < realBoard->mineCount)
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

void inGameMenu(Board *realBoard, int markedCount)
{
    char *gameMenu[4] = {"Save", "Go back", "Quit to main menu", "Quit game"};
    int choice = showMenu("In game menu", gameMenu, 4, "Pick your option");
    if (choice == 0)
    {
        char fileName[1000];
        while (1)
        {
            /* TODO: Implement check for correct file names*/
            printf("Enter a file name(to save in current directoy), don't type in the extension, maximum length 100 characters\n");
            fileName[0] = '\0';
            fgets(fileName, 1000, stdin);
            strtok(fileName, "\n");
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
                printf("Success! Proceeding to save to file %s.bin.\n", fileName);
                break;
            }
        }
        strcat(fileName, ".bin");
        saveBoard(*realBoard, fileName);
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

void loseScreen()
{
    con_clear();
    printf("You've lost\n");
    system("pause");
    menuInstance();

    exitGame(NULL);
}

void winScreen()
{
    con_clear();
    printf("You've won\n");
    system("pause");
    menuInstance();

    exitGame(NULL);
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

char *selectSaveFile()
{
    WIN32_FIND_DATA findFile;
    HANDLE handleFind = NULL;
    char searchDir[2000];
    char searchPath[2048];

    while (1)
    {
        printf("Enter a directory name(maximum length is 100) where to look for the save, or press enter to check current directory:\n");
        printf("You can type in /q to go back to the menu\n");
        fgets(searchDir, 100, stdin);
        if (searchDir[0] == '\n')
        {
            getcwd(searchDir, FILENAME_MAX);
        }
        strtok(searchDir, "\n");
        if (strcmp(searchDir, "/q") == 0)
        {
            menuInstance();

            return NULL;
        }
        sprintf(searchPath, "%s\\*.bin", searchDir);
        handleFind = FindFirstFile(searchPath, &findFile);
        if ((handleFind = FindFirstFile(searchPath, &findFile)) == INVALID_HANDLE_VALUE)
        {

            printf("Path not found: [%s] or no .bin files in directory\n", searchDir);
            handleFind = NULL;
        }
        else
        {
            break;
        }
    }

    char **files = (char **)malloc(100 * sizeof(char *));
    for (int i = 0; i < 100; i++)
    {
        files[i] = (char *)malloc(100 * sizeof(char));
    }
    int fileCount = 0;

    do
    {
        if (strcmp(findFile.cFileName, ".") != 0 && strcmp(findFile.cFileName, "..") != 0)
        {
            sprintf(searchPath, "%s\\%s", searchDir, findFile.cFileName);
            if (!(findFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                strcpy(files[fileCount], searchPath);
                fileCount++;
            }
        }
    } while (FindNextFile(handleFind, &findFile));

    FindClose(handleFind);
    if (fileCount > 0)
    {
        return files[showMenu("Pick the file to load", files, fileCount, "Pick your option")];
    }
    else
    {
        return NULL;
    }
}

void saveBoard(Board realBoard, char *fileName)
{
    FILE *fw = fopen(fileName, "wb");
    fwrite(&realBoard, sizeof(Board), 1, fw);
    for (int i = 0; i < realBoard.sizeY; i++)
    {
        fwrite(realBoard.cells[i], sizeof(Cell), realBoard.sizeX, fw);
    }
    fclose(fw);
}

Board *loadSave(char *fileName)
{
    Board *loadedBoard = malloc(sizeof(Board));
    FILE *fr = fopen(fileName, "rb");

    if (fread(loadedBoard, sizeof(Board), 1, fr) == 1) // True statement
    {
        //Cell **temp = (Cell **)malloc(loadedBoard->sizeY * sizeof(Cell *));
        loadedBoard->cells = (Cell **)malloc(loadedBoard->sizeY * sizeof(Cell *));
        for (int i = 0; i < loadedBoard->sizeY; i++)
        {
            loadedBoard->cells[i] = (Cell *)malloc(loadedBoard->sizeX * sizeof(Cell));
            fread(loadedBoard->cells[i], sizeof(Cell), loadedBoard->sizeX, fr);
        }
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

void exitGame(Board *realBoard)
{
    con_clear();
    if (!realBoard)
    {
        free(realBoard);
    }
    exit(0);
}