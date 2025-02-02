#include <stdlib.h>
#include <ncurses.h>
#include "player.h"
#include "map.h"
#include "save_load.h"

Player player;

int process_player_move(int ch) {
    int new_x = player.x;
    int new_y = player.y;
    int floorIndex = current_floor;  // ✅ Ensure correct floor index///////////////////////

    if (ch == KEY_UP || ch == 'w') new_y--;
    else if (ch == KEY_DOWN || ch == 's') new_y++;
    else if (ch == KEY_LEFT || ch == 'a') new_x--;
    else if (ch == KEY_RIGHT || ch == 'd') new_x++;
    else if (ch == 'r') { new_y--; new_x++; }
    else if (ch == 'e') { new_y--; new_x--; }
    else if (ch == 'c') { new_y++; new_x++; }
    else if (ch == 'z') { new_y++; new_x--; }
    else if (ch == 'S') { handle_stairs(); return 1; }
    else if (ch == 'C') { collect_item(); return 1; }
    else if (ch == 'E' && maps[floorIndex][player.y][player.x] == 'E') {
        printw("You won the game!\n");
        refresh();
        save_game_progress("player", floorIndex);
        getch();
        exit(0);
    }
    else return 0;

    if (new_y < 0 || new_y >= MAP_HEIGHT || new_x < 0 || new_x >= MAP_WIDTH) {
        mvprintw(MAP_HEIGHT, 0, "You can't move out of bounds!");
        refresh();
        return 0;
    }

    char target_tile = maps[floorIndex][new_y][new_x];

    if (target_tile == '|' || target_tile == '_') {
        mvprintw(MAP_HEIGHT, 0, "You hit a wall!");
        refresh();
        return 0;
    }
    if (target_tile == ' ') {
        mvprintw(MAP_HEIGHT, 0, "You can't move into empty space!");
        refresh();
        return 0;
    }
    ///////////
    if (player.health <= 0) {
    printw("You have died. Game Over!\n");
    refresh();
    save_game_progress("player", floorIndex);
    getch();
    exit(0);
}


    player.x = new_x;
    player.y = new_y;
    return 1;
}


void collect_item() {
    for (int i = 0; i < MAX_ROOMS; i++) {
        if (items_on_ground[current_floor - 1][i] == 1 &&
            player.x >= rooms[current_floor - 1][i].x &&
            player.x <= rooms[current_floor - 1][i].x + rooms[current_floor - 1][i].width &&
            player.y >= rooms[current_floor - 1][i].y &&
            player.y <= rooms[current_floor - 1][i].y + rooms[current_floor - 1][i].height) {
            
            player.gold += 10;
            items_on_ground[current_floor - 1][i] = 0;  
            printw("Collected gold! Total: %d\n", player.gold);
            refresh();
            return;
        }
    }
    printw("No items here!\n");
    refresh();
}

void update_health_regen() {
    if (player.food > 90 && player.health < 100) {
        player.health += 1;
        printw("Regenerating health: %d\n", player.health);
        refresh();
    }
}

void handle_stairs() {
    int floorIndex = current_floor;  // ✅ Get current floor index

    if (maps[floorIndex][player.y][player.x] == 'U') {
        printw("Going up to Floor %d...\n", floorIndex + 2);
        refresh();
        save_game_progress("player", floorIndex);
        
        current_floor++;  // ✅ Move to the next floor
        
        // Move player to the downward stair on the new floor
        player.x = stair_down[current_floor][0];
        player.y = stair_down[current_floor][1];

        display_map_with_player();  // ✅ Force update
    } 
    else if (maps[floorIndex][player.y][player.x] == 'D') {
        printw("Going down to Floor %d...\n", floorIndex);
        refresh();
        save_game_progress("player", floorIndex);
        
        current_floor--;  // ✅ Move to the previous floor
        
        // Move player to the upward stair on the new floor
        player.x = stair_up[current_floor][0];
        player.y = stair_up[current_floor][1];

        display_map_with_player();  // ✅ Force update
    } 
    else {
        printw("No stairs here!\n");
        refresh();
    }
}



void initialize_player(int floorIndex) {  // ✅ floor is now 0-based
    if (floorIndex == 0) { 
        // player.x = stair_up[floorIndex][0] + 1;
        // player.y = stair_up[floorIndex][1] + 1;
        player.x=rooms[0][7].x +2;
        player.y=rooms[0][7].y +2;
    } else { 
        player.x = stair_down[floorIndex][0] + 1;
        player.y = stair_down[floorIndex][1] + 1;
    }

    player.gold = 0;
    player.food = 100;
    player.health = 100;

    printw("Player initialized on Floor %d at (%d, %d).\n", floorIndex + 1, player.x, player.y);
    refresh();
}

void display_player_position() {
    mvprintw(MAP_HEIGHT + 1, 0, "Player: (%d, %d) | Floor: %d | Health: %d | Food: %d",
             player.x, player.y, current_floor, player.health, player.food);
    refresh();
}


int player_reached_stairs(int floorIndex) {
    int up_x = stair_up[floorIndex][0], up_y = stair_up[floorIndex][1];
    int down_x = stair_down[floorIndex][0], down_y = stair_down[floorIndex][1];
    return ((player.x == up_x && player.y == up_y) || (player.x == down_x && player.y == down_y));
}

