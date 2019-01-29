#ifndef GAME_MAP_HPP
#define GAME_MAP_HPP

#include "GameModel.hpp"
#include "GameMapItem.hpp"
#include "GameRender.hpp"
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

class PlayerCtrl;
class TankPlayerCtrl;

template <size_t Width, size_t Height>
class MapTemplate : game::Renderer
{
private:
	#pragma region Fields

	bool &m_isUpdateUI;
	std::vector<PlayerCtrl*> m_players;
	size_t m_activePlayerCount = 0;

	Vector2 m_position;

	GameMapModel m_model;

	#pragma endregion

	#pragma region Load Map Model

	void LoadStaticCell(const GameMapModel &model, int ci, int ri)
	{
		Vector2 position = { ci, ri };
		switch (model.GetType(position))
		{
		case E_StaticCellType::OpenSpace:
			CacheString(ci, ri, ToString(E_CellType::None, E_SubType::SubType0), DEFAULT_COLOR);
			break;
		case E_StaticCellType::JebelLand:
			CacheString(ci, ri, ToString(E_CellType::Land, E_SubType::SubType0), DEFAULT_COLOR);
			break;
		case E_StaticCellType::GrassLand:
			CacheString(ci, ri, ToString(E_CellType::Land, E_SubType::SubType1), { DEFAULT_BACK_COLOR, SubTypeColors[1] });
			break;
		case E_StaticCellType::MagmaLand:
			CacheString(ci, ri, ToString(E_CellType::Land, E_SubType::SubType3), { DEFAULT_BACK_COLOR, SubTypeColors[3] });
			break;
		case E_StaticCellType::FrostLand:
			CacheString(ci, ri, ToString(E_CellType::Land, E_SubType::SubType4), { DEFAULT_BACK_COLOR, SubTypeColors[4] });
			break;
		}
	}

	void LoadPlayerCell(const GameMapModel &model)
	{
		m_activePlayerCount = model.PlayerCount();
		for (size_t i = 0; i < m_activePlayerCount; ++i)
			m_players[i]->Reset(model.GetPlayer(i));
		if (m_activePlayerCount == 1)
			m_players[0]->SetEnemy(*m_players[0]);
		else
		{
			m_players[0]->SetEnemy(*m_players[1]);
			m_players[1]->SetEnemy(*m_players[0]);
		}
	}

	#pragma endregion
public:
	const Vector2& getPosition() const { return m_position; }

public:
	#pragma region Construct & Destruct

	MapTemplate(bool &updateUI) : m_isUpdateUI(updateUI), game::Renderer(Width, Height, game::RenderType::StaticLayer0)
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

	#pragma region Load Map Model Interfaces

	void LoadModel(const GameMapModel &model)
	{
		LoadStaticModel(model);
		LoadPlayerCell(model);
	}

	void LoadStaticModel(const GameMapModel &model)
	{
		for (int ci = 0; ci < Width; ++ci)
			for (int ri = 0; ri < Height; ++ri)
				LoadStaticCell(model, ci, ri);
	}

	#pragma endregion

	#pragma region Reuse Methods

	void Reset()
	{
		srand((unsigned)time(nullptr));
		ClearCell();
		LoadModel(m_model);
		SetColor(DEFAULT_COLOR);
		system("cls");
		game::RenderLayer::getInstance().Draw();
	}

	PlayerCtrl& GetPlayer(int index) { return m_activePlayerCount == 1 || index == 0 ? *m_players[0] : *m_players[1]; }
	PlayerCtrl* CheckOver()
	{
		PlayerCtrl *winer = nullptr;
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

	#pragma region Render Methods

	static string ToString(E_CellType celltype, E_SubType subtype = E_SubType::SubType0)
	{
		static const string images[] = { "  ", "■", "☆", "◎", "¤", "※", "〓" };
		//static const string images[] = { "  ", "〓", "❀", "◎", "¤", "※" };
		if (celltype == E_CellType::Land)
		{
			switch (subtype)
			{
			case E_SubType::SubType1:
				return "≡";
			case E_SubType::SubType3:
				return "≈";
			case E_SubType::SubType4:
				return "〓";
			}
		}
		return images[int(celltype)];
	}

	static void DrawCell(int x, int y, ConsoleColor color, const string &text)
	{
		game::RenderLayer::getInstance().SetString({ x,y }, text, game::ToRealColor(color.fore), game::ToRealColor(color.back));
	}

	void ClearCell()
	{
		game::RenderLayer::getInstance().Clear();
	}

	#pragma endregion

	#pragma region CellType Methods


	#pragma endregion
};
typedef MapTemplate<GAME_WIDTH + MAZE_WIDTH, GAME_HEIGHT> GameMap;

#endif
