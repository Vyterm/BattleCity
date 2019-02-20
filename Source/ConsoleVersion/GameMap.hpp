#ifndef GAME_MAP_HPP
#define GAME_MAP_HPP

#include "GameModel.hpp"
#include "GameRender.hpp"
#include "GameTerrian.hpp"
#include "GameBase.hpp"

#include <string>
#include <vector>

using std::string;

class Player;
class Enemy;

class GameMap
{
private:
	std::vector<Player*> m_players;
	std::vector<Enemy*> m_enemys;
	Base m_base;
	size_t m_activePlayerCount = 0;

	LevelModel m_model;

	TerrianCollider m_terrian;
public:
	GameMap();
	~GameMap();

	void SetModel(const LevelModel &model);
	void LoadModel(const LevelModel &model);
	void LoadStaticModel(const LevelModel &model);

	void Reset();

	Player& GetPlayer(int index);
	bool CheckOver();
};

#endif
