#ifndef PLAYER_H
#define PLAYER_H

#define MAX_INVENTORY 10

typedef struct {
    int x, y;
    int gold;
    int food;
    int health;
    int inventory[MAX_INVENTORY];  // Items collected
} Player;

extern Player player;
extern int current_floor;  // ✅ Ensure it's globally accessible

void initialize_player(int floorIndex);  // ✅ Updated function signature
int process_player_move(int ch);
void handle_stairs();
void collect_item();
void update_health_regen();
void display_player_position();
int player_reached_stairs(int floorIndex);  // ✅ Updated function signature

#endif
