#ifndef GAME_GRAPHIC_HPP_INCLUDED
#define GAME_GRAPHIC_HPP_INCLUDED

#include "GameDefines.hpp"
#include "GameLayer.hpp"
#include "winapi.hpp"

#include <iostream>
#include <string>
#include <set>
#include <map>

using std::string;
constexpr auto EMPTY_MODEL_TEXT = "";

namespace game
{
	E_4BitColor To4BitColor(const RenderColor &color);
	RenderColor ToRealColor(const E_4BitColor &color);
	class RenderModel
	{
	public:
		static const RenderModel Empty;
		static const RenderModel Rendered;
	private:
		string m_text;
		RenderColor m_foreColor;
		RenderColor m_backColor;
	public:
		const string& getText() const { return m_text; }
		const RenderColor& getForeColor() const { return m_foreColor; }
		const RenderColor& getBackColor() const { return m_backColor; }
	public:
		RenderModel() : m_text(EMPTY_MODEL_TEXT), m_foreColor(ToRealColor(DEFAULT_FORE_COLOR)), m_backColor(ToRealColor(DEFAULT_BACK_COLOR)) { }
		RenderModel(string text, RenderColor fore, RenderColor back) : m_text(text), m_foreColor(fore), m_backColor(back) { }
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
		bool operator==(const E_Layer &layer) const { return m_layer == layer; }
		bool operator!=(const E_Layer &layer) const { return m_layer != layer; }
	};

	class CacheMix
	{
		Vector2 m_position;
		RenderModel m_cacheMix;
		std::map<RenderType, RenderModel> m_renderModels;
	public:
		void setPosition(const Vector2 &position) { m_position = position; }
	private:
		void MixItem();
	public:
		CacheMix();

		void SetItem(RenderType layer, const RenderModel &model);
		const RenderModel& GetItem(RenderType layer);
		void ClearItem();

		bool RenderMix(RenderModel &target);
	};

	class RenderLayer
	{
	private:
		using ModelLayer = layer<RenderModel, LAYER_WIDTH, LAYER_HEIGHT>;
		using CacheLayer = layer<CacheMix, LAYER_WIDTH, LAYER_HEIGHT>;
		using PModelLayer = ModelLayer*;
		using PCacheLayer = CacheLayer*;
		RenderLayer();
		~RenderLayer();
		static RenderLayer* m_instance;
	public:
		static RenderLayer& getInstance();
	private:
		PModelLayer m_zCacheItems;
		PCacheLayer m_layers;

		std::set<Vector2> m_afreshRenderPoints;
		friend CacheMix;

		void DrawCell(Vector2 position, bool isForce);
	public:
		void SetString(const Vector2 & position, const string & text, const RenderColor &foreColor, const RenderColor &backColor);
		void Draw();
		void Draw(bool isForce);
		void Clear();
		const RenderModel& GetItem(RenderType layer, const Vector2 &position);
		void SetItem(RenderType layer, const Vector2 &position, const RenderModel &model);
	};
}

#endif