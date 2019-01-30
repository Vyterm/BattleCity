#include "GameMap.hpp"
#include "GamePlayer.hpp"
#include "GameEnemy.hpp"

#pragma region Construct & Destruct

GameMap::GameMap() : game::Renderer(LAYER_WIDTH, LAYER_HEIGHT, game::RenderType::StaticLayer0),
	m_jebelCollider(E_StaticCellType::JebelLand),
	m_grassCollider(E_StaticCellType::GrassLand),
	m_magmaCollider(E_StaticCellType::MagmaLand),
	m_frostCollider(E_StaticCellType::FrostLand),
	m_earthCollider(E_StaticCellType::EarthWall)
{
	m_players.push_back(new Player("玩家一", E_4BitColor::LCyan, 'W', 'A', 'S', 'D', 'F'));
	m_players.push_back(new Player("玩家二", E_4BitColor::LWhite, VK_UP, VK_LEFT, VK_DOWN, VK_RIGHT, VK_NUMPAD0));
	for (auto &player : m_players)
		player->set_Active(false);
	auto enemyL = new Enemy(E_TankType::Light, E_4BitColor::LGreen);
	enemyL->set_Active(false);
	m_enemys.push_back(enemyL);
	auto enemyM = new Enemy(E_TankType::Medium, E_4BitColor::LBlue);
	enemyM->set_Active(false);
	m_enemys.push_back(enemyM);
	auto enemyH = new Enemy(E_TankType::Heavy, E_4BitColor::LRed);
	enemyH->set_Active(false);
	m_enemys.push_back(enemyH);
	m_position = { 0, 0 };
}

GameMap::~GameMap()
{
	for (auto &pPlayer : m_players)
		delete pPlayer;
	for (auto &pEnemy : m_enemys)
		delete pEnemy;
}

void GameMap::SetModel(const GameMapModel & model)
{
	m_model = model;
}

#pragma endregion

#pragma region Interfaces

void GameMap::LoadModel(const GameMapModel & model)
{
	LoadStaticModel(model);
	LoadPlayerCell(model);
}

void GameMap::LoadStaticModel(const GameMapModel & model)
{
	m_jebelCollider.ReloadLand(model);
	m_grassCollider.ReloadLand(model);
	m_frostCollider.ReloadLand(model);
	m_magmaCollider.ReloadLand(model);
	m_earthCollider.ReloadLand(model);
}

#pragma endregion

#pragma region Reuse Methods

void GameMap::Reset()
{
	srand((unsigned)time(nullptr));
	game::RenderLayer::getInstance().Clear();
	LoadModel(m_model);
	game::RenderLayer::getInstance().Draw();
}

Player & GameMap::GetPlayer(int index) { return m_activePlayerCount == 1 || index == 0 ? *m_players[0] : *m_players[1]; }

Player * GameMap::CheckOver()
{
	Player *winer = nullptr;
	if (m_activePlayerCount == 2)
		winer = !m_players[0]->get_Active() ? m_players[1] : !m_players[1]->get_Active() ? m_players[0] : nullptr;
	else
		winer = !m_players[0]->get_Active() ? m_players[0] : nullptr;
	if (nullptr == winer) return winer;
	m_players[0]->set_Active(false);
	m_players[1]->set_Active(false);
	vyt::timer::get_instance().HandleClock();
	return winer;
}

#pragma endregion

#pragma region Renderer Methods

void GameMap::LoadPlayerCell(const GameMapModel & model)
{
	m_activePlayerCount = model.PlayerCount();
	for (size_t i = 0; i < m_activePlayerCount; ++i)
	{
		m_players[i]->set_GermPosition(model.GetPlayer(i));
		m_players[i]->set_Active(true);
	}
	for (int i = 0; i < 3; ++i)
	{
		m_enemys[i]->set_GermPosition({ 1 + i * 9, 1 });
		m_enemys[i]->set_Active(true);
	}
}

#pragma endregion
