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

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    // Window and Renderer Setup
    SDL_Color bg_color = {33, 33, 33, 255};

    SDL_Window *window = SDL_CreateWindow("The Binding Of Izach", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
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
        .health = DEFAULT_PLAYER_HEALTH,
        .damage = 1,
        .movement_speed = DEFAULT_PLAYER_SPEED,
        .x_velocity = 0,
        .y_velocity = 0,
        .x_position = 100,
        .y_position = 100,
        .size = PLAYER_SIZE
    };

    struct Entity enemies[MAX_ENEMIES] = {0};
    struct Entity projectiles[MAX_PROJECTILES] = {0};

    randomly_position_entity(&player);
    
    // Delta time variables
    uint64_t frame_start = SDL_GetPerformanceCounter();
    uint64_t last_frame_time = SDL_GetPerformanceCounter();
    long long frequency = SDL_GetPerformanceFrequency();
    float delta_time = 0;
    
    // Variable Setup
    SDL_Event event;
    struct key_state keys = {0, 0, 0, 0, 0, 0, 0, 0};
    int score = 0;
    float shooting_cooldown = 0;
    float player_immunity_cooldown = 0;
    int running = 1;
    int current_wave = 1;
    float next_wave_timer = WAVE_DELAY;
    
    // Debug Cooldown 
    float debug_cooldown = DEBUG_COOLDOWN;

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
                    case SDLK_p: spawn_enemy(enemies); break;
                    case SDLK_w: keys.w_pressed = 1; break;
                    case SDLK_s: keys.s_pressed = 1; break;
                    case SDLK_a: keys.a_pressed = 1; break;
                    case SDLK_d: keys.d_pressed = 1; break;
                    case SDLK_UP: keys.up_pressed = 1; break;
                    case SDLK_DOWN: keys.down_pressed = 1; break;
                    case SDLK_LEFT: keys.left_pressed = 1; break;
                    case SDLK_RIGHT: keys.right_pressed = 1; break;
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
                    case SDLK_UP: keys.up_pressed = 0; break;
                    case SDLK_DOWN: keys.down_pressed = 0; break;
                    case SDLK_LEFT: keys.left_pressed = 0; break;
                    case SDLK_RIGHT: keys.right_pressed = 0; break;
                }
            }   
        }

    // Enemy spawning logic

        // lower wave timer
        if (next_wave_timer > 0)
            next_wave_timer -= delta_time;

        if (next_wave_timer <= 0)
        {
            int all_enemies_dead = 1;
            for (int i = 0; i < MAX_ENEMIES; i++)
            {
                if (enemies[i].alive == 1)
                {
                    all_enemies_dead = 0;
                    break;
                }
            }

            if (all_enemies_dead)
            {
                current_wave++;
                next_wave_timer = WAVE_DELAY;

                // Spawn new wave of enemies
                for (int i = 0; i < current_wave && i < MAX_ENEMIES; i++)
                {
                    spawn_enemy(enemies);
                }
            }
        } 

        // Shooting // TODO, If holding multiple keys, one always takes priority.
        if (shooting_cooldown <= 0)
        {
            if (keys.up_pressed) {shoot_projectile(projectiles, &player, 0, -1); shooting_cooldown = SHOOTING_COOLDOWN;}
            else if (keys.down_pressed) {shoot_projectile(projectiles, &player, 0, 1); shooting_cooldown = SHOOTING_COOLDOWN;}
            else if (keys.left_pressed) {shoot_projectile(projectiles, &player, -1, 0); shooting_cooldown = SHOOTING_COOLDOWN;}
            else if (keys.right_pressed) {shoot_projectile(projectiles, &player, 1, 0); shooting_cooldown = SHOOTING_COOLDOWN;}
        }    

    // Player movement logic
        struct Vector2D direction = {0, 0};

        if (keys.w_pressed) direction.y -= 1;
        if (keys.s_pressed) direction.y += 1;
        if (keys.a_pressed) direction.x -= 1;
        if (keys.d_pressed) direction.x += 1;

        direction = normalize_vector(direction);
        move_entity(&player, direction, delta_time);
        
        // If player is out of bounds, move them back in bounds. // This feels like a hacky solution, but it works for now. TODO: Decide if I want to implement a better solution for this.
        if (player.x_position < 0) player.x_position = 0;
        if (player.x_position > WINDOW_WIDTH - PLAYER_SIZE) player.x_position = WINDOW_WIDTH - PLAYER_SIZE;
        if (player.y_position < 0) player.y_position = 0;
        if (player.y_position > WINDOW_HEIGHT - PLAYER_SIZE) player.y_position = WINDOW_HEIGHT - PLAYER_SIZE;

    // Move Projectiles
        update_projectiles(projectiles);

    // Move enemies towards player
        update_enemies(enemies, &player, delta_time);

    // Collision detection
        // Player-Enemy collision.
        for (int i = 0; i < MAX_ENEMIES; i++)
        {   
            struct Entity* enemyp = &enemies[i];

            if (enemyp->alive == 0) continue;

            if (check_collision(&player, enemyp))
            {   
                if (player_immunity_cooldown == 0)
                {   
                    player_immunity_cooldown = PLAYER_IMMUNITY_COOLDOWN;
                    player.health -= enemyp->damage * (1 + (0.1 * rand_int(0, 5))); // 1-1.5x damage multiplier
                    if (player.health <= 0)
                    {
                        printf("Game Over\nScore: %d\n", score);
                        
                        destroy_all_enemies(enemies);
                        destroy_all_projectiles(projectiles);

                        score = 0;
                        next_wave_timer = WAVE_DELAY;
                        current_wave = 1;

                        randomly_position_entity(&player);
                        player.health = DEFAULT_PLAYER_HEALTH;
                    }
                }
            }
        }
        // Bullet/Enemy collision
        for (int e = 0; e < MAX_ENEMIES; e++)
        {
            if (enemies[e].alive == 0) continue;

            for (int i = 0; i < MAX_PROJECTILES; i++)
            {
                if (projectiles[i].alive == 0) continue;

                if (check_collision(&projectiles[i], &enemies[e]))
                {
                    projectiles[i].alive = 0;
                    enemies[e].health -= projectiles[i].damage;
                    if (enemies[e].health <= 0)
                    {
                        enemies[e].alive = 0;
                        score++;
                        
                        // If the last enemy is dies, set the wave timer to WAVE_DELAY
                        if (get_live_enemies(enemies) == 0)
                            next_wave_timer = WAVE_DELAY;
                    }
                }
            }
        }

    // Cooldown management
        // Lower player immunity cooldown
        if (player_immunity_cooldown > 0)
        {
            player_immunity_cooldown -= delta_time;
            if (player_immunity_cooldown < 0) player_immunity_cooldown = 0;
        }
        // Lower shooting cooldown
        if (shooting_cooldown > 0)
        {
            shooting_cooldown -= delta_time;
            if (shooting_cooldown < 0) shooting_cooldown = 0;
        }

        // Print debug info
        if (debug_cooldown > 0)
            debug_cooldown -= delta_time;
        else
        {
            debug_cooldown = DEBUG_COOLDOWN;
            printf("Wave: %d | Next Wave In: %.2f seconds | Live Enemies: %d | Player Health: %.2f | Score: %d\n", current_wave, next_wave_timer, get_live_enemies(enemies), player.health, score);
        }


        // Draw everything        
        SDL_SetRenderDrawColor(renderer, bg_color.r, bg_color.g, bg_color.b, bg_color.a);
        SDL_RenderClear(renderer);
        // Draw enemies
        for (int i = 0; i < MAX_ENEMIES; i++)
        {
            if (enemies[i].alive == 0) continue;

            render_entity(renderer, &enemies[i], (SDL_Color){64 + enemies[i].health/enemies[i].max_health * 128, 0, enemies[i].health/enemies[i].max_health * 64, 255});
        }
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