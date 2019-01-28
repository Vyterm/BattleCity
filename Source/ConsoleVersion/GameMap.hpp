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

	MapItem m_items[Width][Height];
	MapItem m_staticItems[Width][Height];

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
		m_staticItems[ci][ri].Set(E_CellType::None, E_SubType::SubType0, DEFAULT_COLOR);
		m_items[ci][ri].Set(E_CellType::None, E_SubType::SubType0, DEFAULT_COLOR);
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

	void LoadJumpCell(const GameMapModel &model)
	{
		for (auto &jpm : model.GetJumpPoints())
		{
			m_items[jpm.src.x][jpm.src.y].Set(E_CellType::Jump, jpm.color);
			m_items[jpm.src.x][jpm.src.y].jumpPoint = jpm.dest;
			m_items[jpm.dest.x][jpm.dest.y].Set(E_CellType::Jump, jpm.color);
			m_items[jpm.dest.x][jpm.dest.y].jumpPoint = jpm.src;
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
		// Some bug need be fixed
		//LoadJumpCell(model);
		GenerateRandomFood(model.get_FoodCount());
	}

	void LoadStaticModel(const GameMapModel &model)
	{
		for (int ci = 0; ci < Width; ++ci)
			for (int ri = 0; ri < Height; ++ri)
				LoadStaticCell(model, ci, ri);
		LoadJumpCell(model);
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

	static ConsoleColor ToSubColor(E_SubType subType)
	{
		return { SubTypeColors[int(subType)], DEFAULT_BACK_COLOR };
	}

	static string ToString(const MapItem &item)
	{
		return ToString(item.type, item.subType);
	}

	static string ToString(E_CellType celltype, E_SubType subtype)
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

	static void DrawCell(int x, int y, const MapItem &item)
	{
		DrawCell(x, y, item.color, ToString(item));
	}

	static void DrawCell(int x, int y, ConsoleColor color, const string &text)
	{
		game::RenderLayer::getInstance().SetString({ x,y }, text, game::ToRealColor(color.fore), game::ToRealColor(color.back));
	}

	void ClearCell()
	{
		for (int ri = 0; ri < Height; ++ri)
			for (int ci = 0; ci < Width; ++ci)
				m_items[ci][ri] = MapItem();
	}

	#pragma endregion

	#pragma region Create Methods

	bool SearchEmptyPosition(Vector2 &emptyPoint)
	{
		std::vector<Vector2> emptyPoints;
		for (int ri = 0; ri < GAME_HEIGHT; ++ri)
			for (int ci = 0; ci < GAME_WIDTH; ++ci)
				if (E_CellType::None == m_staticItems[ci][ri] && E_CellType::None == m_items[ci][ri])
					emptyPoints.push_back({ ci,ri });
		if (0 == emptyPoints.size())
			return false;
		emptyPoint = emptyPoints[rand() % emptyPoints.size()];
		return true;
	}

	bool GenerateRandomFood()
	{
		Vector2 emptyPoint;
		if (!SearchEmptyPosition(emptyPoint))
			return false;
		auto randomType = (unsigned)rand() % 100;
		//E_SubType subType = randomType < m_model.FoodWeight(E_FoodType::NormalEffect) ? E_SubType::SubType0 :
		//	randomType < m_model.FoodWeight(E_FoodType::AppendLength) ? E_SubType::SubType1 :
		//	randomType < m_model.FoodWeight(E_FoodType::RemoveLength) ? E_SubType::SubType2 :
		//	randomType < m_model.FoodWeight(E_FoodType::Acceleration) ? E_SubType::SubType3 :
		//	randomType < m_model.FoodWeight(E_FoodType::Deceleration) ? E_SubType::SubType4 :
		//	randomType < m_model.FoodWeight(E_FoodType::Reverse		) ? E_SubType::SubType5 :
		//	randomType < m_model.FoodWeight(E_FoodType::BuffStrong	) ? E_SubType::SubType6 : E_SubType::SubType7;
		E_SubType subType = randomType < 0 ? E_SubType::SubType0 :
			randomType < 20 ? E_SubType::SubType1 :
			randomType < 30 ? E_SubType::SubType2 :
			randomType < 40 ? E_SubType::SubType3 :
			randomType < 60 ? E_SubType::SubType4 :
			randomType < 65 ? E_SubType::SubType5 :
			randomType < 95 ? E_SubType::SubType6 : E_SubType::SubType7;

		m_items[emptyPoint.x][emptyPoint.y].Set(E_CellType::Food, subType, { SubTypeColors[int(subType)] ,DEFAULT_BACK_COLOR });
		return true;
	}

	bool GenerateRandomFood(size_t count)
	{
		for (size_t i = 0; i < count; ++i)
			if (!GenerateRandomFood())
				return false;
		return true;
	}

	#pragma endregion

	#pragma region CellType Methods

	const MapItem& operator[](Vector2 position) const { return m_items[position.x][position.y]; }
	MapItem& operator[](Vector2 position) { return m_items[position.x][position.y]; }
	const MapItem& Index(int x, int y) const { return m_items[x][y]; }
	MapItem& Index(int x, int y) { return m_items[x][y]; }
	const MapItem& GetItem(Vector2 position) { return E_CellType::None == m_items[position.x][position.y] ? m_staticItems[position.x][position.y] : m_items[position.x][position.y]; }
	const MapItem& GetStaticItem(Vector2 position) { return m_staticItems[position.x][position.y]; }

	bool MoveAble(int x, int y)
	{
		auto item = GetItem({ x, y });
		return E_CellType::None == item.type || E_CellType::Food == item.type || E_CellType::Jump == item.type || 
			(E_CellType::Land == item.type && E_SubType::SubType1 == item.subType) ||
			//(E_CellType::Land == item.type && E_SubType::SubType4 == item.subType) ||
			(E_CellType::Land == item.type && E_SubType::SubType4 == item.subType);
	}

	bool IsBlocked(const Vector2 &position)
	{
		bool isBlocked = true;
		isBlocked &= !MoveAble(position.x + 1, position.y);
		isBlocked &= !MoveAble(position.x - 1, position.y);
		isBlocked &= !MoveAble(position.x, position.y + 1);
		isBlocked &= !MoveAble(position.x, position.y - 1);
		return isBlocked;
	}

	#pragma endregion
};
typedef MapTemplate<GAME_WIDTH + MAZE_WIDTH, GAME_HEIGHT> GameMap;

#endif
