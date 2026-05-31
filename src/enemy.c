#include "enemy.h"

struct Entity create_enemy(void)
{
    struct Entity enemy = {
        .alive = 1,
        .max_health = 7,
        .health = enemy.max_health,
        .damage = 1,
        .movement_speed = DEFAULT_ENEMY_SPEED * ((rand_int(1,5) * 0.2) + 0.8),
        .time_until_can_move = TIME_UNTIL_ENEMY_CAN_MOVE,
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

        if (enemies[i].time_until_can_move > 0)
        {
            enemies[i].time_until_can_move -= dt;
            if (enemies[i].time_until_can_move < 0) enemies[i].time_until_can_move = 0;
        }

        if (enemies[i].time_until_can_move > 0) continue;

        move_enemy_towards_player(&enemies[i], player, dt);
    }
}

void move_enemy_towards_player(struct Entity *enemy, struct Entity *player, float dt)
{   
    struct Vector2D normalized_direction = get_enemy_move(enemy, player);
    move_entity(enemy, normalized_direction, dt);
}

void destroy_enemy(struct Entity *enemy)
{
    enemy->alive = 0;
}

void destroy_all_enemies(struct Entity enemies[])
{
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        destroy_enemy(&enemies[i]);
    }
}

struct Vector2D get_enemy_move(struct Entity *enemy, struct Entity *player)
{
    struct Vector2D direction = {
        .x = (player->x_position + (player->size / 2)) - (enemy->x_position + (enemy->size / 2)),
        .y = (player->y_position + (player->size / 2)) - (enemy->y_position + (enemy->size / 2))
    };

    return normalize_vector(direction);
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