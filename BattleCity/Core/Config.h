#pragma once

struct Config
{
    enum
    {
        TILE_NUM_COLS = 26,
        TILE_NUM_ROWS = 26,
        TILE_SCREEN_SIZE = 32,
        TILE_SCREEN_HALF_SIZE = TILE_SCREEN_SIZE >> 1,

        SCREEN_WIDTH = TILE_NUM_COLS * TILE_SCREEN_SIZE,
        SCREEN_HEIGHT = TILE_NUM_ROWS * TILE_SCREEN_SIZE,

        MISSILE_HEALTH_DAMAGE = 1,
        MISSILE_FULL_HEALTH = 1,
        MISSILE_SPEED = 4,
        MISSILE_HALF_SIZE = 4,

        ENEMY_TANK_FULL_HEALTH = 1,
        ENEMY_TANK_SPEED = 1,
        ENEMY_TANK_HALF_SIZE = TILE_SCREEN_SIZE - 4,

        PLAYER_TANK_FULL_HEALTH = 3,
        PLAYER_TANK_SPEED = 1,
        PLAYER_TANK_HALF_SIZE = TILE_SCREEN_SIZE - 4,

        BRICK_WALL_FULL_HEALTH = 1,
        STEEL_WALL_FULL_HEALTH = 1,
        PLAYER_BASE_FULL_HEALTH = 1,

        MAX_NUM_ENEMY_TANKS = 8,
        MAX_NUM_ACTIVE_ENEMY_TANKS = 2,

        TANK_WEAPON_RELOAD_IN_MS = 5000,
        TANK_PATH_VALIDATION_IN_MS = 7000,
        ENEMY_TANK_RESPAWN_IN_MS = 5000,
        UPDATE_DELAY_IN_MS = 16,
    };
};
