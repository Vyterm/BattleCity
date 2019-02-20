#include "GameMap.hpp"
#include "GamePlayer.hpp"
#include "GameEnemy.hpp"
#include "GameBullet.hpp"
#include "GameSurface.hpp"

#include <algorithm>

constexpr auto MAX_ENEMY_COUNT = 5;

#pragma region Construct & Destruct

GameMap::GameMap()
{
	m_players.Append<Player>("玩家一", Player::PlayerKeys[0]);
	m_players.Append<Player>("玩家二", Player::PlayerKeys[1]);
	for (auto &player : m_players)
		player.set_Active(false);
	m_enemys.AppendRange<Enemy>(MAX_ENEMY_COUNT);
	for (auto &enemy : m_enemys)
		enemy.set_Active(false);
	vyt::autoTimer<GameMap>::RegisterTimer(*this, 5000);
}

GameMap::~GameMap()
{
	game::RenderLayer::getInstance().Clear();
	vyt::autoTimer<GameMap>::UnregisterTimer(*this);
}

void GameMap::CacheModel(const LevelModel & model)
{
	m_model = model;
}

#pragma endregion

#pragma region Interfaces

void GameMap::ActiveColliders()
{
	m_remainEnemys = m_model.EnemyModels();
	for (size_t i = 0; i < m_activePlayerCount; ++i)
		m_players[i].set_Active(true);
	m_base.SetActive(true);
}

void GameMap::RenderModel(const LevelModel & model)
{
	m_terrian.ReloadLand(model);
	MsgSurface();
	auto playerModels = model.PlayerModels();
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
	size_t deadEnemyCount = 0;
	for (auto &enemy : m_enemys)
		if (!enemy.get_Active())
			++deadEnemyCount;
	vyt::autoTimer<GameMap>::ChangeDeltaTime(*this, 10000 / (deadEnemyCount + 1));
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
	return true;
}

#pragma endregion
