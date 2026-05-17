#include "projectile.h"

struct Entity create_projectile(int damage)
{
    struct Entity projectile = {
        .alive = 1,
        .damage = damage,
        .movement_speed = 3.5,
        .x_velocity = 0,
        .y_velocity = 0,
        .x_position = -100,
        .y_position = -100,
        .size = 15
    };
    return projectile;
}

int shoot_projectile(struct Entity projectiles[], struct Entity *player, int dx, int dy)
{ 
    if (get_live_projectiles(projectiles) >= MAX_PROJECTILES) 
    {
        return 1; 
    }
    int next_slot = get_next_free_projectile_spot(projectiles);

    projectiles[next_slot] = create_projectile(player->damage);

    projectiles[next_slot].x_position = (player->x_position + (player->size/2) - projectiles[next_slot].size/2);
    projectiles[next_slot].y_position = (player->y_position + (player->size/2) - projectiles[next_slot].size/2);

    projectiles[next_slot].x_velocity = dx;
    projectiles[next_slot].y_velocity= dy;

    return 0;
}


void update_projectile(struct Entity *projectile)
{   
    projectile->x_position += projectile->x_velocity * projectile->movement_speed;
    projectile->y_position += projectile->y_velocity * projectile->movement_speed;

    if (projectile->x_position + projectile->size < 0 || projectile->x_position > WINDOW_WIDTH + projectile->size)
    {
        projectile->alive = 0;
    }
    if (projectile->y_position + projectile->size < 0 || projectile->y_position > WINDOW_HEIGHT + projectile->size)
    {
        projectile->alive = 0;
    }
}

void update_projectiles(struct Entity projectiles[])
{
    for (int i = 0; i < MAX_PROJECTILES; i++)
    {
        update_projectile(&projectiles[i]);
    }
}

int get_live_projectiles(struct Entity projectiles[])
{
    int count = 0;
    for (int i = 0; i < MAX_PROJECTILES; i++)
    {
        if (projectiles[i].alive == 1)
        {
            count++;
        } 
    }
    return count;
}

int get_next_free_projectile_spot(struct Entity projectiles[])
{
    int count = 0;
    for (int i = 0; i < MAX_PROJECTILES; i++)
    {
        if (projectiles[i].alive)
        {
            count++;
            continue;
        }    

        if (count != MAX_PROJECTILES)
        {
            return count;
        }    
    }
    return 94;
}