#include "GameMap.hpp"
#include "GamePlayer.hpp"
#include "GameEnemy.hpp"
#include "GameBullet.hpp"
#include "GameSurface.hpp"

#pragma region Construct & Destruct

GameMap::GameMap()
{
	m_players.push_back(new Player("玩家一", E_4BitColor::LCyan, 'W', 'A', 'S', 'D', 'F'));
	m_players.push_back(new Player("玩家二", E_4BitColor::LWhite, VK_UP, VK_LEFT, VK_DOWN, VK_RIGHT, VK_NUMPAD0));
	for (auto &player : m_players)
		player->set_Active(false);
	auto enemyL = new Enemy(E_TankType::Light);
	enemyL->set_Active(false);
	m_enemys.push_back(enemyL);
	auto enemyM = new Enemy(E_TankType::Medium);
	enemyM->set_Active(false);
	m_enemys.push_back(enemyM);
	auto enemyH = new Enemy(E_TankType::Heavy);
	enemyH->set_Active(false);
	m_enemys.push_back(enemyH);
}

GameMap::~GameMap()
{
	for (auto &pPlayer : m_players)
		delete pPlayer;
	for (auto &pEnemy : m_enemys)
		delete pEnemy;
	game::RenderLayer::getInstance().Clear();
}

void GameMap::SetModel(const LevelModel & model)
{
	m_model = model;
}

#pragma endregion

#pragma region Interfaces

void GameMap::ActiveColliders()
{
	for (size_t i = 0; i < m_activePlayerCount; ++i)
	{
		m_players[i]->set_Active(true);
	}
	for (int i = 0; i < 3; ++i)
	{
		m_enemys[i]->set_GermPosition({ 1 + i * (GAME_WIDTH / 2 - 2), 1 });
		m_enemys[i]->set_Active(true);
	}
}

void GameMap::RenderModel(const LevelModel & model)
{
	m_terrian.ReloadLand(model);
	MsgSurface();
	auto germPoints = model.GermPoints();
	m_activePlayerCount = germPoints.size();
	size_t i = 0;
	for (auto &germPoint : germPoints)
	{
		m_players[i]->set_GermPosition(germPoint);
		m_players[i]->ActiveDraw();
		++i;
	}
	for (size_t i = m_activePlayerCount; i < m_players.size(); ++i)
		m_players[i]->DeactiveDraw();
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
	m_base.SetActive(true);
	game::RenderLayer::getInstance().Draw();
}

Player & GameMap::GetPlayer(int index) { return m_activePlayerCount == 1 || index == 0 ? *m_players[0] : *m_players[1]; }

bool GameMap::CheckOver()
{
	bool isEnemyWin = true;
	for (auto &pPlayer : m_players)
		isEnemyWin = isEnemyWin && !pPlayer->get_Active();
	isEnemyWin = isEnemyWin || !m_base.IsActive();
	bool isPlayerWin = true;
	for (auto &pEnemy : m_enemys)
		isPlayerWin = isPlayerWin && !pEnemy->get_Active();
	if (!isEnemyWin && !isPlayerWin) return false;

	for (auto &pPlayer : m_players)
		pPlayer->set_Active(false);
	for (auto &pEnemy : m_enemys)
		pEnemy->set_Active(false);
	Bullet::Clear();
	vyt::timer::get_instance().HandleClock();
	OverSurface(isPlayerWin);
	return true;
}

#pragma endregion
