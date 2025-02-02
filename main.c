#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include "map.h"
#include "player.h"
#include "menu.h"
#include "settings.h"
#include "save_load.h"
#include "scoreboard.h"

#define MAX_TRACKS 10

void start_game(int load_saved, char *username, char *password) {
    clear();

    if (load_saved) {
        if (!load_saved_game(username, password)) {
            printw("Failed to load saved game. Starting a new game instead.\n");
            refresh();
            getch();
            load_saved = 0;
        }
    }

    printw("Starting the game...\n");
    refresh();

    current_floor = 0;  // ✅ Start at Floor 0 (Fix indexing)
    generate_map();
    initialize_player(current_floor);

    clear();
    display_map_with_player();
    display_player_position();

    int ch;
    while ((ch = getch()) != 'q') {
        if (process_player_move(ch)) {
            clear();
            display_map_with_player();
            display_player_position();
        }

        // ✅ WIN CONDITION: Press 'E' in Room [3][7]
        if (player.x == rooms[3][7].x && player.y == rooms[3][7].y && ch == 'E') {
            printw("🎉 YOU WIN! 🎉\n");
            update_scoreboard(username, 50);  // ✅ Give 50 points for winning
            refresh();
            getch();
            return;
        }

        // ✅ LOSE CONDITION: Health = 0
        if (player.health <= 0) {
            printw("💀 GAME OVER! 💀\n");
            update_scoreboard(username, 10);  // ✅ Give 10 points for playing
            refresh();
            getch();
            return;
        }
    }

    printw("Game Over. Returning to Main Menu...\n");
    refresh();
    getch();
}



// // Global settings
// int difficulty = 1;    // 1: Easy, 2: Medium, 3: Hard
// int sound_enabled = 1; // 1: ON, 0: OFF
// char *music_tracks[MAX_TRACKS]; // Store available music tracks
// int track_count = 0;
// int selected_track = 0; // Index of the chosen music track





int main() {
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    srand(time(NULL));

    int menu_choice;

    while (1) {
        menu_choice = display_menu();
        switch (menu_choice) {
            case 1: // Start Game Menu
                start_game_menu();
                break;
            case 2: // Settings
                open_settings();
                break;
            case 3: // Contacts Menu
                endwin();
                contacts_menu();
                initscr();
                break;
            case 4: // Scoreboard
                show_scoreboard();

                break;
            case 5: // Exit
                endwin();
                return 0;
            default:
                printw("Invalid choice. Please try again.\n");
                refresh();
                getch();
                break;
        }
    }

    endwin();
    return 0;
}
