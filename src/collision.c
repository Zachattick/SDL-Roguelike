#include "collision.h"

int check_collision(struct Entity *a, struct Entity *b)
{
    return (a->x_position < b->x_position + b->size &&
            a->x_position + a->size > b->x_position &&
            a->y_position < b->y_position + b->size &&
            a->y_position + a->size > b->y_position);
}