#ifndef GAME_SURFACE_HPP
#define GAME_SURFACE_HPP

#include "winapi.hpp"
#include "GameGraphic.hpp"
#include "GameMap.hpp"
#include "GamePlayer.hpp"

#include <vector>
#include <map>
#include <string>
#include <sstream>
using std::string;

class SurfaceText
{
	string m_text;
	RenderColor m_foreColor = game::ToRealColor(DEFAULT_FORE_COLOR);
	RenderColor m_backColor = game::ToRealColor(DEFAULT_BACK_COLOR);
	const string PLAYER_SPACE = "        ";
public:
	SurfaceText(const SurfaceText &st) : m_text(st.m_text), m_foreColor(st.m_foreColor), m_backColor(st.m_backColor)
	{

	}
	SurfaceText& operator=(const SurfaceText &st)
	{
		m_text = st.m_text;
		m_foreColor = st.m_foreColor;
		m_backColor = st.m_backColor;
		return *this;
	}
	SurfaceText(string text, RenderColor color = game::ToRealColor(DEFAULT_FORE_COLOR)) : m_text(text), m_foreColor(color) { }
	SurfaceText(string player1, string player2, RenderColor color = game::ToRealColor(DEFAULT_FORE_COLOR)) : m_foreColor(color)
	{
		std::ostringstream oss;
		oss << player1 << PLAYER_SPACE << player2;
		m_text = oss.str();
	}
	template <typename T>
	SurfaceText(string prefix, T player, std::streamsize width, char fill, string suffix = "", RenderColor color = game::ToRealColor(DEFAULT_FORE_COLOR))
		: m_foreColor(color)
	{
		std::ostringstream oss;
		oss << prefix << std::setw(width) << std::setfill(fill) << player << suffix;
		m_text = oss.str();
	}
	template <typename T>
	SurfaceText(string prefix, T player1, T player2, std::streamsize width, char fill, string suffix = "", RenderColor color = game::ToRealColor(DEFAULT_FORE_COLOR))
		: m_foreColor(color)
	{
		std::ostringstream oss;
		oss << prefix << std::setw(width) << std::setfill(fill) << player1 << suffix
			<< PLAYER_SPACE << prefix << std::setw(width) << std::setfill(fill) << player2 << suffix;
		m_text = oss.str();
	}
	const std::string& ToString() const { return m_text; }
	size_t size()const { return m_text.size(); }
	friend std::ostream& operator<<(std::ostream &os, const SurfaceText& text)
	{
		os << text.m_text;
		return os;
	}
	void Output(const Vector2 &position) const
	{
		game::RenderLayer::getInstance().SetString(position, m_text, m_foreColor, m_backColor);
	}
};

using Msgs = std::vector<SurfaceText>;

class HomeSurface : public game::Renderer
{
public:
	enum E_HomeOption { NewGame, Continue, Setting, Editor, Quit };
private:
	bool m_isContinue;
	E_HomeOption m_currentOption;
	Vector2 m_position;
	static const SurfaceText m_select;
	static const SurfaceText m_empty;
	std::vector<E_HomeOption> m_options;
public:
	E_HomeOption get_Option() const { return m_currentOption; }
	const Vector2& getPosition() const { return m_position; }
public:
	HomeSurface(bool isContinue = false);
	void Update();
	bool IsActive() const;
	void SetActive(bool isActive);
};

void DrawBorder(int posXS, int posXE, int posYS, int posYE);
void DrawHollowBorder(int posXS, int posXE, int posYS, int posYE);
void UnfinishedSurface(int x, int y, DWORD millseconds, string text);
void OverSurface(const Player &winer, bool isWin);

void ShowMsg(Msgs &&msgs);

void ShowMsg(const Player &player1, const Player &player2);

#endif