#ifndef GAME_GRAPHIC_HPP_INCLUDED
#define GAME_GRAPHIC_HPP_INCLUDED

#include "GameDefines.hpp"
#include "GameLayer.hpp"
#include "winapi.hpp"

#include <iostream>
#include <string>
#include <vector>

using std::string;

namespace game
{
	E_4BitColor To4BitColor(const RenderColor &color);
	RenderColor ToRealColor(const E_4BitColor &color);
	class RenderModel
	{
	private:
		string m_text;
		RenderColor m_foreColor;
		RenderColor m_backColor;
	public:
		RenderModel() : m_text("  "), m_foreColor(ToRealColor(DEFAULT_FORE_COLOR)), m_backColor(ToRealColor(DEFAULT_BACK_COLOR)) { }
		RenderModel& operator=(const string &text) { m_text = text; return *this; }
		void Set(RenderColor foreColor, RenderColor backColor) { m_foreColor = foreColor; m_backColor = backColor; }
		bool operator==(const RenderModel& rhs) const
		{
			return m_text == rhs.m_text &&
				m_foreColor == rhs.m_foreColor &&
				m_backColor == rhs.m_backColor;
		}
		bool operator!=(const RenderModel& rhs) const
		{
			return m_text != rhs.m_text ||
				m_foreColor != rhs.m_foreColor ||
				m_backColor != rhs.m_backColor;
		}
		void Print(Vector2 position) const;
	};

	class Renderer;
	class RenderLayer : layer<RenderModel, GAME_WIDTH+MAZE_WIDTH, GAME_HEIGHT>
	{
	private:
		RenderLayer() { }
		static RenderLayer m_instance;
	public:
		static RenderLayer& getInstance() { return m_instance; }
	private:
		RenderModel m_zCacheItems[LAYER_WIDTH][LAYER_HEIGHT];
	//public:
		void DrawCell(size_t x, size_t y, bool isForce);
	public:
		void SetString(const Vector2 &position, const string &text, const RenderColor &foreColor, const RenderColor &backColor);
		void Draw();
		void Draw(bool isForce);
		friend Renderer;
	private:
		std::vector<Renderer*> m_renders;
	};
}

#endif