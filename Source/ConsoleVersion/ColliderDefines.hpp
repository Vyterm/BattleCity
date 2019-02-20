#ifndef GAME_CTRL_DEFINES_HPP_INCLUDED
#define GAME_CTRL_DEFINES_HPP_INCLUDED

#include <string>

const std::string COLLIDER_TYPE_FRIEND_TANK = "FT";
const std::string COLLIDER_TYPE_ENEMY_TANK = "ET";
const std::string COLLIDER_TYPE_SPACE_OPENSPACE = "SO";
const std::string COLLIDER_TYPE_JEBEL_LANDSPACE = "JL";
const std::string COLLIDER_TYPE_GRASS_LANDSPACE = "GL";
const std::string COLLIDER_TYPE_MAGMA_LANDSPACE = "ML";
const std::string COLLIDER_TYPE_FROST_LANDSPACE = "FL";
const std::string COLLIDER_TYPE_EARTH_LANDSPACE = "EL";
const std::string COLLIDER_TYPE_FRIEND_BULLET = "FB";
const std::string COLLIDER_TYPE_ENEMY_BULLET = "EB";
const std::string COLLIDER_TYPE_PLAYER_BASE = "PB";

constexpr auto SPEED_BULLET = 80;
constexpr auto SPEED_LIGHT_TANK = 30;
constexpr auto SPEED_MEDIUM_TANK = -20;
constexpr auto SPEED_HEAVY_TANK = -50;
constexpr auto SPEED_ASSAULT_TANK = 0;

#endif