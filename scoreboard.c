#include "scoreboard.h"

void saveToScoreboard(Player save)
{
    FILE *fr = fopen("scoreboard.txt", "a");
    fprintf(fr, "%s %i\n", save.name, save.time);
    fclose(fr);

    int count = getCount() + 1;

    fr = fopen("count.txt", "w");
    fprintf(fr, "%i", count);
    fclose(fr);

    return;
}

void viewScoreboard()
{
    int count = getCount();

    FILE *fr = fopen("scoreboard.txt", "r");

    Player *scoreboard = malloc(sizeof(Player) * count);
    for (int i = 0; i < count; i++)
    {
        scoreboard[i].name = malloc(sizeof(char) * NAME_MAX);
        fscanf(fr, "%s %i", scoreboard[i].name, &scoreboard[i].time);
    }
    scoreboard = sortScoreboard(scoreboard, count);
    fclose(fr);
    printf("%14sScoreboard\n\n", " ");
    printf("   %23s %7s\n", "Vardas", "Laikas");
    for (int i = 0; i < SHOW_TOP; i++)
    {
        printf("%4i. %20s %7i\n", i + 1, scoreboard[i].name, scoreboard[i].time);
    }
    system("pause");

    return;
}

int getCount()
{
    int count = 0;
    FILE *fr = fopen("count.txt", "r");
    fscanf(fr, "%i", &count);
    fclose(fr);

    return count;
}