#include "GameMap.hpp"
#include "GamePlayer.hpp"
#include "GameEnemy.hpp"
#include "CtrlDefines.hpp"

#pragma region Construct & Destruct

GameMap::GameMap() : game::Renderer(LAYER_WIDTH, LAYER_HEIGHT, game::RenderType::StaticLayer0),
	m_jebelCollider(COLLIDER_TYPE_JEBEL_LANDSPACE, StaticCellImages[int(E_StaticCellType::JebelLand)]),
	m_grassCollider(COLLIDER_TYPE_GRASS_LANDSPACE, StaticCellImages[int(E_StaticCellType::GrassLand)]),
	m_earthCollider(COLLIDER_TYPE_EARTH_LANDSPACE, StaticCellImages[int(E_StaticCellType::EarthWall)])
{
	m_players.push_back(new Player("���һ", E_4BitColor::LCyan, 'W', 'A', 'S', 'D', 'F'));
	m_players.push_back(new Player("��Ҷ�", E_4BitColor::LWhite, VK_UP, VK_LEFT, VK_DOWN, VK_RIGHT, VK_NUMPAD0));
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
	m_jebelCollider.ClearLands();
	m_grassCollider.ClearLands();
	m_earthCollider.ClearLands();
	for (int ci = 0; ci < GameMapModel::WIDTH; ++ci)
		for (int ri = 0; ri < GameMapModel::HEIGHT; ++ri)
			LoadStaticCell(model, ci, ri);
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

void GameMap::LoadStaticCell(const GameMapModel & model, int ci, int ri)
{
	Vector2 position = { ci, ri };
	switch (model.GetType(position))
	{
	case E_StaticCellType::OpenSpace:
		CacheString(ci, ri, StaticCellImages[int(E_StaticCellType::OpenSpace)], DEFAULT_COLOR);
		break;
	case E_StaticCellType::JebelLand:
		m_jebelCollider.AppendLand(position, DEFAULT_COLOR);
		break;
	case E_StaticCellType::GrassLand:
		m_grassCollider.AppendLand(position, { DEFAULT_BACK_COLOR, E_4BitColor::LGreen });
		break;
	case E_StaticCellType::MagmaLand:
		CacheString(ci, ri, StaticCellImages[int(E_StaticCellType::MagmaLand)], { DEFAULT_BACK_COLOR, E_4BitColor::LRed });
		break;
	case E_StaticCellType::FrostLand:
		CacheString(ci, ri, StaticCellImages[int(E_StaticCellType::FrostLand)], { DEFAULT_BACK_COLOR, E_4BitColor::LBlue });
		break;
	case E_StaticCellType::EarthWall:
		m_earthCollider.AppendLand(position, { model.GetColor(position), DEFAULT_BACK_COLOR });
		break;
	}
}

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