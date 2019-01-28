#ifndef GAME_GRAPHIC_HPP_INCLUDED
#define GAME_GRAPHIC_HPP_INCLUDED

#include "GameDefines.hpp"
#include "GameLayer.hpp"
#include "winapi.hpp"

#include <iostream>
#include <string>
#include <map>


constexpr auto LAYER_WIDTH = GAME_WIDTH + MAZE_WIDTH;
constexpr auto LAYER_HEIGHT = GAME_HEIGHT;
using std::string;

namespace game
{
	E_4BitColor To4BitColor(const RenderColor &color);
	RenderColor ToRealColor(const E_4BitColor &color);
	class RenderModel
	{
	public:
		static const RenderModel Empty;
	private:
		string m_text;
		RenderColor m_foreColor;
		RenderColor m_backColor;
	public:
		const string& getText() const { return m_text; }
		const RenderColor& getForeColor() const { return m_foreColor; }
		const RenderColor& getBackColor() const { return m_backColor; }
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

	struct RenderType
	{
	public:
		enum E_Layer { StaticLayer0, ActiveLayer0, StaticLayer1, ActiveLayer1, UICanvas, };
	private:
		E_Layer m_layer;
	public:
		RenderType(E_Layer layer) : m_layer(layer) { }
		bool operator<(const RenderType &rhs) const { return m_layer < rhs.m_layer; }
		bool operator<=(const RenderType &rhs) const { return m_layer <= rhs.m_layer; }
		bool operator>(const RenderType &rhs) const { return m_layer > rhs.m_layer; }
		bool operator>=(const RenderType &rhs) const { return m_layer >= rhs.m_layer; }
		bool operator==(const RenderType &rhs) const { return m_layer == rhs.m_layer; }
		bool operator!=(const RenderType &rhs) const { return m_layer != rhs.m_layer; }
	};

	class RenderLayer
	{
	private:
		using ModelLayer = layer<RenderModel, LAYER_WIDTH, LAYER_HEIGHT>;
		using PModelLayer = ModelLayer*;
		RenderLayer();
		~RenderLayer();
		static RenderLayer* m_instance;
	public:
		static RenderLayer& getInstance();
	private:
		PModelLayer m_zCacheItems;
		std::map<RenderType, PModelLayer> m_layers;

		RenderModel MixCell(size_t x, size_t y);
		void DrawCell(size_t x, size_t y, bool isForce);
	public:
		void SetString(const Vector2 & position, const string & text, const RenderColor &foreColor, const RenderColor &backColor);
		void Draw();
		void Draw(bool isForce);
		const RenderModel& GetItem(RenderType layer, const Vector2 &position);
		void SetItem(RenderType layer, const Vector2 &position, const RenderModel &model);
	};
}

#endif