#include <stdio.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "score.h"
#include "entity.h"
#include "utils.h"
#include "input.h"
#include "collision.h"
#include "config.h"
#include "player.h"
#include "enemy.h"
#include "movement.h"
#include "projectile.h"

int main(void)
{
    // Initialization
    srand(time(NULL));
    printf("SDL_Init(): %s\n", SDL_Init(SDL_INIT_VIDEO) == 0 ? "Success" : "Error");
    printf("TTF_Init(): %s\n", TTF_Init() == 0 ? "Success" : "Error");

    // Window and Renderer Setup
    SDL_Color bg_color = {125, 125, 125, 255};

    SDL_Window *window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!window) {
        printf("Error creating window: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        printf("Error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        return 1;
    }
    
    TTF_Font *font = TTF_OpenFont("assets/fonts/Morgenta.ttf", 16);
    if (!font) {
        printf("Error loading font: %s\n", TTF_GetError());
        return 1;
    }

    
    // Game Setup
    struct Entity player = {
        .alive = 1,
        .movement_speed = DEFAULT_PLAYER_SPEED,
        .x_velocity = 0,
        .y_velocity = 0,
        .x_position = 100,
        .y_position = 100,
        .size = PLAYER_SIZE
    };
    struct Entity enemy = {
        .alive = 1,
        .movement_speed = DEFAULT_ENEMY_SPEED,
        .x_velocity = 0,
        .y_velocity = 0,
        .x_position = 0,
        .y_position = 0,
        .size = ENEMY_SIZE
    };

    struct Entity projectiles[MAX_PROJECTILES] = {0};

    randomly_position_entity(&enemy);
    randomly_position_entity(&player);
    
    // Delta time variables
    uint64_t frame_start = SDL_GetPerformanceCounter();
    uint64_t last_frame_time = SDL_GetPerformanceCounter();
    long long frequency = SDL_GetPerformanceFrequency();
    float delta_time = 0;
    
    // Variable Setup
    SDL_Event event;
    struct key_state keys = {0, 0, 0, 0};
    int score = 0;
    
    int running = 1;
    
    // Main game loop
    while (running)
    {   
        frame_start = SDL_GetPerformanceCounter();
        delta_time = (float)(frame_start - last_frame_time) / frequency;
        last_frame_time = frame_start;

        if (delta_time > 0.5f) // Cap delta time to prevent extreme values
            delta_time = 0.5f;

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
                    case SDLK_UP: shoot_projectile(projectiles, &player, 0, -1); break;
                    case SDLK_DOWN: shoot_projectile(projectiles, &player, 0, 1); break;
                    case SDLK_RIGHT: shoot_projectile(projectiles, &player, 1, 0); break;
                    case SDLK_LEFT: shoot_projectile(projectiles, &player, -1, 0); break;
                    case SDLK_p: printf("DEBUG: Projectile count: %d\n", get_live_projectiles(projectiles));
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

        float dx = 0;
        float dy = 0;

        if (keys.w_pressed) dy -= 1;
        if (keys.s_pressed) dy += 1;
        if (keys.a_pressed) dx -= 1;
        if (keys.d_pressed) dx += 1;

        // Move player, If player is inside the window.
        float x_move = get_distance_to_move_on_x(&player, dx, dy, delta_time);
        float y_move = get_distance_to_move_on_y(&player, dx, dy, delta_time);

        // TODO, this works, but player cant fully touch the wall.
        if ((player.x_position + x_move) < 0 || (player.x_position + x_move) > WINDOW_WIDTH - PLAYER_SIZE)
            dx = 0;
        if ((player.y_position + y_move) < 0 || (player.y_position + y_move) > WINDOW_HEIGHT - PLAYER_SIZE)
            dy = 0;

        move_entity(&player, dx, dy, delta_time);


        // Move Projectiles

        update_projectiles(projectiles);

        // Move enemy towards player
        move_enemy_towards_player(&enemy, &player, delta_time);

        // Check collision
        if (check_collision(&player, &enemy))
        {
            printf("Game Over\n Score: %d\n", score);
        }
        
        // Bullet collision
        for (int i = 0; i < MAX_PROJECTILES; i++)
        {
            if (projectiles[i].alive)
            {
                if (check_collision(&projectiles[i], &enemy))
                {
                    randomly_position_entity(&enemy);
                    projectiles[i].alive = 0;
                    score++;
                }
            }
        }
        // Draw everything
        
        SDL_SetRenderDrawColor(renderer, bg_color.r, bg_color.g, bg_color.b, bg_color.a);
        SDL_RenderClear(renderer);
        // Draw enemy
        render_entity(renderer, &enemy, (SDL_Color){255, 0, 0, 255});
        // Draw player
        render_entity(renderer, &player, (SDL_Color){15, 255, 25, 255});
        // Draw projectiles
        for (int i = 0; i < MAX_PROJECTILES; i++)
        {   
            if (projectiles[i].alive == 1)
                render_entity(renderer, &projectiles[i], (SDL_Color){0, 0, 200, 255});
        }
        // Draw score
        render_score(renderer, font, score);

        SDL_RenderPresent(renderer);
    }
    
    TTF_CloseFont(font);
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}