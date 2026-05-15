#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "entity.h"

void shoot_projectile(struct Entity *player, struct Entity *projectile, int dx, int dy);
void update_projectile(struct Entity *projectile);

#endif