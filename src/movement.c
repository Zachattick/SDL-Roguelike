#include "movement.h"

void place_entity_at_location(struct Entity* entity, float x, float y)
{
    entity->x_position = x;
    entity->y_position = y;
}

void move_entity(struct Entity* entity, float dx, float dy, float dt)
{
    float length = sqrtf((dx * dx) + (dy * dy));

    if (length != 0)
    {
        dx /= length;
        dy /= length;
    }

    entity->x_position += dt * dx * entity->movement_speed;
    entity->y_position += dt * dy * entity->movement_speed;
}

float get_distance_to_move_on_x(struct Entity* entity, float dx, float dy, float dt)
{
    float length = sqrtf((dx * dx) + (dy * dy));

    if (length != 0)
    {
        dx /= length;
        dy /= length;
    }

    return dt * dx * entity->movement_speed;
}

float get_distance_to_move_on_y(struct Entity* entity, float dx, float dy, float dt)
{
    float length = sqrtf((dx * dx) + (dy * dy));

    if (length != 0)
    {
        dx /= length;
        dy /= length;
    }

    return dt * dy * entity->movement_speed;   
}