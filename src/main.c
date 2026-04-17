#include <stdio.h>
#include <time.h>
#include <SDL2/SDL.h>

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

void randomly_position_enemy(struct Character *enemy)
{
    enemy->rect.x = rand_int(0, WINDOW_WIDTH - PLAYER_SIZE);
    enemy->rect.y = rand_int(0, WINDOW_HEIGHT - PLAYER_SIZE);
}

int main(void)
{
    srand(time(NULL));

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    

    SDL_Event event;
    
    struct Character player = {0, 0, {100, 100, PLAYER_SIZE, PLAYER_SIZE}};
    struct Character enemy = {0, 0, {0, 0, PLAYER_SIZE, PLAYER_SIZE}};
    
    randomly_position_enemy(&enemy);
    
    
    struct key_state keys = {0, 0, 0, 0};
    
    int score = 0;

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

        SDL_SetRenderDrawColor(renderer, 125, 125, 125, 255);
        SDL_RenderClear(renderer);

        if (check_collision(&player, &enemy))
        {
            randomly_position_enemy(&enemy);
            score++;
            printf("Score: %d\n", score);
        }

        // Draw enemy
        render_character(renderer, &enemy, (SDL_Color){255, 0, 0, 255});
        // Draw player
        render_character(renderer, &player, (SDL_Color){15, 255, 25, 255});

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }
    
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}