#include "GameMap.hpp"
#include "GameCtrl.hpp"
#include "CtrlDefines.hpp"

#pragma region Construct & Destruct

GameMap::GameMap(bool & updateUI) : m_isUpdateUI(updateUI), game::Renderer(LAYER_WIDTH, LAYER_HEIGHT, game::RenderType::StaticLayer0),
	m_jebelCollider(COLLIDER_TYPE_JEBEL_LANDSPACE, StaticCellImages[int(E_StaticCellType::JebelLand)]),
	m_grassCollider(COLLIDER_TYPE_GRASS_LANDSPACE, StaticCellImages[int(E_StaticCellType::GrassLand)]),
	m_earthCollider(COLLIDER_TYPE_EARTH_LANDSPACE, StaticCellImages[int(E_StaticCellType::EarthWall)])
{
	m_players.push_back(new TankPlayerCtrl("玩家一", updateUI, E_4BitColor::LCyan, 'W', 'A', 'S', 'D'));
	m_players.push_back(new TankPlayerCtrl("玩家二", updateUI, E_4BitColor::LWhite, VK_UP, VK_LEFT, VK_DOWN, VK_RIGHT));
	for (auto &player : m_players)
		player->Clear();
	m_position = { 0, 0 };
}

GameMap::~GameMap()
{
	for (auto &pPlayer : m_players)
		delete pPlayer;
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
		winer = !m_players[0]->get_Alive() ? m_players[1] : !m_players[1]->get_Alive() ? m_players[0] : nullptr;
	else
		winer = !m_players[0]->get_Alive() ? m_players[0] : nullptr;
	if (nullptr == winer) return winer;
	m_players[0]->Clear();
	m_players[1]->Clear();
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
		m_players[i]->Reset();
	}
}

#pragma endregion
