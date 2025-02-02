#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#define MAX_PLAYERS 100

typedef struct {
    char name[50];
    int rank;
    int gold;
    int plays;
    int experience;
} ScoreEntry;

// 🔥 Declare these globally so other files can access them
extern ScoreEntry scoreboard_data[MAX_PLAYERS];  
extern int player_count;

void update_scoreboard(const char *username, int points_gained);  // ✅ Updated argument
void display_scoreboard();
void show_scoreboard();  // Keep this name to avoid conflicts

#endif
