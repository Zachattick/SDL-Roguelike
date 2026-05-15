#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "entity.h"

int shoot_projectile(struct Entity projectiles[], struct Entity *player, int dx, int dy);
void update_projectile(struct Entity *projectile);
void update_projectiles(struct Entity projectiles[]);
int get_live_projectiles(struct Entity projectiles[]);
int get_next_free_projectile_spot(struct Entity projectiles[]);

#endif