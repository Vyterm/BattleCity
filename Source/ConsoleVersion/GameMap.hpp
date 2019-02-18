#ifndef GAME_MAP_HPP
#define GAME_MAP_HPP

#include "GameModel.hpp"
#include "GameRender.hpp"
#include "GameTerrian.hpp"
#include "GameBase.hpp"
#include "vytVector.hpp"
#include "winapi.hpp"

#include <string>
#include <vector>

using std::string;

class Player;
class Enemy;

class GameMap : game::Renderer
{
private:
	std::vector<Player*> m_players;
	std::vector<Enemy*> m_enemys;
	Base m_base;
	size_t m_activePlayerCount = 0;

	Vector2 m_position;

	LevelModel m_model;

	vyt::vector<TerrianCollider> m_terrians;
public:
	const Vector2& getPosition() const { return m_position; }
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
