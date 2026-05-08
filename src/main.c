#include <stdio.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define PLAYER_SIZE 25


struct Character {
    int x_velocity, y_velocity;
    SDL_Rect rect;
};

struct key_state {
    int w_pressed;
    int a_pressed;
    int s_pressed;
    int d_pressed;
};

int rand_int(int min, int max)
{
    return (rand() % (max - min + 1)) + min;
}

int check_collision(struct Character *a, struct Character *b)
{
    return (a->rect.x < b->rect.x + b->rect.w &&
            a->rect.x + a->rect.w > b->rect.x &&
            a->rect.y < b->rect.y + b->rect.h &&
            a->rect.y + a->rect.h > b->rect.y);
}

void render_character(SDL_Renderer *renderer, struct Character *character, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &character->rect);
}

void render_score(SDL_Renderer *renderer, TTF_Font *font, int score)
{
    char score_text[20];
    snprintf(score_text, sizeof(score_text), "Score: %d", score);
    SDL_Surface *text_surface = TTF_RenderText_Blended(font, score_text, (SDL_Color){0, 0, 0, 0});
    if (!text_surface) {
        printf("Error creating text surface: %s\n", TTF_GetError());
        return;
    }
    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    if (!text_texture) {
        printf("Error creating text texture: %s\n", SDL_GetError());
        SDL_FreeSurface(text_surface);
        return;
    }
    SDL_FreeSurface(text_surface);

    SDL_Rect text_rect = {25, 25, 100, 25};
    if (SDL_RenderCopy(renderer, text_texture, NULL, &text_rect) != 0)
    {   
        SDL_Log("SDL_RenderCopy Error: %s", SDL_GetError());
    }
    SDL_DestroyTexture(text_texture);
}

void randomly_position_enemy(struct Character *enemy)
{
    enemy->rect.x = rand_int(0, WINDOW_WIDTH - PLAYER_SIZE);
    enemy->rect.y = rand_int(0, WINDOW_HEIGHT - PLAYER_SIZE);
}


int main(void)
{
    srand(time(NULL));

    SDL_Color bg_color = {125, 125, 125, 255};
    int score = 0;

    printf("SDL_Init(): %s\n", SDL_Init(SDL_INIT_VIDEO) == 0 ? "Success" : "Error");
    printf("TTF_Init(): %s\n", TTF_Init() == 0 ? "Success" : "Error");

    SDL_Window *window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!window) {
        printf("Error creating window: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        printf("Error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        return 1;
    }
    
    TTF_Font *font = TTF_OpenFont("assets/fonts/Morgenta.ttf", 24);
    if (!font) {
        printf("Error loading font: %s\n", TTF_GetError());
        return 1;
    }

    SDL_Event event;
    

    struct Character player = {0, 0, {100, 100, PLAYER_SIZE, PLAYER_SIZE}};
    struct Character enemy = {0, 0, {0, 0, PLAYER_SIZE, PLAYER_SIZE}};
    
    randomly_position_enemy(&enemy);
    
    
    struct key_state keys = {0, 0, 0, 0};
    

    int running = 1;
    while (running)
    {   

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = 0;
            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE: running = 0; break;
                    case SDLK_w: keys.w_pressed = 1; break;
                    case SDLK_s: keys.s_pressed = 1; break;
                    case SDLK_a: keys.a_pressed = 1; break;
                    case SDLK_d: keys.d_pressed = 1; break;
                }
            }
            else if (event.type == SDL_KEYUP)
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_w: keys.w_pressed = 0; break;
                    case SDLK_s: keys.s_pressed = 0; break;
                    case SDLK_a: keys.a_pressed = 0; break;
                    case SDLK_d: keys.d_pressed = 0; break;
                }
            }   
        }

        // Player movement logic

        // Move player
        player.x_velocity = 0;
        player.y_velocity = 0;

        if (keys.w_pressed) player.y_velocity -= 5;
        if (keys.s_pressed) player.y_velocity += 5;
        if (keys.a_pressed) player.x_velocity -= 5;
        if (keys.d_pressed) player.x_velocity += 5;

        if ((player.rect.x + player.x_velocity) >= 0 && (player.rect.x + player.x_velocity) <= WINDOW_WIDTH - PLAYER_SIZE)
            player.rect.x += player.x_velocity;
        if ((player.rect.y + player.y_velocity) >= 0 && (player.rect.y + player.y_velocity) <= WINDOW_HEIGHT - PLAYER_SIZE)
            player.rect.y += player.y_velocity;

        // Check collision
        if (check_collision(&player, &enemy))
        {
            randomly_position_enemy(&enemy);
            score++;
            printf("Score: %d\n", score);
        }
    
        // Draw everything
        
    
        SDL_SetRenderDrawColor(renderer, bg_color.r, bg_color.g, bg_color.b, bg_color.a);
        SDL_RenderClear(renderer);
        // Draw enemy
        render_character(renderer, &enemy, (SDL_Color){255, 0, 0, 255});
        // Draw player
        render_character(renderer, &player, (SDL_Color){15, 255, 25, 255});
        // Draw score
        render_score(renderer, font, score);

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }
    
    TTF_CloseFont(font);
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}