#include "GameMap.hpp"
#include "GamePlayer.hpp"
#include "GameEnemy.hpp"
#include "GameBullet.hpp"
#include "GameSurface.hpp"

#include <algorithm>

constexpr auto MAX_ENEMY_COUNT = 5;

static const string GameArchiveLocation = "save.bcg";

#pragma region Construct & Destruct

size_t GameMap::ActiveEnemyCount() const
{
	size_t count = 0;
	for (auto &enemy : m_enemys)
		if (enemy.get_Active())
			++count;
	return count;
}

GameMap::GameMap()
{
	m_model = __LevelModel::NewLevel();
	m_players.Append<Player>("玩家一", Player::PlayerKeys[0]);
	m_players.Append<Player>("玩家二", Player::PlayerKeys[1]);
	for (auto &player : m_players)
		player.set_Active(false);
	m_enemys.AppendRange<Enemy>(MAX_ENEMY_COUNT);
	for (auto &enemy : m_enemys)
		enemy.set_Active(false);
	vyt::autoTimer<GameMap>::RegisterTimer(*this, 1000);
}

GameMap::~GameMap()
{
	game::RenderLayer::getInstance().Clear();
	vyt::autoTimer<GameMap>::UnregisterTimer(*this);
}

bool GameMap::ExistArchive()
{
	return FileManager::Exist(FileManager::DefaultPath + GameArchiveLocation);
}

void GameMap::Archive() const
{
	std::ofstream ofs;
	ofs.open(FileManager::DefaultPath + GameArchiveLocation);
	ofs << *this;
	ofs.close();
	Bullet::Clear();
}

bool GameMap::LoadByArchive()
{
	if (!ExistArchive()) return false;
	std::ifstream ifs;
	ifs.open(FileManager::DefaultPath + GameArchiveLocation);
	ifs >> *this;
	ifs.close();
	return true;
}

bool GameMap::LoadByBrowse()
{
	bool success = m_model->LoadByBrowse();
	if (success) Reset();
	return success;
}

void GameMap::Restore()
{

}

#pragma endregion

#pragma region Interfaces

void GameMap::ActiveColliders()
{
	m_remainEnemys = m_model->EnemyModels();
	for (size_t i = 0; i < m_activePlayerCount; ++i)
		m_players[i].set_Active(true);
	m_base.SetActive(true);
}

void GameMap::RenderModel(const LevelModel & model)
{
	m_terrian.ReloadLand(model);
	MsgSurface();
	auto playerModels = model->PlayerModels();
	std::sort(playerModels.begin(), playerModels.end(), [](auto &lhs, auto &rhs) { return lhs.germPosition < rhs.germPosition; });
	m_activePlayerCount = playerModels.size();
	size_t i = 0;
	for (auto &playerModel : playerModels)
	{
		m_players[i].ApplyModel(playerModel);
		m_players[i].ActiveDraw();
		++i;
	}
	for (; i < m_players.size(); ++i)
		m_players[i].DeactiveDraw();
}

#pragma endregion

#pragma region Reuse Methods

void GameMap::Reset()
{
	srand((unsigned)time(nullptr));
	game::RenderLayer::getInstance().Clear();
	m_terrian.ClearLands();
	RenderModel(m_model);
	ActiveColliders();
	game::RenderLayer::getInstance().Draw();
}

const Player & GameMap::GetPlayer(size_t index) const { return m_activePlayerCount == 1 || index == 0 ? m_players[0] : m_players[1]; }

void GameMap::Process()
{
	if (m_remainEnemys.empty()) return;
	for (auto &enemy : m_enemys)
	{
		if (enemy.get_Active()) continue;
		enemy.ApplyModel(m_remainEnemys.front());
		enemy.set_Active(true);
		m_remainEnemys.pop_front();
		break;
	}
	vyt::autoTimer<GameMap>::ChangeDeltaTime(*this, 10000 / (1 + m_enemys.size() - ActiveEnemyCount()));
}

bool GameMap::CheckOver()
{
	bool isEnemyWin = true;
	for (auto &player : m_players)
		isEnemyWin = isEnemyWin && !player.get_Active();
	isEnemyWin = isEnemyWin || !m_base.IsActive();
	bool isPlayerWin = m_remainEnemys.empty();
	for (auto &enemy : m_enemys)
		isPlayerWin = isPlayerWin && !enemy.get_Active();
	if (!isEnemyWin && !isPlayerWin) return false;

	for (auto &player : m_players)
		player.set_Active(false);
	for (auto &enemy : m_enemys)
		enemy.set_Active(false);
	Bullet::Clear();
	vyt::timer::get_instance().HandleClock();
	OverSurface(isPlayerWin);
	FileManager::DeleteFile(FileManager::DefaultPath + GameArchiveLocation);
	return true;
}

#pragma endregion

#pragma region Archive

std::ostream& operator<<(std::ostream &os, const GameMap &map)
{
	os << map.m_model;
	os << map.m_terrian;
	os << map.m_remainEnemys.size() << " ";
	for (auto &remainEnemy : map.m_remainEnemys)
		os << remainEnemy;
	os << map.m_activePlayerCount << " ";
	for (size_t i = 0; i < map.m_activePlayerCount; ++i)
	{
		os << map.m_players[i].getModel();
		os << map.m_players[i].getDirection();
		os << map.m_players[i].getPosition();
		os << map.m_players[i].getScore() << " ";
		os << map.m_players[i].getHealth() << " ";
		os << map.m_players[i].getLifeCount() << " ";
	}
	auto aec = map.ActiveEnemyCount();
	os << aec << " ";
	for (size_t i = 0; i < aec; ++i)
	{
		os << map.m_enemys[i].getModel();
		os << map.m_enemys[i].getDirection();
		os << map.m_enemys[i].getPosition();
		os << map.m_enemys[i].getHealth() << " ";
		os << map.m_enemys[i].getLifeCount() << " ";
	}
	return os;
}

std::istream& operator>>(std::istream &is, GameMap &map)
{
	game::RenderLayer::getInstance().Clear();
	is >> *map.m_model;
	is >> map.m_terrian;
	size_t remainEnemyCount;
	is >> remainEnemyCount;
	for (size_t i = 0; i < remainEnemyCount; ++i)
	{
		TankModel tank;
		is >> tank;
		map.m_remainEnemys.push_back(tank);
	}
	is >> map.m_activePlayerCount;
	for (size_t i = 0; i < map.m_activePlayerCount; ++i)
	{
		TankModel tank;
		Direction2D direction;
		Vector2 position;
		int score, health, lifeCount;
		is >> tank >> direction >> position >> score >> health >> lifeCount;
		map.m_players[i].ApplyModel(tank);
		map.m_players[i].set_Active(true);
		map.m_players[i].setDirection(direction);
		map.m_players[i].setPosition(position);
		map.m_players[i].setScore(score);
		map.m_players[i].setHealth(health);
		map.m_players[i].setLifeCount(lifeCount);
	}
	size_t activeEnemyCount;
	is >> activeEnemyCount;
	for (size_t i = 0; i < activeEnemyCount; ++i)
	{
		TankModel tank;
		Direction2D direction;
		Vector2 position;
		int health, lifeCount;
		is >> tank >> direction >> position >> health >> lifeCount;
		map.m_enemys[i].ApplyModel(tank);
		map.m_enemys[i].set_Active(true);
		map.m_enemys[i].setDirection(direction);
		map.m_enemys[i].setPosition(position);
		map.m_enemys[i].setHealth(health);
		map.m_enemys[i].setLifeCount(lifeCount);
	}
	return is;
}

#pragma endregion

