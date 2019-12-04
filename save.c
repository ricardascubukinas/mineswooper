/** 
 * Project's goal: A minesweeper implementation
 * Author: Ričardas Čubukinas
 * Requires game_logic.h, con_lib.h, end.h, instance.h, board.h, save.h, utility.h
 * 
 * 
*/

#include "save.h"

char *selectSaveFile()
{
    char *fileName = malloc(sizeof(char) * 1000);
    while (1)
    {
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

    return fileName;
}

char *selectLoadFile()
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
            mainMenuInstance();

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

Board *loadBoard(char *fileName)
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