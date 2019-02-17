#include "GameSurface.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>

using std::cout;
using std::cin;
using std::endl;

static constexpr auto GAME_OVER_S_INDEXY = GAME_HEIGHT / 2 - 5;
static constexpr auto GAME_OVER_E_INDEXY = GAME_HEIGHT / 2 + 5;
static constexpr auto GAME_OVER_S_INDEXX = GAME_WIDTH / 2 - 15;
static constexpr auto GAME_OVER_E_INDEXX = GAME_WIDTH / 2 + 15;

static constexpr auto GAME_MSG_S_INDEXY = 0;
static constexpr auto GAME_MSG_E_INDEXY = MSG_HEIGHT - 1;
static constexpr auto GAME_MSG_S_INDEXX = GAME_WIDTH;
static constexpr auto GAME_MSG_E_INDEXX = GAME_WIDTH + MSG_WIDTH - 1;

void DrawBorder(int posXS, int posXE, int posYS, int posYE)
{
	for (int ri = posYS; ri <= posYE; ++ri)
	{
		for (int ci = posXS; ci <= posXE; ++ci)
		{
			E_StaticCellType cellType = (ri == posYS || ri == posYE || ci == posXS || ci == posXE) ? E_StaticCellType::JebelLand : E_StaticCellType::OpenSpace;
			game::RenderLayer::getInstance().SetString({ ci,ri }, StaticCellImages[int(cellType)], game::ToRealColor(DEFAULT_FORE_COLOR), game::ToRealColor(DEFAULT_BACK_COLOR));
		}
	}
}

void DrawHollowBorder(int posXS, int posXE, int posYS, int posYE)
{
	const string &cellString = StaticCellImages[int(E_StaticCellType::JebelLand)];
	for (int ci = posXS; ci <= posXE; ++ci)
		game::RenderLayer::getInstance().SetString({ ci,posYS }, cellString, game::ToRealColor(DEFAULT_FORE_COLOR), game::ToRealColor(DEFAULT_BACK_COLOR));
	for (int ci = posXS; ci <= posXE; ++ci)
		game::RenderLayer::getInstance().SetString({ ci,posYE }, cellString, game::ToRealColor(DEFAULT_FORE_COLOR), game::ToRealColor(DEFAULT_BACK_COLOR));
	for (int ri = posYS + 1; ri < posYE; ++ri)
	{
		game::RenderLayer::getInstance().SetString({ posXS,ri }, cellString, game::ToRealColor(DEFAULT_FORE_COLOR), game::ToRealColor(DEFAULT_BACK_COLOR));
		game::RenderLayer::getInstance().SetString({ posXE,ri }, cellString, game::ToRealColor(DEFAULT_FORE_COLOR), game::ToRealColor(DEFAULT_BACK_COLOR));
	}
}

void UnfinishedSurface(int x, int y, DWORD millseconds, string text)
{
	SetColor(DEFAULT_COLOR);
	system("cls");
	SetPosition(x, y);
	cout << text;
	Sleep(millseconds);
	while (!IsKeyDown(VK_RETURN))
		continue;
}

void MsgSurface()
{
	DrawHollowBorder(GAME_MSG_S_INDEXX, GAME_MSG_E_INDEXX, GAME_MSG_S_INDEXY, GAME_MSG_E_INDEXY);
}

void OverSurface(const Player &winer, bool isWin)
{
	DrawBorder(GAME_OVER_S_INDEXX, GAME_OVER_E_INDEXX, GAME_OVER_S_INDEXY, GAME_OVER_E_INDEXY);
	game::RenderLayer::getInstance().SetString({ GAME_OVER_S_INDEXX + 13, GAME_OVER_S_INDEXY + 4 },
		winer.get_Name(), game::ToRealColor(winer.get_Color()), game::ToRealColor(DEFAULT_BACK_COLOR));
	game::RenderLayer::getInstance().SetString({ GAME_OVER_S_INDEXX + 13 + int(winer.get_Name().size())/2, GAME_OVER_S_INDEXY + 4 },
		(isWin ? "胜利" : "失败"), game::ToRealColor(DEFAULT_FORE_COLOR), game::ToRealColor(DEFAULT_BACK_COLOR));
	game::RenderLayer::getInstance().SetString({ GAME_OVER_S_INDEXX + 8, GAME_OVER_S_INDEXY + 5 },
		"输入q退出游戏，输入r重新开始", game::ToRealColor(DEFAULT_FORE_COLOR), game::ToRealColor(DEFAULT_BACK_COLOR));
}

void ShowMsg(Msgs && msgs)
{
	int ri = GAME_MSG_S_INDEXY + 1;
	for (auto msg : msgs)
	{
		if (++ri >= (GAME_MSG_E_INDEXY - 1))
			break;
		int ci = GAME_MSG_S_INDEXX + (GAME_MSG_E_INDEXX - GAME_MSG_S_INDEXX - msg.size() / 2) / 2;
		game::RenderLayer::getInstance().SetString({ ci,ri }, msg.ToString(), game::ToRealColor(DEFAULT_FORE_COLOR), game::ToRealColor(DEFAULT_BACK_COLOR));
	}
}

void ShowMsg(const Player & player1, const Player & player2)
{
	if (&player1 == &player2)
	{
		ShowMsg({
			{ "" },
			{ "" },
			{ "玩家一" },
			{ "分数", player1.get_Score(), 3, '0' },
			{ "速度", player1.TextSpeed(), 3, '0' },
			{ "" },
			{ "" },
			{ "" },
			{ "" },
			{ "" },
			{ "W,A,S,D操作玩家一" },
			{ "" },
			{ "空格键暂停游戏" },
			});
	}
	else
	{
		ShowMsg({
			{ "" },
			{ "" },
			{ "玩家一", "玩家二" },
			{ "分数", player1.get_Score(), player2.get_Score(), 3, '0' },
			{ "速度", player1.TextSpeed(), player2.TextSpeed(), 3, '0' },
			{ "" },
			{ "" },
			{ "" },
			{ "" },
			{ "" },
			{ "W,A,S,D操作玩家一" },
			{ "↑,↓,←,→操作玩家二" },
			{ "空格键暂停游戏" },
			});
	}
}

const SurfaceText HomeSurface::m_select("→_→");
const SurfaceText HomeSurface::m_empty("     ");

template <typename TVector, typename TValue>
int IndexOf(const TVector &vector, const TValue &value)
{
	return std::distance(vector.begin(), std::find(vector.begin(), vector.end(), value));
}
template <typename TKey, typename TValue>
int IndexOf(const std::map<TKey, TValue> &map, const TKey &value)
{
	return std::distance(map.begin(), map.find(value));
}

HomeSurface::HomeSurface(bool isContinue) : game::Renderer(0, 0, game::RenderType::UICanvas, false), m_isContinue(isContinue), m_currentOption(NewGame), m_position(0, 0)
{
	m_options = {
		{ NewGame, {"开始游戏"}   },
		{ Setting, {"游戏设置"}   },
		{ Editor,  {"地图编辑器"} },
		{ Quit,	   {"退出游戏"}	 }
	};
	if (isContinue)
	{
		m_options.emplace(Continue, SurfaceText("继续游戏" ));
	}
}

void HomeSurface::Update()
{
	if (!IsActive()) return;
	if (IsKeyDown(VK_RETURN))
		SetActive(false);
	if (IsKeyDown(VK_UP) || IsKeyDown('W'))
	{
		m_empty.Output({ 25,20 + IndexOf(m_options, m_currentOption) });
		m_currentOption = NewGame == m_currentOption ? Quit : E_HomeOption(m_currentOption - 1);
		if (Continue == m_currentOption && !m_isContinue)
			m_currentOption = E_HomeOption(m_currentOption - 1);
		m_select.Output({ 25,20 + IndexOf(m_options, m_currentOption) });
	}
	if (IsKeyDown(VK_DOWN) || IsKeyDown('S'))
	{
		m_empty.Output({ 25,20 + IndexOf(m_options, m_currentOption) });
		m_currentOption = Quit == m_currentOption ? NewGame : E_HomeOption(m_currentOption + 1);
		if (Continue == m_currentOption && !m_isContinue)
			m_currentOption = E_HomeOption(m_currentOption + 1);
		m_select.Output({ 25,20 + IndexOf(m_options, m_currentOption) });
	}
}

bool HomeSurface::IsActive() const
{
	return GetDrawActive();
}

void HomeSurface::SetActive(bool isActive)
{
	if (isActive && !IsActive())
	{
		game::RenderLayer::getInstance().Clear();
		SetColor(DEFAULT_COLOR);
		system("cls");
		DrawHollowBorder(0, LAYER_WIDTH - 1, 0, 3);
		DrawHollowBorder(0, LAYER_WIDTH - 1, 3, 40);
		SurfaceText name(GAME_NAME, game::ToRealColor(E_4BitColor::LCyan));
		name.Output({ 21, 1 });
		SurfaceText version(GAME_VERSION, game::ToRealColor(E_4BitColor::LWhite));
		version.Output({ 27, 2 });
		int i = 0;
		for (auto &option : m_options)
			option.second.Output({ 28, 20 + i++ });
		m_select.Output({ 25,20 + IndexOf(m_options, m_currentOption) });
	}
	SetDrawActive(isActive);
}
