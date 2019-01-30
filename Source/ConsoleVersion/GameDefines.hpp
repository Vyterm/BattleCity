#ifndef GAME_LIB_HPP_INCLUDED
#define GAME_LIB_HPP_INCLUDED

#include "GameColor.hpp"
#include "GameMath.hpp"

constexpr auto GAME_NAME = "坦克大战(Console Version) by 郭弈天";
constexpr auto GAME_VERSION = "Version 0.1.9";

constexpr auto WIN_HEIGHT = 43;
constexpr auto WIN_WIDTH = 120;
constexpr auto GAME_HEIGHT = 40;
constexpr auto GAME_WIDTH = 40;
constexpr auto MSG_WIDTH = 20;
constexpr auto MSG_HEIGHT = 20;
constexpr auto MAZE_HEIGHT = 20;
constexpr auto MAZE_WIDTH = 20;

constexpr auto LAYER_WIDTH = GAME_WIDTH + MAZE_WIDTH;
constexpr auto LAYER_HEIGHT = GAME_HEIGHT;

constexpr auto MAX_PLAYER_COUNT = 2;

constexpr auto SPEED_DELTA = 80;
constexpr auto ACCELERATING_FACTOR = 0.998;

#endif