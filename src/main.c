#include <stdio.h>
#include <SDL2/SDL.h>

int main(void)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    

    SDL_Event event;
    int running = 1;
    
    SDL_Rect player = { 100, 100, 50, 50 };
    
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
                    case SDLK_w: player.y -= 5; break;
                    case SDLK_s: player.y += 5; break;
                    case SDLK_a: player.x -= 5; break;
                    case SDLK_d: player.x += 5; break;
                }
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderClear(renderer);
        
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &player);

        
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}