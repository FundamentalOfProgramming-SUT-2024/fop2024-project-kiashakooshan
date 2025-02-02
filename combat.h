#ifndef COMBAT_H
#define COMBAT_H

typedef struct {
    int health;      // Enemy health
    int damage;      // Enemy attack damage
    int x, y;        // Enemy position
    int is_alive;    // Status of the enemy
} Enemy;

// Combat-related functions
void initialize_enemy(Enemy* enemy, int x, int y, int health, int damage);
void start_combat(Enemy* enemy);
void player_attack(Enemy* enemy);
void enemy_attack(Enemy* enemy);

#endif
