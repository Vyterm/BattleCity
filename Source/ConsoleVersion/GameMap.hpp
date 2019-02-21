#ifndef GAME_MAP_HPP
#define GAME_MAP_HPP

#include "GameModel.hpp"
#include "GameTerrian.hpp"
#include "GameBase.hpp"
#include "vytVector.hpp"
#include "vytTimer.hpp"

#include <string>
#include <deque>

using std::string;

class Player;
class Enemy;

class GameMap
{
private:
	LevelModel m_model;

	std::deque<TankModel> m_remainEnemys;
	vyt::vector<Player> m_players;
	vyt::vector<Enemy> m_enemys;
	Base m_base;
	size_t m_activePlayerCount = 0;

	TerrianCollider m_terrian;
private:
	size_t ActiveEnemyCount() const;
public:
	size_t RemainEnemyCount() const { return m_remainEnemys.size(); }
public:
	GameMap();
	~GameMap();

	static bool ExistArchive();
	void Archive() const;
	bool LoadByArchive();
	bool LoadByBrowse();
	void Restore();
	void Reset();
private:
	void ActiveColliders();
public:
	void RenderModel(const LevelModel &model);

	const Player& GetPlayer(size_t index) const;
	void Process();
	bool CheckOver();

	friend std::ostream& operator<<(std::ostream &os, const GameMap &map);
	friend std::istream& operator>>(std::istream &is, GameMap &map);
};

#endif
