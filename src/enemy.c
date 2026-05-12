#include "enemy.h"

void randomly_position_enemy(struct Entity *enemy)
{
    enemy->x_position = rand_int(0, WINDOW_WIDTH - ENEMY_SIZE);
    enemy->y_position = rand_int(0, WINDOW_HEIGHT - ENEMY_SIZE);
}