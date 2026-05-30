#ifndef MOVEMENT_H
#define MOVEMENT_H

struct Entity;

struct Vector2D {
    float x;
    float y;
};

void place_entity_at_location(struct Entity* entity, float x, float y);
void move_entity(struct Entity* entity, struct Vector2D direction, float dt);


#endif