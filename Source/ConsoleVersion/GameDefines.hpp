#ifndef GAME_LIB_HPP_INCLUDED
#define GAME_LIB_HPP_INCLUDED

#include "GameColor.hpp"
#include "GameMath.hpp"

constexpr auto GAME_NAME = "̹�˴�ս(Console Version) by ������";
constexpr auto GAME_VERSION = "Version 0.2.9";
constexpr int VERSION[] = { 0, 2, 9 };

constexpr auto WIN_HEIGHT = 43;
constexpr auto WIN_WIDTH = 122;
constexpr auto GAME_HEIGHT = 40;
constexpr auto GAME_WIDTH = 41;
constexpr auto MSG_WIDTH = 20;
constexpr auto MSG_HEIGHT = 40;
constexpr auto LAYER_WIDTH = WIN_WIDTH / 2;
constexpr auto LAYER_HEIGHT = WIN_HEIGHT;

constexpr auto MAX_PLAYER_COUNT = 2;

constexpr auto SPEED_DELTA = 150;
constexpr auto ACCELERATING_FACTOR = 0.99;

#endif