#ifndef SAVE_LOAD_H
#define SAVE_LOAD_H

#include "player.h"
#include "map.h"

#define MAX_FLOORS 4

void save_game_progress(const char *username, int floor);
int load_saved_game(const char *username, const char *password);

#endif


