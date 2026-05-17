#include "score.h"
#include "config.h"

void render_score(SDL_Renderer *renderer, TTF_Font *font, int score)
{
    char score_int_string[20];
    char score_string[20] = "Score";
    snprintf(score_int_string, sizeof(score_int_string), "%d", score);

    SDL_Surface *score_string_surface = TTF_RenderText_Solid(font, score_string, (SDL_Color){0, 99, 0, 0});
    if (score_string_surface == NULL) {
        printf("Error creating text surface: %s\n", TTF_GetError());
        return;
    }
    SDL_Surface *score_int_surface = TTF_RenderText_Solid(font, score_int_string, (SDL_Color){0, 120, 0, 0});
    if (score_int_surface == NULL) {
        printf("Error creating text surface: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, score_string_surface);
    if (text_texture == NULL) {
        printf("Error creating text texture: %s\n", SDL_GetError());
        SDL_FreeSurface(score_string_surface);
        return;
    }
    SDL_Texture *int_texture = SDL_CreateTextureFromSurface(renderer, score_int_surface);
    if (int_texture == NULL) {
        printf("Error creating text texture: %s\n", SDL_GetError());
        SDL_FreeSurface(score_int_surface);
        return;
    }

    SDL_FreeSurface(score_string_surface);
    SDL_FreeSurface(score_int_surface);

    int tw, th;
    int iw, ih;
    SDL_QueryTexture(text_texture, NULL, NULL, &tw, &th);
    SDL_QueryTexture(int_texture, NULL, NULL, &iw, &ih);

    SDL_Rect text_rect = {(WINDOW_WIDTH/2) - (tw/2), 25, tw+10, th+10};
    SDL_Rect int_rect = {(WINDOW_WIDTH/2) - (iw/2), text_rect.y + text_rect.h + 5, iw+10, ih+10};
    if (SDL_RenderCopy(renderer, text_texture, NULL, &text_rect) != 0)
    {   
        SDL_Log("SDL_RenderCopy Error: %s", SDL_GetError());
    }
    if (SDL_RenderCopy(renderer, int_texture, NULL, &int_rect) != 0)
    {   
        SDL_Log("SDL_RenderCopy Error: %s", SDL_GetError());
    }
    SDL_DestroyTexture(text_texture);
    SDL_DestroyTexture(int_texture);
}