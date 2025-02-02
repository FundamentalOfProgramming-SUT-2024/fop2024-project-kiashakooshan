#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "settings.h"

#define MAX_TRACKS 10

// Global settings
int difficulty = 1;    // 1: Easy, 2: Medium, 3: Hard
int sound_enabled = 1; // 1: ON, 0: OFF
char *music_tracks[MAX_TRACKS]; // Store available music tracks
int track_count = 0;
int selected_track = 0; // Index of the chosen music track

void load_music_tracks() {
    // Hardcoded list of available tracks (You can modify this dynamically later)
    music_tracks[0] = "music/track1.mp3";
    music_tracks[1] = "music/track2.mp3";
    music_tracks[2] = "music/track3.mp3";
    music_tracks[3] = "music/track4.mp3";
    track_count = 4;
}

void play_music() {
    if (sound_enabled && track_count > 0) {
        char command[100];
        sprintf(command, "mpg123 -q %s &", music_tracks[selected_track]);
        system("pkill mpg123"); // Stop previous music
        system(command);
        printw("Playing: %s\n", music_tracks[selected_track]);
    }
}

void open_settings() {
    int choice;
    load_music_tracks();

    while (1) {
        clear();
        printw("\n=== Settings Menu ===\n");
        printw("1. Set Difficulty (Current: %d)\n", difficulty);
        printw("2. Toggle Sound (Current: %s)\n", sound_enabled ? "ON" : "OFF");
        printw("3. Select Music Track (Current: %s)\n", music_tracks[selected_track]);
        printw("4. Back to Main Menu\n");
        printw("Enter your choice: ");
        refresh();

        scanw("%d", &choice);

        switch (choice) {
            case 1: {
                printw("Select Difficulty (1: Easy, 2: Medium, 3: Hard): ");
                refresh();
                int new_difficulty;
                scanw("%d", &new_difficulty);
                if (new_difficulty >= 1 && new_difficulty <= 3) {
                    difficulty = new_difficulty;
                    printw("Difficulty set to %d.\n", difficulty);
                } else {
                    printw("Invalid difficulty level! No changes made.\n");
                }
                break;
            }
            case 2:
                sound_enabled = !sound_enabled;
                if (!sound_enabled) system("pkill mpg123"); // Stop music if disabled
                printw("Sound %s.\n", sound_enabled ? "enabled" : "disabled");
                break;
            case 3:
                printw("\nSelect a music track:\n");
                for (int i = 0; i < track_count; i++) {
                    printw("%d. %s\n", i + 1, music_tracks[i]);
                }
                printw("Enter track number: ");
                refresh();
                int new_track;
                scanw("%d", &new_track);
                if (new_track >= 1 && new_track <= track_count) {
                    selected_track = new_track - 1;
                    play_music();
                } else {
                    printw("Invalid selection!\n");
                }
                break;
            case 4:
                printw("Returning to main menu...\n");
                refresh();
                getch();
                return;
            default:
                printw("Invalid choice! Please try again.\n");
        }
        refresh();
        getch();
    }
}

const char* get_color_name(int color) {
    switch (color) {
        case 1: return "Red";
        case 2: return "Blue";
        case 3: return "Green";
        default: return "Default";
    }
}

