#ifndef ENEMY_H
#define ENEMY_H

#include "config.h"
#include "entity.h"
#include "utils.h"
#include "movement.h"

struct Entity create_enemy(void);
int spawn_enemy(struct Entity enemies[]);
void move_enemy_towards_player(struct Entity *enemy, struct Entity *player, float dt);
void update_enemies(struct Entity enemies[], struct Entity *player, float dt);
int get_live_enemies(struct Entity enemies[]);
int get_next_free_enemy_spot(struct Entity enemies[]);

#endif
