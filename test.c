#include <stdio.h>
#include <stdlib.h>

void saveToFile(int **board);
int **loadFromFile();

int main()
{
    int **board = (int **)malloc(sizeof(int *) * 3);
    for (int i = 0; i < 3; i++)
    {
        board[i] = (int *)malloc(sizeof(int) * 3);
    }
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            board[i][j] = i * 3 + j + 1;
        }
    }
    saveToFile(board);
    board = loadFromFile();

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("%i ", board[i][j]);
        }
    }

    return 0;
}

void saveToFile(int **board)
{

    FILE *fw = fopen("save.bin", "wb");
    for (int i = 0; i < 3; i++)
    {
        fwrite(board[i], sizeof(int), 3, fw);
    }

    fclose(fw);

    return;
}

int **loadFromFile()
{
    FILE *fr = fopen("save.bin", "rb");
    int **board = (int **)malloc(sizeof(int *) * 3);
    for (int i = 0; i < 3; i++)
    {
        board[i] = (int *)malloc(sizeof(int) * 3);
        fread(board[i], sizeof(int), 3, fr);
    }

    fclose(fr);

    return board;
}
