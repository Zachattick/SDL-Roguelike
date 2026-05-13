#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "entity.h"

void move_entity(struct Entity* entity, float dx, float dy, float dt);
float get_distance_to_move_on_x(struct Entity* entity, float dx, float dy, float dt);
float get_distance_to_move_on_y(struct Entity* entity, float dx, float dy, float dt);


#endif