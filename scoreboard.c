#include "scoreboard.h"

void scoreboardInstance()
{
    char *menuScoreboard[1] = {"Go back"};
    showMenu("Welcome to the scoreboard", menuScoreboard, 1, "Pick what you want to do");
    viewScoreboard();
    return;
}

void viewScoreboard()
{
}