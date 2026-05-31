#ifndef ENTITY_H
#define ENTITY_H

#include <SDL2/SDL.h>
#include "config.h"
#include "utils.h"

struct Entity {
    int alive;
    int moved_last_frame;
    float max_health;
    float health;
    float damage;
    float movement_speed;
    float x_position, y_position;
    float x_velocity, y_velocity;
    float size;
};

void randomly_position_entity(struct Entity *entity);
void render_entity(SDL_Renderer *renderer, struct Entity *entity, SDL_Color color);

#endif