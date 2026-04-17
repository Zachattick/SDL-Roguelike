#include <stdio.h>
#include <SDL2/SDL.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define PLAYER_SIZE 17


struct Player {
    int x, y;
    int x_velocity, y_velocity;
    SDL_Rect rect;
};

struct key_state {
    int w_pressed;
    int a_pressed;
    int s_pressed;
    int d_pressed;
};

int main(void)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    

    SDL_Event event;
    int running = 1;
    
    struct Player player = { 100, 100, 0, 0, {100, 100, PLAYER_SIZE, PLAYER_SIZE} };
    
    struct key_state keys = {0, 0, 0, 0};
    int y_velocity = 0;
    int x_velocity = 0;

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

        x_velocity = 0;
        y_velocity = 0;

        if (keys.w_pressed) y_velocity -= 5;
        if (keys.s_pressed) y_velocity += 5;
        if (keys.a_pressed) x_velocity -= 5;
        if (keys.d_pressed) x_velocity += 5;

        if ((player.x + x_velocity) >= 0 && (player.x + x_velocity) <= WINDOW_WIDTH - PLAYER_SIZE)
            player.x += x_velocity;
        if ((player.y + y_velocity) >= 0 && (player.y + y_velocity) <= WINDOW_HEIGHT - PLAYER_SIZE)
            player.y += y_velocity;

        player.rect.x = player.x;
        player.rect.y = player.y;

        SDL_SetRenderDrawColor(renderer, 125, 125, 125, 255);
        SDL_RenderClear(renderer);
        
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &player.rect);

        
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}