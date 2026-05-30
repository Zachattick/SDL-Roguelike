#include "movement.h"
#include "entity.h"
#include "utils.h"

void place_entity_at_location(struct Entity* entity, float x, float y)
{
    entity->x_position = x;
    entity->y_position = y;
}

void move_entity(struct Entity* entity, struct Vector2D direction, float dt)
{
    // DOES NOT normalize the direction vector, it should be normalized before calling this function. 
    // Allows for wider use of this function, as it can be used for any direction vector, normalized or not.

    entity->x_position += dt * direction.x * entity->movement_speed;
    entity->y_position += dt * direction.y * entity->movement_speed;
}