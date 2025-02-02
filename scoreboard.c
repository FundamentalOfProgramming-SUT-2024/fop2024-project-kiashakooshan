#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "scoreboard.h"

#define CONTACTS_FILE "contacts.txt"

ScoreEntry scoreboard_data[MAX_PLAYERS];  // This must exist only in scoreboard.c
int player_count = 0;


void update_scoreboard(const char *username, int points_gained) {
    FILE *file = fopen(CONTACTS_FILE, "r");
    if (!file) {
        printw("Error: Could not open contacts file.\n");
        refresh();
        return;
    }

    char name[50], email[50], password[50];
    int score;
    FILE *temp = fopen("temp.txt", "w");
    
    while (fscanf(file, "%s %s %s %d", name, email, password, &score) == 4) {
        if (strcmp(name, username) == 0) {
            score += points_gained;  // Increase player's points
        }
        fprintf(temp, "%s %s %s %d\n", name, email, password, score);
    }

    fclose(file);
    fclose(temp);
    
    remove(CONTACTS_FILE);
    rename("temp.txt", CONTACTS_FILE);
    
    printw("Score updated for %s: +%d points.\n", username, points_gained);
    refresh();
}


void display_scoreboard() {
    printf("\n=== SCOREBOARD ===\n");
    for (int i = 0; i < player_count; i++) {
        printf("%d. %s | Gold: %d | Plays: %d | XP: %d\n",
               i + 1, scoreboard_data[i].name, scoreboard_data[i].gold, scoreboard_data[i].plays, scoreboard_data[i].experience);
    }
}

void show_scoreboard() {  // Renamed from `scoreboard()`
    display_scoreboard();
}
