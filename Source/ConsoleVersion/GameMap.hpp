#ifndef GAME_MAP_HPP
#define GAME_MAP_HPP

#include "GameModel.hpp"
#include "GameRender.hpp"
#include "GameCollider.hpp"
#include "CtrlDefines.hpp"
#include "winapi.hpp"

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <ctime>

using std::cout;
using std::cin;
using std::endl;
using std::string;

class Player;
class TankPlayerCtrl;

constexpr E_4BitColor SubTypeColors[] = { E_4BitColor::White, E_4BitColor::LGreen, E_4BitColor::LPurple, E_4BitColor::LRed, E_4BitColor::LBlue, E_4BitColor::Black, E_4BitColor::LYellow, E_4BitColor::Green, E_4BitColor::Cyan };

template <size_t Width, size_t Height>
class MapTemplate : game::Renderer, game::Collider
{
private:
	#pragma region Fields

	bool &m_isUpdateUI;
	std::vector<Player*> m_players;
	size_t m_activePlayerCount = 0;
	std::vector<Vector2> m_jebelLands;

	Vector2 m_position;

	GameMapModel m_model;

	#pragma endregion

	#pragma region Renderer Methods

	void LoadStaticCell(const GameMapModel &model, int ci, int ri)
	{
		Vector2 position = { ci, ri };
		switch (model.GetType(position))
		{
		case E_StaticCellType::OpenSpace:
			CacheString(ci, ri, StaticCellImages[int(E_StaticCellType::OpenSpace)], DEFAULT_COLOR);
			break;
		case E_StaticCellType::JebelLand:
			CacheString(ci, ri, StaticCellImages[int(E_StaticCellType::JebelLand)], DEFAULT_COLOR);
			m_jebelLands.push_back(position);
			break;
		case E_StaticCellType::GrassLand:
			CacheString(ci, ri, StaticCellImages[int(E_StaticCellType::GrassLand)], { DEFAULT_BACK_COLOR, SubTypeColors[1] });
			break;
		case E_StaticCellType::MagmaLand:
			CacheString(ci, ri, StaticCellImages[int(E_StaticCellType::MagmaLand)], { DEFAULT_BACK_COLOR, SubTypeColors[3] });
			break;
		case E_StaticCellType::FrostLand:
			CacheString(ci, ri, StaticCellImages[int(E_StaticCellType::FrostLand)], { DEFAULT_BACK_COLOR, SubTypeColors[4] });
			break;
		}
	}

	void LoadPlayerCell(const GameMapModel &model)
	{
		m_activePlayerCount = model.PlayerCount();
		for (size_t i = 0; i < m_activePlayerCount; ++i)
		{
			m_players[i]->set_GermPosition(model.GetPlayer(i));
			m_players[i]->Reset();
		}
	}

	#pragma endregion

	#pragma region Collider Methods

	void OnCollision(Collider &collider)
	{

	}
	bool Contains(const Vector2 &position)
	{
		for (auto &land : m_jebelLands)
			if (land == position)
				return true;
		return false;
	}
	bool SetPositionByIndex(size_t index, Vector2 &point)
	{
		if (index >= m_jebelLands.size())
			return false;
		point = m_jebelLands[index];
		return true;
	}

	#pragma endregion
public:
	const Vector2& getPosition() const { return m_position; }
	const std::string& getType() const { return COLLIDER_TYPE_JEBEL_LANDSPACE; }

public:
	#pragma region Construct & Destruct

	MapTemplate(bool &updateUI) : m_isUpdateUI(updateUI), game::Renderer(Width, Height, game::RenderType::StaticLayer0), game::Collider(true)
	{
		m_players.push_back(new TankPlayerCtrl("玩家一", updateUI, E_4BitColor::LCyan, 'W', 'A', 'S', 'D'));
		m_players.push_back(new TankPlayerCtrl("玩家二", updateUI, E_4BitColor::LWhite, VK_UP, VK_LEFT, VK_DOWN, VK_RIGHT));
		for (auto &player : m_players)
			player->Clear();
		m_position = { 0, 0 };
	}
	~MapTemplate()
	{
		for (auto &pPlayer : m_players)
			delete pPlayer;
	}

	void SetModel(const GameMapModel &model)
	{
		m_model = model;
	}

	#pragma endregion

	#pragma region Interfaces

	void LoadModel(const GameMapModel &model)
	{
		LoadStaticModel(model);
		LoadPlayerCell(model);
	}

	void LoadStaticModel(const GameMapModel &model)
	{
		m_jebelLands.erase(m_jebelLands.begin(), m_jebelLands.end());
		for (int ci = 0; ci < Width; ++ci)
			for (int ri = 0; ri < Height; ++ri)
				LoadStaticCell(model, ci, ri);
	}

	#pragma endregion

	#pragma region Reuse Methods

	void Reset()
	{
		srand((unsigned)time(nullptr));
		game::RenderLayer::getInstance().Clear();
		LoadModel(m_model);
		game::RenderLayer::getInstance().Draw();
	}

	Player& GetPlayer(int index) { return m_activePlayerCount == 1 || index == 0 ? *m_players[0] : *m_players[1]; }
	Player* CheckOver()
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

};
typedef MapTemplate<GAME_WIDTH + MAZE_WIDTH, GAME_HEIGHT> GameMap;

#endif
