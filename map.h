#ifndef MAP_H
#define MAP_H

#define MAP_WIDTH 80
#define MAP_HEIGHT 26
#define MAX_ROOMS 8
#define MAX_FLOORS 4

typedef struct {
    int x, y;
    int width, height;
} Room;

extern int current_floor;  // ✅ Ensure consistency across files
extern char maps[MAX_FLOORS][MAP_HEIGHT][MAP_WIDTH];  
extern Room rooms[MAX_FLOORS][MAX_ROOMS];  
extern int room_count[MAX_FLOORS];  
extern int seen_room[MAX_FLOORS][MAX_ROOMS];  
extern int items_on_ground[MAX_FLOORS][MAX_ROOMS];  

// Stair positions
extern int stair_up[MAX_FLOORS][2];  
extern int stair_down[MAX_FLOORS][2];

void generate_map();
void initialize_map(int floor);
void add_room_with_walls(Room room, int floor);
void create_corridor(Room room1, Room room2, int part, int floor);
void display_map_with_player();
void place_stairs();

#endif
