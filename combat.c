#include <stdio.h>
#include "combat.h"
#include "player.h"


extern Player player;  // Access player globally
// int player_x = player.x;
// int player_y = player.y;

// Initialize an enemy with position, health, and damage
void initialize_enemy(Enemy* enemy, int x, int y, int health, int damage) {
    enemy->x = x;
    enemy->y = y;
    enemy->health = health;
    enemy->damage = damage;
    enemy->is_alive = 1;
}

// Player attacks an enemy
void player_attack(Enemy* enemy) {
    if (enemy->is_alive) {
        enemy->health -= 10;  // Example: Player deals fixed 10 damage
        printf("You attacked the enemy! Enemy health: %d\n", enemy->health);

        if (enemy->health <= 0) {
            enemy->is_alive = 0;
            printf("You killed the enemy!\n");
        }
    }
}

// Enemy attacks the player
void enemy_attack(Enemy* enemy) {
    if (enemy->is_alive) {
        player.health -= enemy->damage;  // Reduce player's health
        printf("Enemy attacked you! Your health: %d\n", player.health);

        if (player.health <= 0) {
            printf("You have died! Game over.\n");
        }
    }
}

// Start combat with an enemy
void start_combat(Enemy* enemy) {
    printf("\n=== Combat Started! ===\n");
    printf("Enemy Position: (%d, %d), Health: %d, Damage: %d\n",
           enemy->x, enemy->y, enemy->health, enemy->damage);

    while (player.health > 0 && enemy->is_alive) {
        printf("Press 'a' to attack or 'r' to run: ");
        int ch = getchar();

        if (ch == 'a') {
            player_attack(enemy);
            if (enemy->is_alive) {
                enemy_attack(enemy);
            }
        } else if (ch == 'r') {
            printf("You ran away from the enemy!\n");
            break;
        }

        if (player.health <= 0) {
            printf("You have been defeated.\n");
            break;
        }
    }
}
