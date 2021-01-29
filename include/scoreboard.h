#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include "instance.h"
#include "player.h"

#define NAME_MAX 20
#define SHOW_TOP 10

void saveToScoreboard(Player save);
void viewScoreboard();
int getCount();

#endif