#pragma once

namespace Config
{
	static const unsigned NUM_MAP_TILE_ROWS = 21;
	static const unsigned NUM_MAP_TILE_COLS = 21;
	static const unsigned MAP_TILE_SIZE_IN_PIXELS = 32;
	static const float MAP_TILE_WORLD_SIZE = 32.0f;
	static const float MAP_TILE_WORLD_HALF_SIZE = 0.5f * MAP_TILE_WORLD_SIZE;
	
	static const unsigned MAP_WIDTH = NUM_MAP_TILE_COLS * MAP_TILE_SIZE_IN_PIXELS;
	static const unsigned MAP_HEIGHT = NUM_MAP_TILE_ROWS * MAP_TILE_SIZE_IN_PIXELS;

	static const unsigned SCORE_PANEL_WIDTH = MAP_WIDTH;
	static const unsigned SCORE_PANEL_HEIGHT = MAP_TILE_SIZE_IN_PIXELS;

	static const unsigned WINDOW_WIDTH = MAP_WIDTH;
	static const unsigned WINDOW_HEIGHT = MAP_HEIGHT + SCORE_PANEL_HEIGHT;

	static const float PACMAN_SPEED = 0.07f;
	static const unsigned PACMAN_LIVES = 3;

	static const float BLINKY_SPEED = 0.05f;
	static const float PINKY_SPEED = 0.05f;

	static const unsigned PILL_POINT_SCORE = 10;
	static const unsigned POWER_UP_SCORE = 25;
	static const unsigned KILLING_GHOST_SCORE = 100;
	static const int POWER_UP_DURATION_IN_SEC = 10;
	static const int PATH_VALIDATION_TIMEOUT_IN_SEC = 2;
}