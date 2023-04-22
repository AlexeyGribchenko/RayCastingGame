#pragma once
#include <cmath>

// Window
const float WIDTH = 1920;
const float HEIGHT = 1080;
const float FPS = 60;

// RayCasting
const float NUM_RAYS = 1920;
const float H_FOV = 0.55;
const float DISTANCE_TO_DISPLAY = NUM_RAYS / tan(2  * H_FOV);
const float DELTA_ANGLE = H_FOV * 2 / NUM_RAYS;
const float COLOR_FACTOR_RATIO = 0.000001;
const float TILE_W = WIDTH / NUM_RAYS;
const float TILE_H = 100;
const float MAX_RAY_LENGHT = 1500;

// Textures

// GameField
const float SQUARE_TILE = 100;
const float MAP_SQUARE_TILE = 10;

const float FIRST_LEVEL_SIZE = 13;
const float SECOND_LEVEL_SIZE = 17;
const float THIRD_LEVEL_SIZE = 25;

// Player
const float MAX_PLAYER_SPEED = 2;
const float MAX_SHIFT_PLAYER_SPEED = 3;
const float PLAYER_RADIUS = 3;
const float PLAYER_START_ANGLE = 0;
const float PLAYER_START_SPEED = 4;
const float PLAYER_START_VIEW_VER_POS = 0;
const float PLAYER_START_COORD_X = (FIRST_LEVEL_SIZE / 2 + 1);
const float PLAYER_START_COORD_Y = (FIRST_LEVEL_SIZE / 2 + 1);
