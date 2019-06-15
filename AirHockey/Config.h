#pragma once

const float PLAY_TIME = 3600.0f;
const int UI_AREA_HEIGHT = 50;

const int BOARD_WIDTH = 700;
const int BOARD_HEIGHT = 400;
const float BOARD_FRICTION = 0.0001f;
const float BOARD_WALL_RESTITUTION = 0.9f;

const int GOAL_TOP_OFFSET = 92;
const int GOAL_BOTTOM_OFFSET = 308;
const float GOAL_RADIUS = 0.5f * float(GOAL_BOTTOM_OFFSET - GOAL_TOP_OFFSET);

const float STRIKER_RADIUS = 23.0f;
const float STRIKER_SPEED = 80.0f;

const float PUCK_RADIUS = 16.0f;
const float PUCK_SPEED = 360.0f;