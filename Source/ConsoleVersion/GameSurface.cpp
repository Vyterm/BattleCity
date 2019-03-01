#include "GameSurface.hpp"
#include "vytAlgorithm.hpp"

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
			game::RenderLayer::getInstance().SetString({ ci,ri }, StaticCellImages[int(cellType)],
				game::ToRealColor(DEFAULT_FORE_COLOR), game::ToRealColor(DEFAULT_BACK_COLOR));
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
	WindowManager::Get()->SetColor(DEFAULT_COLOR);
	system("cls");
	WindowManager::Get()->SetPosition(x, y);
	cout << text;
	Sleep(millseconds);
	while (!IsKeyDown(VK_RETURN))
		continue;
	game::RenderLayer::getInstance().Clear();
}

void MsgSurface()
{
	DrawHollowBorder(GAME_MSG_S_INDEXX, GAME_MSG_E_INDEXX, GAME_MSG_S_INDEXY, GAME_MSG_E_INDEXY);
}

void OverSurface(bool isWin, bool hasNextLevel)
{
	std::string hint = "输入q退出游戏，输入r";
	if (isWin && hasNextLevel)
		hint += "继续下一关";
	else
		hint += "重新开始本关";
	DrawBorder(GAME_OVER_S_INDEXX, GAME_OVER_E_INDEXX, GAME_OVER_S_INDEXY, GAME_OVER_E_INDEXY);
	game::RenderLayer::getInstance().SetString({ GAME_OVER_S_INDEXX + 13, GAME_OVER_S_INDEXY + 4 },
		"闯关" + std::string(isWin ? "通过" : "失败"), game::ToRealColor(isWin ? E_4BitColor::LGreen : E_4BitColor::LRed), game::ToRealColor(DEFAULT_BACK_COLOR));
	game::RenderLayer::getInstance().SetString({ GAME_OVER_S_INDEXX + 8, GAME_OVER_S_INDEXY + 5 },
		hint, game::ToRealColor(DEFAULT_FORE_COLOR), game::ToRealColor(DEFAULT_BACK_COLOR));
}

void ShowMsg(const Msgs &msgs)
{
	int ri = GAME_MSG_S_INDEXY + 1;
	for (auto msg : msgs)
	{
		if (++ri >= (GAME_MSG_E_INDEXY - 1))
			break;
		int ci = GAME_MSG_S_INDEXX + (GAME_MSG_E_INDEXX - GAME_MSG_S_INDEXX - int(msg.size()) / 2) / 2;
		msg.Output({ ci,ri });
	}
}

void ShowMsg(const GameMap &map, bool isGamePause)
{
	const Player & player1 = map.GetPlayer(0);
	const Player & player2 = map.GetPlayer(1);
	static Msgs msgs;
	if (msgs.size() == 0)
		for (int i = 0; i < MSG_HEIGHT - 2; ++i)
			msgs.push_back(SurfaceText(""));
	msgs[0] = { "      关  卡: ", map.GetStage(), 0, ' ', "", game::ToRealColor(E_4BitColor::LYellow) };
	msgs[2] = { "剩余敌军数量:", map.RemainEnemyCount(), 3, ' ' };
	if (&player1 != &player2)
	{
		msgs[6] = { "玩家一", "玩家二" };
		msgs[8] = { "分数:", player1.getScore(), player2.getScore(), 3, '0', "", game::ToRealColor(E_4BitColor::LWhite) };
		msgs[10] = { "速度:", player1.TextSpeed(), player2.TextSpeed(), 3, '0', "", game::ToRealColor(E_4BitColor::Gray) };
		msgs[12] = { "生命:", player1.getLifeCount(), player2.getLifeCount(), 3, ' ', "", game::ToRealColor(E_4BitColor::LPurple) };
		msgs[14] = { "血量:", player1.getHealth(), player2.getHealth(), 3, ' ', "", game::ToRealColor(E_4BitColor::LRed) };
		msgs[23] = { "帮助", game::ToRealColor(E_4BitColor::LYellow) };
		msgs[25] = { "W,A,S,D,F操作玩家一", game::ToRealColor(E_4BitColor::LGreen) };
		msgs[27] = { "↑,↓,←,→,0操作玩家二", game::ToRealColor(E_4BitColor::LGreen) };
	}
	else
	{
		msgs[6] = { "玩家一" };
		msgs[8] = { "分数:", player1.getScore(), 3, '0', "", game::ToRealColor(E_4BitColor::LWhite) };
		msgs[10] = { "速度:", player1.TextSpeed(), 3, '0', "", game::ToRealColor(E_4BitColor::Gray) };
		msgs[12] = { "生命:", player1.getLifeCount(), 3, ' ', "", game::ToRealColor(E_4BitColor::LPurple) };
		msgs[14] = { "血量:", player1.getHealth(), 3, ' ', "", game::ToRealColor(E_4BitColor::LRed) };
		msgs[23] = { "帮助", game::ToRealColor(E_4BitColor::LYellow) };
		msgs[25] = { "W,A,S,D,F操作玩家一", game::ToRealColor(E_4BitColor::LGreen) };
		msgs[27] = { "                        ", game::ToRealColor(E_4BitColor::LGreen) };
	}
	static E_4BitColor pauseColor = E_4BitColor::LBlue;
	static clock_t twinkleTimer;
	msgs[29] = { isGamePause ? "空格键恢复游戏" : "空格键暂停游戏", game::ToRealColor(isGamePause ? pauseColor : E_4BitColor::LGreen) };
	msgs[31] = { isGamePause ? "Escape键保存并退出游戏" : "                      ", game::ToRealColor(isGamePause ? pauseColor : E_4BitColor::LGreen) };
	msgs[33] = { isGamePause ? "游戏已暂停" : "          ", game::ToRealColor(pauseColor) };
	if (isGamePause && clock() - twinkleTimer > 500)
	{
		twinkleTimer = clock();
		pauseColor = E_4BitColor(enumType(pauseColor) + 1);
		if (enumType(pauseColor) >= enumType(E_4BitColor::LWhite))
			pauseColor = E_4BitColor::LBlue;
	}
	ShowMsg(msgs);
}

void TextImageSurface::setPosition(const Vector2 & position)
{
	m_position = position;
}

void TextImageSurface::Render()
{
	auto position = getPosition();
	for (auto &lineImage : m_textImage)
	{
#ifdef _DEBUG
		if (lineImage.size() > m_emptyLine.size())
			throw std::invalid_argument("The image doesn't match rect");
		size_t lineWidth = lineImage.size() / 2 + lineImage.size() % 2;
		if (lineWidth + m_position.x >= LAYER_WIDTH)
			throw std::invalid_argument("The image out of window size");
#endif
		//CacheString(0, y++, lineImage);
		game::RenderLayer::getInstance().SetString(position, lineImage, game::ToRealColor(m_color.fore), game::ToRealColor(m_color.back));
		++position.y;
	}
}

void TextImageSurface::Abrase()
{
	for (auto position = getPosition(); position.y < int(m_textImage.size()) + getPosition().y; ++position.y)
		game::RenderLayer::getInstance().SetString(position, m_emptyLine, game::ToRealColor(m_color.fore), game::ToRealColor(m_color.back));
}

TextImageSurface::TextImageSurface(const Vector2 & position, const ConsoleColor &color, std::vector<std::string>&& textImage)
	: m_position(position), m_color(color), m_textImage(textImage)
{
	for (size_t i = 0; i < m_textImage[0].size(); ++i)
		m_emptyLine += " ";
	Render();
}

void TextImageSurface::MoveHorizontal(int xOffset)
{
	Abrase();
	m_positionOffset.x += xOffset;
	Render();
}

void TextImageSurface::MoveVertial(int yOffset)
{
	Abrase();
	m_positionOffset.y += yOffset;
	Render();
}

void TextImageSurface::Active()
{
	Render();
	//SetDrawActive(true);
}

void TextImageSurface::Deactive()
{
	Abrase();
	m_positionOffset = { 0, 0 };
	//SetDrawActive(false);
}

const SurfaceText HomeSurface::m_invalidSelect("←_←");
const SurfaceText HomeSurface::m_select("→_→");
const SurfaceText HomeSurface::m_empty("     ");
constexpr auto OptionY = 25;

HomeSurface::HomeSurface(bool isContinue) : m_isContinue(isContinue), m_currentOption(NewGame), m_position(0, 0),
	m_helloImage({LAYER_WIDTH/2-22, 6}, DEFAULT_COLOR, {
	"_|_|_|                _|      _|      _|                  _|_|_|  _|    _|                ",
	"_|    _|    _|_|_|  _|_|_|_|_|_|_|_|  _|    _|_|        _|            _|_|_|_|  _|    _|  ",
	"_|_|_|    _|    _|    _|      _|      _|  _|_|_|_|      _|        _|    _|      _|    _|  ",
	"_|    _|  _|    _|    _|      _|      _|  _|            _|        _|    _|      _|    _|  ",
	"_|_|_|      _|_|_|      _|_|    _|_|  _|    _|_|_|        _|_|_|  _|      _|_|    _|_|_|  ",
	"                                                                                      _|  ",
	"                                                                                  _|_|    ", 
	}), m_dropTimer(clock()), m_dropCount(0)
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
	if (m_dropCount < 5 && clock() - m_dropTimer > 500)
	{
		m_dropTimer = clock();
		++m_dropCount;
		m_helloImage.MoveVertial(1);
	}
	if (IsKeyDown(VK_RETURN) && !m_isInvalid)
		SetActive(false);
	bool anyChanged = false;
	if (IsKeyDown(VK_UP) || IsKeyDown('W'))
	{
		m_empty.Output({ 25,OptionY + vyt::IndexOfKey(m_options, m_currentOption) });
		m_currentOption = NewGame == m_currentOption ? Quit : E_HomeOption(m_currentOption - 1);
		if (Continue == m_currentOption && !m_isContinue)
			m_currentOption = E_HomeOption(m_currentOption - 1);
		anyChanged = true;
	}
	if (IsKeyDown(VK_DOWN) || IsKeyDown('S'))
	{
		m_empty.Output({ 25,OptionY + vyt::IndexOfKey(m_options, m_currentOption) });
		m_currentOption = Quit == m_currentOption ? NewGame : E_HomeOption(m_currentOption + 1);
		if (Continue == m_currentOption && !m_isContinue)
			m_currentOption = E_HomeOption(m_currentOption + 1);
		anyChanged = true;
	}
	if (IsKeyDown(VK_LEFT) || IsKeyDown('A'))
	{
		m_isInvalid = true;
		anyChanged = true;
	}
	if (IsKeyDown(VK_RIGHT) || IsKeyDown('D'))
	{
		m_isInvalid = false;
		anyChanged = true;
	}
	if (!anyChanged) return;
	if (m_isInvalid)
		m_invalidSelect.Output({ 25,OptionY + vyt::IndexOfKey(m_options, m_currentOption) });
	else
		m_select.Output({ 25,OptionY + vyt::IndexOfKey(m_options, m_currentOption) });
}

bool HomeSurface::IsActive() const
{
	return m_isActive;
}

void HomeSurface::SetActive(bool isActive)
{
	if (isActive && !IsActive())
	{
		DrawHollowBorder(0, LAYER_WIDTH - 1, 0, 3);
		DrawHollowBorder(0, LAYER_WIDTH - 1, 3, 40);
		SurfaceText name(GAME_NAME, game::ToRealColor(E_4BitColor::LCyan));
		name.Output({ 21, 1 });
		SurfaceText version(GAME_VERSION, game::ToRealColor(E_4BitColor::LWhite));
		version.Output({ 27, 2 });
		int i = 0;
		for (auto &option : m_options)
			option.second.Output({ 28, OptionY + i++ });
		m_select.Output({ 25,OptionY + vyt::IndexOfKey(m_options, m_currentOption) });
		m_helloImage.Active();
		m_dropCount = 0;
		m_dropTimer = clock();
	}
	else if (!isActive && IsActive())
	{
		m_helloImage.Deactive();
		game::RenderLayer::getInstance().Clear();
	}
	m_isActive = isActive;
}
