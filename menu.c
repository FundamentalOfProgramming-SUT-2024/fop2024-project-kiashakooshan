#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>
#include <ncurses.h>
#include "scoreboard.h"
#include "save_load.h"
#include "menu.h"

//#include"setting.h"
#define CONTACTS_FILE "contacts.txt"

void add_contact();
void view_contacts();
void search_contact();
void delete_contact();
void new_game_menu();
void resume_game();

typedef struct {
    char name[50];
    int points;
} ContactEntry;




void start_game_menu() {  // Renamed from start_game()
    int choice;
    do {
        clear();
        printw("\n=== Start Game ===\n");
        printw("1. New Game\n");
        printw("2. Resume Game\n");
        printw("3. Back to Main Menu\n");
        printw("Enter your choice: ");
        refresh();

        scanw("%d", &choice);

        switch (choice) {
            case 1:
                new_game_menu();
                break;
            case 2:
                resume_game();
                break;
            case 3:
                return;
            default:
                printw("Invalid choice! Try again.\n");
        }
        refresh();
        getch();
    } while (choice != 3);
}




void start_guest_game() {
    printw("Starting game as guest...\n");
    refresh();
    start_game(0, NULL, NULL);
}

void register_user() {
    char username[50], password[50];

    clear();
    printw("Enter Username: ");
    refresh();
    scanw("%s", username);

    printw("Enter Password: ");
    refresh();
    scanw("%s", password);

    printw("Starting game with save enabled...\n");
    refresh();
    start_game(1, username, password);
}





void new_game_menu() {
    int choice;
    do {
        clear();
        printw("\n=== New Game ===\n");
        printw("1. Enter as Guest\n");
        printw("2. Enter as Contact (Save Progress)\n");
        printw("3. Back to Start Game Menu\n");
        printw("Enter your choice: ");
        refresh();

        scanw("%d", &choice);

        switch (choice) {
            case 1:
                start_guest_game();
                break;
            case 2:
                register_user();
                break;
            case 3:
                return;
            default:
                printw("Invalid choice! Try again.\n");
        }
        refresh();
        getch();
    } while (choice != 3);
}

void resume_game() {
    char name[50], password[50];

    clear();
    printw("Enter Username: ");
    refresh();
    scanw("%s", name);

    printw("Enter Password: ");
    refresh();
    scanw("%s", password);

    if (load_saved_game(name, password)) {
        printw("Game loaded successfully!\n");
    } else {
        printw("Invalid username or password!\n");
    }
    refresh();
    getch();
}

void scoreboard() {
    clear();
    printw("=== Scoreboard ===\n");
    
    show_scoreboard();  // Use the new function name

    refresh();
    getch();
}







int is_valid_email(const char *email) {
    regex_t regex;
    int ret;

    // Compile regular expression
    ret = regcomp(&regex, "^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$", REG_EXTENDED);
    if (ret) {
        return 0;
    }

    // Execute regular expression
    ret = regexec(&regex, email, 0, NULL, 0);
    regfree(&regex);

    if (!ret) {
        return 1;
    } else {
        return 0;
    }
}

int contains_digit(const char *str) {
    while (*str) {
        if (isdigit(*str)) {
            return 1;
        }
        str++;
    }
    return 0;
}

int contains_upper(const char *str) {
    while (*str) {
        if (isupper(*str)) {
            return 1;
        }
        str++;
    }
    return 0;
}

int contains_lower(const char *str) {
    while (*str) {
        if (islower(*str)) {
            return 1;
        }
        str++;
    }
    return 0;
}



int display_menu() {
    clear(); 
    printw("=== ROGUE GAME ===\n");
    printw("1. Start Game\n");
    printw("2. Settings\n");
    printw("3. Contacts Menu\n");
    printw("4. Scoreboard\n");  // NEW OPTION
    printw("5. Exit\n");
    printw("Choose an option: ");
    refresh();

    echo();
    int choice;
    scanw("%d", &choice);
    noecho();
    return choice;
}



void contacts_menu() {
    int choice;
    do {
        clear();
        printw("\n=== Contacts Menu ===\n");
        printw("1. Add Contact\n");
        printw("2. View Contacts\n");
        printw("3. Search Contact\n");
        printw("4. Delete Contact\n");
        printw("5. Back to Main Menu\n");
        printw("Enter your choice: ");
        refresh();

        scanw("%d", &choice);

        switch (choice) {
            case 1:
                add_contact();
                break;
            case 2:
                view_contacts();
                break;
            case 3:
                search_contact();
                break;
            case 4:
                delete_contact();
                break;
            case 5:
                return;
            default:
                printw("Invalid choice! Please try again.\n");
        }
        refresh();
        getch();
    } while (choice != 5);
}


void add_contact() {
    char name[50], email[50], password[50];

    // Check if name already exists
    printf("Enter name: ");
    scanf("%s", name);
    FILE *file = fopen(CONTACTS_FILE, "r");
    if (file != NULL) {
        char existing_name[50], existing_email[50], existing_password[50];
        int existing_points;
        while (fscanf(file, "%s %s %s %d", existing_name, existing_email, existing_password, &existing_points) == 4) {
            if (strcmp(name, existing_name) == 0) {
                fclose(file);
                printf("Error: The name is already used.\n");
                return;
            }
        }
        fclose(file);
    }

    // Validate email format
    printf("Enter email: ");
    scanf("%s", email);
    if (!is_valid_email(email)) {
        printf("Error: Invalid email format! Should be xxx@y.zzz.\n");
        return;
    }

    // Validate password
    printf("Enter password: ");
    scanf("%s", password);
    if (strlen(password) < 7) {
        printf("Error: Password must be at least 7 characters long.\n");
        return;
    }
    if (!contains_digit(password) || !contains_upper(password) || !contains_lower(password)) {
        printf("Error: Password must contain at least one digit, one uppercase letter, and one lowercase letter.\n");
        return;
    }

    // Add contact and set initial points to 0
    file = fopen(CONTACTS_FILE, "a");
    if (file == NULL) {
        printf("Error: Could not open contacts file.\n");
        return;
    }
    fprintf(file, "%s %s %s %d\n", name, email, password, 0);  // 🔥 Added '0' points
    fclose(file);

    printf("Contact added successfully with 0 points.\n");
}

// void view_contacts() {
//     char name[50], email[50], password[50];
//     FILE *file = fopen(CONTACTS_FILE, "r");
//     if (file == NULL) {
//         printf("No contacts found.\n");
//         return;
//     }

//     printf("\n=== Saved Contacts ===\n");
//     while (fscanf(file, "%s %s %s", name, email, password) == 3) {
//         printf("Name: %s, Email: %s\n", name, email);
//     }
//     fclose(file);
// }
// void view_contacts() {
//     char name[50], email[50], password[50];
//     FILE *file = fopen(CONTACTS_FILE, "r");

//     clear();
//     printw("\n=== Saved Contacts ===\n");

//     if (file == NULL) {
//         printw("No contacts found.\n");
//         refresh();
//         getch();
//         return;
//     }

//     while (fscanf(file, "%s %s %s", name, email, password) == 3) {
//         printw("Name: %s, Email: %s\n", name, email);
//     }

//     fclose(file);
//     refresh();
//     getch();  // Wait for user to press a key before returning
// }

int compare_contacts(const void *a, const void *b) {
    ContactEntry *contactA = (ContactEntry *)a;
    ContactEntry *contactB = (ContactEntry *)b;

    if (contactA->points == 0 && contactB->points > 0) return 1;  // Move zero-point contacts to bottom
    if (contactB->points == 0 && contactA->points > 0) return -1; // Keep high scores at the top
    return contactB->points - contactA->points; // Sort by descending points
}

void view_contacts() {
    typedef struct {
        char name[50];
        int points;
    } Contact;

    Contact contacts[MAX_PLAYERS];
    int count = 0;
    
    FILE *file = fopen(CONTACTS_FILE, "r");
    if (file == NULL) {
        printw("No contacts found.\n");
        refresh();
        getch();
        return;
    }

    // Read contacts
    while (fscanf(file, "%s %d", contacts[count].name, &contacts[count].points) == 2) {
        if (contacts[count].points < 0) {
            contacts[count].points = 0; // Ensure no negative points
        }
        count++;
    }
    fclose(file);

    // Sort contacts by points (descending order)
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (contacts[i].points < contacts[j].points) {
                Contact temp = contacts[i];
                contacts[i] = contacts[j];
                contacts[j] = temp;
            }
        }
    }

    // Display sorted contacts
    clear();
    printw("\n=== Sorted Contacts by Points ===\n");
    for (int i = 0; i < count; i++) {
        if (i < 3) {
            attron(A_BOLD | A_UNDERLINE);  // Highlight top 3
        }
        printw("%d. %s - Points: %d\n", i + 1, contacts[i].name, contacts[i].points);
        attroff(A_BOLD | A_UNDERLINE);
    }
    refresh();
    getch(); // Wait for user input
}


void search_contact() {
    char search[50], name[50], email[50], password[50];
    int found = 0;

    printf("Enter name or email to search: ");
    scanf("%s", search);

    FILE *file = fopen(CONTACTS_FILE, "r");
    if (file == NULL) {
        printf("No contacts found.\n");
        return;
    }

    while (fscanf(file, "%s %s %s", name, email, password) == 3) {
        if (strcmp(search, name) == 0 || strcmp(search, email) == 0) {
            printf("Contact Found - Name: %s, Email: %s\n", name, email);
            found = 1;
            break;
        }
    }
    fclose(file);

    if (!found) {
        printf("No matching contact found.\n");
    }
}

void delete_contact() {
    char target[50], name[50], email[50], password[50];
    int found = 0;

    printf("Enter name or email to delete: ");
    scanf("%s", target);

    FILE *file = fopen(CONTACTS_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (file == NULL || temp == NULL) {
        printf("Error: Could not open file.\n");
        return;
    }
    /////che shod inja
    while (fscanf(file, "%s %s %s", name, email, password) == 3) {
        if (strcmp(target, name) != 0 && strcmp(target, email) != 0) {
            fprintf(temp, "%s %s %s\n", name, email, password);
        } else {
            found = 1;
        }
    }
    fclose(file);
    fclose(temp);

    remove(CONTACTS_FILE);
    rename("temp.txt", CONTACTS_FILE);

    if (found) {
        printf("Contact deleted successfully.\n");
    } else {
        printf("No matching contact found.\n");
    }
}
