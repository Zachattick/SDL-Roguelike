#include "enemy.h"

struct Entity create_enemy(void)
{
    struct Entity enemy = {
        .alive = 1,
        .health = 3,
        .damage = 1,
        .movement_speed = DEFAULT_ENEMY_SPEED,
        .x_velocity = 0,
        .y_velocity = 0,
        .x_position = 0,
        .y_position = 0,
        .size = ENEMY_SIZE
    };
    return enemy;
}

int spawn_enemy(struct Entity enemies[])
{
    if (get_live_enemies(enemies) >= MAX_ENEMIES) return 0;
    
    int next_slot = get_next_free_enemy_spot(enemies);

    enemies[next_slot] = create_enemy();
    randomly_position_entity(&enemies[next_slot]);

    return 1;
}

void update_enemies(struct Entity enemies[], struct Entity *player, float dt)
{
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (enemies[i].alive == 0) continue;

        move_enemy_towards_player(&enemies[i], player, dt);
    }
}

void move_enemy_towards_player(struct Entity *enemy, struct Entity *player, float dt)
{
    float dx = (player->x_position + (player->size/2)) - (enemy->x_position + (enemy->size/2));
    float dy = (player->y_position + (player->size/2)) - (enemy->y_position + (enemy->size/2));

    float length = sqrtf(dx * dx + dy * dy);

    dx /= length;
    dy /= length;

    move_entity(enemy, dx, dy, dt);
}

int get_live_enemies(struct Entity enemies[])
{
    int count = 0;
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (enemies[i].alive == 1)
        {
            count++;
        } 
    }
    return count;
}

int get_next_free_enemy_spot(struct Entity enemies[])
{
    int count = 0;
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (enemies[i].alive)
        {
            count++;
            continue;
        }    

        if (count != MAX_ENEMIES)
        {
            return count;
        }    
    }
    return -1;
}