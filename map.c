#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include "map.h"
#include "player.h"  // For debugging messages (if needed)

// Global arrays: separate map for each floor.
char maps[MAX_FLOORS][MAP_HEIGHT][MAP_WIDTH];  // Each floor’s map
Room rooms[MAX_FLOORS][MAX_ROOMS];             // Rooms per floor
int room_count[MAX_FLOORS] = {0};                // Room count per floor

// Stair positions for each floor
int stair_up[MAX_FLOORS][2];    // [floor][0]=x, [floor][1]=y for upward stair
int stair_down[MAX_FLOORS][2];  // for downward stair

// Debug: current floor (for display and player functions)
int current_floor = 0;

// These arrays track fog of war and dropped items; they remain unchanged here.
int seen_room[MAX_FLOORS][MAX_ROOMS] = {0};
int items_on_ground[MAX_FLOORS][MAX_ROOMS] = {0};

void display_map_with_player() {
    clear();
    printw("Displaying map for Floor %d...\n", current_floor + 1);  // ✅ Display correct floor number
    refresh();

    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            if (i == player.y && j == player.x) {
                printw("@");
            } else {
                printw("%c", maps[current_floor][i][j]);  // ✅ Use correct floor map
            }
        }
        printw("\n");
    }
    mvprintw(MAP_HEIGHT + 1, 0, "Health: %d | Food: %d", player.health, player.food);
    refresh();
}




void place_win_condition() {
    int last_room_x = rooms[3][7].x + rooms[3][7].width / 2;
    int last_room_y = rooms[3][7].y + rooms[3][7].height / 2;
    maps[3][last_room_y][last_room_x] = 'E';
}




// Initializes an empty map for a given floor.
void initialize_map(int floor) {
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            maps[floor][i][j] = ' ';
        }
    }
}

// Draws a room on the floor’s map by placing walls and floors.
void add_room_with_walls(Room room, int floor) {
    for (int i = room.y - 1; i <= room.y + room.height; i++) {
        for (int j = room.x - 1; j <= room.x + room.width; j++) {
            if (i == room.y - 1 || i == room.y + room.height) {
                maps[floor][i][j] = '_';  // Top or bottom wall
            } else if (j == room.x - 1 || j == room.x + room.width) {
                maps[floor][i][j] = '|';  // Left or right wall
            } else {
                maps[floor][i][j] = '.';  // Floor
            }
        }
    }
}

// Creates a corridor between two rooms (using a simple L-shape connection).
// (This function is not changed in logic from before.)
void create_corridor(Room room1, Room room2, int part, int floor) {
    int start_x, start_y, end_x, end_y;

    if (part < 3) {
        start_x = room1.x + room1.width;
        start_y = room1.y + rand() % room1.height;
        end_x = room2.x - 1;
        end_y = room2.y + rand() % room2.height;
    } else if (part == 3) {
        start_x = room1.x + rand() % room1.width;
        start_y = room1.y - 1;
        end_x = room2.x + rand() % room2.width;
        end_y = room2.y + room2.height;
    } else {
        start_x = room1.x - 1;
        start_y = room1.y + rand() % room1.height;
        end_x = room2.x + room2.width;
        end_y = room2.y + rand() % room2.height;
    }

    printw("Creating corridor from (%d, %d) to (%d, %d) on Floor %d\n", start_x, start_y, end_x, end_y, floor + 1);
    refresh();

    maps[floor][start_y][start_x]='#';
    maps[floor][end_y][end_x]='#';



    int mid_x = start_x;
    int mid_y = start_y;




if (part < 3) {
        while (mid_x != end_x - 1) {
            mid_x++;
            if (mid_x >= 0 && mid_x < MAP_WIDTH) maps[floor][mid_y][mid_x] = '#';
            else {
                printw("Error: mid_x out of bounds at (%d, %d)\n", mid_x, mid_y);
                refresh();
                return;
            }
        }
        while (mid_y != end_y) {
            mid_y += (end_y > mid_y) ? 1 : -1;
            if (mid_y >= 0 && mid_y < MAP_HEIGHT) maps[floor][mid_y][mid_x] = '#';
            else {
                printw("Error: mid_y out of bounds at (%d, %d)\n", mid_x, mid_y);
                refresh();
                return;
            }
        }
    } else if (part == 3) {
        while (mid_y != end_y + 1) {
            mid_y--;
            if (mid_y >= 0 && mid_y < MAP_HEIGHT) maps[floor][mid_y][mid_x] = '#';
            else {
                printw("Error: mid_y out of bounds at (%d, %d)\n", mid_x, mid_y);
                refresh();
                return;
            }
        }
        while (mid_x != end_x) {
            mid_x += (end_x > mid_x) ? 1 : -1;
            if (mid_x >= 0 && mid_x < MAP_WIDTH) maps[floor][mid_y][mid_x] = '#';
            else {
                printw("Error: mid_x out of bounds at (%d, %d)\n", mid_x, mid_y);
                refresh();
                return;
            }
        }
    } else if (part >= 4 && part <= 6) {
        while (mid_x != end_x + 1) {
            mid_x--;
            if (mid_x >= 0 && mid_x < MAP_WIDTH) maps[floor][mid_y][mid_x] = '#';
            else {
                printw("Error: mid_x out of bounds at (%d, %d)\n", mid_x, mid_y);
                refresh();
                return;
            }
        }
        while (mid_y != end_y) {
            mid_y += (end_y > mid_y) ? 1 : -1;
            if (mid_y >= 0 && mid_y < MAP_HEIGHT) maps[floor][mid_y][mid_x] = '#';
            else {
                printw("Error: mid_y out of bounds at (%d, %d)\n", mid_x, mid_y);
                refresh();
                return;
            }
        }
    }
    // while (mid_x != end_x) {
    //     mid_x += (end_x > mid_x) ? 1 : -1;
    //     maps[floor][mid_y][mid_x] = '#';
    // }
    // while (mid_y != end_y) {
    //     mid_y += (end_y > mid_y) ? 1 : -1;
    //     maps[floor][mid_y][mid_x] = '#';
    // }
}

// Places stairs on each floor based on your design.
// Floor 1: only an upward stair in Room 1 (index 0)
// Floor 2: downward stair in Room 1 (index 0) and upward stair in Room 8 (index 7)
// Floor 3: upward stair in Room 1 (index 0) and downward stair in Room 8 (index 7)
// Floor 4: only a downward stair in Room 1 (index 0)
void place_stairs() {
    // Floor 1
    stair_up[0][0] = rooms[0][0].x + rooms[0][0].width / 2;
    stair_up[0][1] = rooms[0][0].y + rooms[0][0].height / 2;
    maps[0][stair_up[0][1]][stair_up[0][0]] = 'U';
    printw("Floor 1 stair (up) placed at (%d, %d)\n", stair_up[0][0], stair_up[0][1]);
    refresh();

    // Floor 2
    stair_down[1][0] = rooms[1][0].x + rooms[1][0].width / 2;
    stair_down[1][1] = rooms[1][0].y + rooms[1][0].height / 2;
    maps[1][stair_down[1][1]][stair_down[1][0]] = 'D';
    stair_up[1][0] = rooms[1][7].x + rooms[1][7].width / 2;
    stair_up[1][1] = rooms[1][7].y + rooms[1][7].height / 2;
    maps[1][stair_up[1][1]][stair_up[1][0]] = 'U';
    printw("Floor 2 stair (down) at (%d, %d), stair (up) at (%d, %d)\n",
           stair_down[1][0], stair_down[1][1], stair_up[1][0], stair_up[1][1]);
    refresh();

    // Floor 3
    stair_up[2][0] = rooms[2][0].x + rooms[2][0].width / 2;
    stair_up[2][1] = rooms[2][0].y + rooms[2][0].height / 2;
    maps[2][stair_up[2][1]][stair_up[2][0]] = 'U';
    stair_down[2][0] = rooms[2][7].x + rooms[2][7].width / 2;
    stair_down[2][1] = rooms[2][7].y + rooms[2][7].height / 2;
    maps[2][stair_down[2][1]][stair_down[2][0]] = 'D';
    printw("Floor 3 stair (up) at (%d, %d), stair (down) at (%d, %d)\n",
           stair_up[2][0], stair_up[2][1], stair_down[2][0], stair_down[2][1]);
    refresh();

    // Floor 4
    stair_down[3][0] = rooms[3][0].x + rooms[3][0].width / 2;
    stair_down[3][1] = rooms[3][0].y + rooms[3][0].height / 2;
    maps[3][stair_down[3][1]][stair_down[3][0]] = 'D';
    printw("Floor 4 stair (down) placed at (%d, %d)\n", stair_down[3][0], stair_down[3][1]);
    refresh();
}

// Generates the map for all floors.
void generate_map() {
    srand(time(NULL));

    int part_bounds[8][4] = {
        {0, 13, 19, 25},  {20, 13, 39, 25},  {40, 13, 59, 25},  {60, 13, 79, 25},
        {60, 0, 79, 12},  {40, 0, 59, 12},   {20, 0, 39, 12},   {0, 0, 19, 12}
    };

    for (int floor = 0; floor < MAX_FLOORS; floor++) {
        initialize_map(floor);
        room_count[floor] = 0;

        for (int part = 0; part < 8; part++) {
            int x_min = part_bounds[part][0];
            int y_min = part_bounds[part][1];
            int x_max = part_bounds[part][2];
            int y_max = part_bounds[part][3];

            Room room;
            room.width = rand() % 6 + 4;
            room.height = rand() % 6 + 4;
            room.x = x_min + 2 + rand() % (x_max - x_min - room.width - 2);
            room.y = y_min + 2 + rand() % (y_max - y_min - room.height - 2);

            add_room_with_walls(room, floor);
            rooms[floor][room_count[floor]++] = room;

            printw("Floor %d: Room %d placed in Part %d: x=%d, y=%d, w=%d, h=%d\n",
                   floor + 1, room_count[floor], part + 1, room.x, room.y, room.width, room.height);
            refresh();
        }

        for (int part = 0; part < 7; part++) {
            printw("Connecting Floor %d, Part %d to Part %d...\n", floor + 1, part + 1, part + 2);
            refresh();
            create_corridor(rooms[floor][part], rooms[floor][part + 1], part, floor);
        }
    }

    // 🔥 Only call `place_stairs()` once here!
    printw("Placing stairs...\n");
    refresh();
    place_stairs();

    place_win_condition();

    printw("Maps for all floors successfully generated with %d rooms per floor.\n", room_count[0]);
    refresh();
}
