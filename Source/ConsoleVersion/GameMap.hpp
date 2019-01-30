#ifndef GAME_MAP_HPP
#define GAME_MAP_HPP

#include "GameModel.hpp"
#include "GameRender.hpp"
#include "GameTerrian.hpp"
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
	size_t m_activePlayerCount = 0;

	Vector2 m_position;

	GameMapModel m_model;

	TerrianCollider m_jebelCollider;
	TerrianCollider m_grassCollider;
	TerrianCollider m_magmaCollider;
	TerrianCollider m_frostCollider;
	TerrianCollider m_earthCollider;
private:
	void LoadStaticCell(const GameMapModel &model, int ci, int ri);
	void LoadPlayerCell(const GameMapModel &model);

public:
	const Vector2& getPosition() const { return m_position; }
public:
	GameMap();
	~GameMap();

	void SetModel(const GameMapModel &model);
	void LoadModel(const GameMapModel &model);
	void LoadStaticModel(const GameMapModel &model);

	void Reset();

	Player& GetPlayer(int index);
	Player* CheckOver();
};

#endif
