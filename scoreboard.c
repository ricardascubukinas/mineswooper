#include "scoreboard.h"

void saveToScoreboard(Player save)
{
    FILE *fr = fopen("scoreboard.txt", "a");
    if (fr != NULL)
    {
        fprintf(fr, "%s %i\n", save.name, save.time);
    }

    fclose(fr);

    int count = getCount() + 1;

    fr = fopen("count.txt", "w");
    if (fr != NULL)
    {
        fprintf(fr, "%i", count);
    }

    fclose(fr);

    return;
}

void viewScoreboard()
{
    int count = getCount();
    if (count > 0)
    {
        FILE *fr = fopen("scoreboard.txt", "r");
        Player *scoreboard;
        if (fr != NULL)
        {
            scoreboard = malloc(sizeof(Player) * count);
            for (int i = 0; i < count; i++)
            {
                scoreboard[i].name = malloc(sizeof(char) * NAME_MAX);
                fscanf(fr, "%s %i", scoreboard[i].name, &scoreboard[i].time);
            }
            scoreboard = sortScoreboard(scoreboard, count);
        }
        fclose(fr);

        printf("%14sScoreboard\n\n", " ");
        printf("   %23s %7s\n", "Vardas", "Laikas");
        for (int i = 0; i < SHOW_TOP && i < count; i++)
        {
            printf("%4i. %20s %7i\n", i + 1, scoreboard[i].name, scoreboard[i].time);
        }
    }

    system("pause");

    return;
}

int getCount()
{
    int count = 0;
    FILE *fr = fopen("count.txt", "r");
    if (fr != NULL)
    {
        fscanf(fr, "%i", &count);
    }
    fclose(fr);

    return count;
}