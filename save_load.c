#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "save_load.h"
#include "map.h"
#include "player.h"
#include "scoreboard.h"

typedef struct {
    int floor;
    int gold;
    int food;
    int health;
    int experience;
    int plays;
    int seen_rooms[MAX_ROOMS];
    int map_items[MAX_ROOMS];
    int player_x, player_y;
    char maps[MAX_FLOORS][MAP_HEIGHT][MAP_WIDTH]; // Store full map per floor
} SaveData;

char save_file[50];

void save_game_progress(const char *username, int floor) {
    snprintf(save_file, sizeof(save_file), "save_%s.dat", username);
    FILE *file = fopen(save_file, "wb");
    if (!file) {
        printf("Error saving game!\n");
        return;
    }

    SaveData data;
    data.floor = floor;
    data.gold = player.gold;
    data.food = player.food;
    data.health = player.health;
    data.experience = scoreboard_data[floor - 1].experience;
    data.plays = scoreboard_data[floor - 1].plays;
    data.player_x = player.x;
    data.player_y = player.y;

    for (int i = 0; i < MAX_ROOMS; i++) {
        data.seen_rooms[i] = seen_room[floor - 1][i];
        data.map_items[i] = items_on_ground[floor - 1][i];
    }

    // Copy full map data for all floors
    memcpy(data.maps, maps, sizeof(maps));
    
    fwrite(&data, sizeof(SaveData), 1, file);
    fclose(file);
    printf("Game progress saved for %s.\n", username);
}

int load_saved_game(const char *username, const char *password) {
    (void)password;  // Mark password as unused to avoid warning
    snprintf(save_file, sizeof(save_file), "save_%s.dat", username);
    FILE *file = fopen(save_file, "rb");
    if (!file) {
        printf("No saved game found.\n");
        return 0;
    }

    SaveData data;
    fread(&data, sizeof(SaveData), 1, file);
    fclose(file);

    player.gold = data.gold;
    player.food = data.food;
    player.health = data.health;
    player.x = data.player_x;
    player.y = data.player_y;
    scoreboard_data[data.floor - 1].experience = data.experience;
    scoreboard_data[data.floor - 1].plays = data.plays;

    for (int i = 0; i < MAX_ROOMS; i++) {
        seen_room[data.floor - 1][i] = data.seen_rooms[i];
        items_on_ground[data.floor - 1][i] = data.map_items[i];
    }
    
    // Restore full map data
    memcpy(maps, data.maps, sizeof(maps));

    printf("Game loaded: Floor %d\n", data.floor);
    return 1;
}
