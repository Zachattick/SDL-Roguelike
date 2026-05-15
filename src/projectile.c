#include "projectile.h"

void shoot_projectile(struct Entity *player, struct Entity *projectile, int dx, int dy)
{

    projectile->x_position = (player->x_position + (player->size/2) - projectile->size/2);
    projectile->y_position = (player->y_position + (player->size/2) - projectile->size/2);

    projectile->x_velocity = dx;
    projectile->y_velocity = dy;
}

void update_projectile(struct Entity *projectile)
{
    projectile->x_position += projectile->x_velocity * projectile->movement_speed;
    projectile->y_position += projectile->y_velocity * projectile->movement_speed;
}