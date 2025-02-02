#ifndef SETTINGS_H
#define SETTINGS_H

#define MAX_TRACKS 10

// Declare global variables (NOT define them here)
extern int difficulty;
extern int sound_enabled;
extern char *music_tracks[MAX_TRACKS];
extern int track_count;
extern int selected_track;

// Function declarations
void open_settings();
void load_music_tracks();
void play_music();
const char* get_color_name(int color);

#endif
