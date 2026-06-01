#ifndef CONFIG_H
#define CONFIG_H

#define DEFAULT_FPS 1
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define PLAYER_SIZE 75

#define DEFAULT_PLAYER_SPEED 300.0
#define DEFAULT_ENEMY_SPEED 75.0

#define DEFAULT_PLAYER_HEALTH 10.0
#define DEFAULT_ENEMY_HEALTH 5.0

#define MAX_ENEMIES 10
#define ENEMY_SIZE 25
#define TIME_UNTIL_ENEMY_CAN_MOVE 1 // SECONDS - Time after spawning before an enemy can move


#define MAX_PROJECTILES 25
#define SHOOTING_COOLDOWN 0.33 // SECONDS

#define PLAYER_IMMUNITY_COOLDOWN 0.25 // SECONDS

#define WAVE_DELAY 2.5 // SECONDS - Time after a wave is cleared before the next wave spawns


#define DEBUG_COOLDOWN 1.0 // SECONDS - Time between debug prints

#endif