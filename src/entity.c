#include "entity.h"

void render_entity(SDL_Renderer *renderer, struct Entity *entity, SDL_Color color)
{   
    SDL_Rect rect = {
        .x = entity->x_position,
        .y = entity->y_position,
        .w = entity->size,
        .h = entity->size
    };

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}