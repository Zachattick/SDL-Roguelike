#ifndef ENTITY_H
#define ENTITY_H

#include <SDL2/SDL.h>

struct Entity {
    float movement_speed;
    float x_position, y_position;
    float x_velocity, y_velocity;
    float size;
};


void render_entity(SDL_Renderer *renderer, struct Entity *entity, SDL_Color color);

#endif