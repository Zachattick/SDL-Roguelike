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

int main(void)
{
    srand(time(NULL));

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    

    SDL_Event event;
    int running = 1;
    
    struct Character player = {0, 0, {100, 100, PLAYER_SIZE, PLAYER_SIZE}};


    int enemy_x = rand_int(0, WINDOW_WIDTH-PLAYER_SIZE);
    int enemy_y = rand_int(0, WINDOW_HEIGHT-PLAYER_SIZE);

    struct Character enemy = {0, 0, {enemy_x, enemy_y, PLAYER_SIZE, PLAYER_SIZE}};
    
    struct key_state keys = {0, 0, 0, 0};

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


        // Draw enemy
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &enemy.rect);

        // Draw player
        SDL_SetRenderDrawColor(renderer, 15, 255, 25, 255);
        SDL_RenderFillRect(renderer, &player.rect);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}