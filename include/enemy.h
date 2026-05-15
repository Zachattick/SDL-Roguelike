#ifndef ENEMY_H
#define ENEMY_H

#include "config.h"
#include "entity.h"
#include "utils.h"
#include "movement.h"

void move_enemy_towards_player(struct Entity *enemy, struct Entity *player, float dt);

#endif
