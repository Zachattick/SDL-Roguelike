#include <stdio.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define DEFAULT_FPS 60
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define PLAYER_SIZE 30
#define ENEMY_SIZE 25

#define DEFAULT_PLAYER_SPEED 2.3
#define DEFAULT_ENEMY_SPEED 0.8

struct Character {
    float movement_speed;
    float x_position, y_position;
    float x_velocity, y_velocity;
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
    return (a->x_position < b->x_position + b->rect.w &&
            a->x_position + a->rect.w > b->x_position &&
            a->y_position < b->y_position + b->rect.h &&
            a->y_position + a->rect.h > b->y_position);
}

void render_character(SDL_Renderer *renderer, struct Character *character, SDL_Color color)
{
    character->rect.x = character->x_position;
    character->rect.y = character->y_position;

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &character->rect);
}

void render_score(SDL_Renderer *renderer, TTF_Font *font, int score)
{
    char score_int_string[20];
    char score_string[20] = "Score";
    snprintf(score_int_string, sizeof(score_int_string), "%d", score);

    SDL_Surface *score_string_surface = TTF_RenderText_Solid(font, score_string, (SDL_Color){0, 0, 0, 0});
    if (score_string_surface == NULL) {
        printf("Error creating text surface: %s\n", TTF_GetError());
        return;
    }
    SDL_Surface *score_int_surface = TTF_RenderText_Solid(font, score_int_string, (SDL_Color){0, 0, 0, 0});
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


void randomly_position_enemy(struct Character *enemy)
{
    enemy->x_position = rand_int(0, WINDOW_WIDTH - ENEMY_SIZE);
    enemy->y_position = rand_int(0, WINDOW_HEIGHT - ENEMY_SIZE);
}

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
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
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
    struct Character player = {
        .movement_speed = DEFAULT_PLAYER_SPEED,
        .x_velocity = 0,
        .y_velocity = 0,
        .x_position = 100,
        .y_position = 100,
        .rect = {
            .x = player.x_position,
            .y = player.y_position,
            .w = PLAYER_SIZE,
            .h = PLAYER_SIZE
        }
    };
    struct Character enemy = {
        .movement_speed = DEFAULT_ENEMY_SPEED,
        .x_velocity = 0,
        .y_velocity = 0,
        .x_position = 0,
        .y_position = 0,
        .rect = {
            .x = enemy.x_position,
            .y = enemy.y_position,
            .w = ENEMY_SIZE,
            .h = ENEMY_SIZE
        }
    };

    SDL_Event event;
    randomly_position_enemy(&enemy);

    // Varibale Setup
    struct key_state keys = {0, 0, 0, 0};
    int score = 0;
    
    int automatic = 0;
    int running = 1;

    // Main game loop
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
                    case SDLK_COMMA: automatic = 1; printf("Automatic mode enabled\n"); break;
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
        enemy.x_velocity = 0;
        enemy.y_velocity = 0;

        float dx = 0;
        float dy = 0;

        if (keys.w_pressed) dy -= 1;
        if (keys.s_pressed) dy += 1;
        if (keys.a_pressed) dx -= 1;
        if (keys.d_pressed) dx += 1;

        float length = sqrtf((dx * dx) + (dy * dy));

        if (length != 0)
        {
            dx /= length;
            dy /= length;
        }

        // printf("%f\n", dx);
        // printf("%f\n", dy);

        player.x_velocity = dx * player.movement_speed;
        player.y_velocity = dy * player.movement_speed;

        // Move player, If player is inside the window.
        if ((player.x_position + player.x_velocity) >= 0 && (player.x_position + player.x_velocity) <= WINDOW_WIDTH - PLAYER_SIZE)
            player.x_position += player.x_velocity; printf("Player: x_velocity: %.2f\n", player.x_velocity);
        if ((player.y_position + player.y_velocity) >= 0 && (player.y_position + player.y_velocity) <= WINDOW_HEIGHT - PLAYER_SIZE)
            player.y_position += player.y_velocity; printf("Player: y_velocity: %.2f\n", player.y_velocity);
        
        if (automatic) // Vacuum enemy towards player
        {
            float distance_x = (enemy.x_position + ENEMY_SIZE/2) - (player.x_position + PLAYER_SIZE/2);
            float distance_y = (enemy.y_position + ENEMY_SIZE/2) - (player.y_position + PLAYER_SIZE/2);
            
            if (distance_x > 0) // Enemy is to the right of player, move left
            {
                enemy.x_velocity -= (distance_x < enemy.movement_speed) ? distance_x : enemy.movement_speed;
            }
            else if (distance_x < 0) // Enemy is to the left of player, move right
            {
                enemy.x_velocity += (distance_x > enemy.movement_speed) ? distance_x : enemy.movement_speed;
            }

            if (distance_y > 0) // Enemy is below player, move up
            {
                enemy.y_velocity -= (distance_y < enemy.movement_speed) ? distance_y : enemy.movement_speed;
            }
            else if (distance_y < 0) // Enemy is above player, move down
            {
                enemy.y_velocity += (distance_y > enemy.movement_speed) ? distance_y : enemy.movement_speed;
            }

            // Move enemy
            enemy.x_position += enemy.x_velocity; printf("Enemy: x_velocity: %.2f\n", enemy.x_velocity);
            enemy.y_position += enemy.y_velocity; printf("Enemy: y_velocity: %.2f\n", enemy.y_velocity);
        }


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

        SDL_Delay(16); // ~60 FPS
    }
    
    TTF_CloseFont(font);
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}