#include "enemy.h"

void move_enemy_towards_player(struct Entity *enemy, struct Entity *player, float dt)
{
    float dx = (player->x_position + (player->size/2)) - (enemy->x_position + (enemy->size/2));
    float dy = (player->y_position + (player->size/2)) - (enemy->y_position + (enemy->size/2));
    printf("Player x: %.2f, Player y: %.2f\n", player->x_position, player->y_position);
    printf("dx: %.2f, dy: %.2f\n", dx, dy);

    float length = sqrtf(dx * dx + dy * dy);

    dx /= length;
    dy /= length;

    move_entity(enemy, dx, dy, dt);
}