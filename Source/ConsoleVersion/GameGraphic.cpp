#include "GameGraphic.hpp"
#include "GameRender.hpp"
#include "winapi.hpp"

#include <iostream>
#include <cstdlib>

using namespace std;

namespace game
{
	E_4BitColor game::To4BitColor(const RenderColor & color)
	{
		E_4BitColor color4bit = E_4BitColor::Black;
		if (color.get_red() != 0)
			color4bit = color4bit | E_4BitColor::Red;
		if (color.get_green() != 0)
			color4bit = color4bit | E_4BitColor::Green;
		if (color.get_blue() != 0)
			color4bit = color4bit | E_4BitColor::Blue;
		if (color.get_alpha() == 2)
			color4bit = color4bit | E_4BitColor::Gray;
		return color4bit;
	}

	RenderColor game::ToRealColor(const E_4BitColor & color)
	{
		return RenderColor(
			((color & E_4BitColor::Red) == E_4BitColor::Red) ? 1 : 0,
			((color & E_4BitColor::Green) == E_4BitColor::Green) ? 1 : 0,
			((color & E_4BitColor::Blue) == E_4BitColor::Blue) ? 1 : 0,
			((color & E_4BitColor::Gray) == E_4BitColor::Gray) ? 2 : 0
		);
	}

	const RenderModel RenderModel::Empty = {};
	const RenderModel RenderModel::Rendered = { "  ", ToRealColor(DEFAULT_FORE_COLOR), ToRealColor(DEFAULT_BACK_COLOR) };

	void game::RenderModel::Print(Vector2 position) const
	{
		SetPosition(position.x, position.y);
		SetColor({ To4BitColor(m_foreColor), To4BitColor(m_backColor) });
		cout << m_text;
	}

	game::RenderLayer* game::RenderLayer::m_instance = new RenderLayer();

	RenderLayer & RenderLayer::getInstance()
	{
		if (nullptr == m_instance)
			m_instance = new RenderLayer();
		return *m_instance;
	}

	RenderLayer::RenderLayer() : m_afreshRenderPoints(LAYER_WIDTH * LAYER_HEIGHT)
	{
		m_zCacheItems = new ModelLayer();
		m_layers = new CacheLayer();
		for (int x = 0; x < LAYER_WIDTH; ++x)
			for (int y = 0; y < LAYER_HEIGHT; ++y)
				m_layers->Item({ x,y }).setPosition({ x,y });
	}

	RenderLayer::~RenderLayer()
	{
		delete m_zCacheItems;
		delete m_layers;
	}

	void game::RenderLayer::DrawCell(Vector2 position, bool isForce)
	{
		auto &cacheItem = m_zCacheItems->Item(position);
		if (!m_layers->Item(position).RenderMix(cacheItem) && !isForce) return;
		cacheItem.Print(position);
	}

	void game::RenderLayer::Draw()
	{
		Draw(false);
	}

	void game::RenderLayer::Draw(bool isForce)
	{
		Renderer::ApplyRenderers();
		for (auto &point : m_afreshRenderPoints)
			DrawCell(point, isForce);
		m_afreshRenderPoints.clear();
		if (!isForce) return;
		for (size_t x = 0; x < LAYER_WIDTH; ++x)
			for (size_t y = 0; y < LAYER_HEIGHT; ++y)
				DrawCell({ int(x), int(y) }, isForce);
	}

	void RenderLayer::Clear()
	{
		SetColor(DEFAULT_COLOR);
		system("cls");
		for (size_t x = 0; x < LAYER_WIDTH; ++x)
			for (size_t y = 0; y < LAYER_HEIGHT; ++y)
			{
				Vector2 pos = { int(x), int(y) };
				m_zCacheItems->SetItem(pos, RenderModel::Rendered);
				m_layers->Item(pos).ClearItem();
			}
	}

	void game::RenderLayer::SetString(const Vector2 & position, const string & text, const RenderColor &foreColor, const RenderColor &backColor)
	{
		size_t index = 0;
		while (index < text.size())
		{
			Vector2 pos = { position.x + int(index / 2), position.y };
			auto item = m_layers->Item(pos).GetItem(RenderType::UICanvas);
			item = text.substr(index, 2);
			item.Set(foreColor, backColor);
			m_layers->Item(pos).SetItem(RenderType::UICanvas, item);
			index += 2;
		}
	}

	const RenderModel & game::RenderLayer::GetItem(RenderType layer, const Vector2 & position)
	{
		return m_layers->Item(position).GetItem(layer);
	}

	void RenderLayer::SetItem(RenderType layer, const Vector2 & position, const RenderModel & model)
	{
		m_layers->Item(position).SetItem(layer, model);
	}

	CacheMix::CacheMix()
	{
		ClearItem();
	}

	void CacheMix::MixItem()
	{
		const RenderModel* item;
		for (size_t layer = size_t(RenderType::StaticLayer0); layer <= size_t(RenderType::UICanvas); ++layer)
		{
			item = &(m_renderModels[static_cast<RenderType::E_Layer>(layer)]);
			if (EMPTY_MODEL_TEXT == item->getText()) continue;
			m_cacheMix = item->getText();
			if (layer == size_t(RenderType::StaticLayer0) || layer == size_t(RenderType::StaticLayer1))
				m_cacheMix.Set(item->getForeColor(), item->getBackColor());
			else if (layer == size_t(RenderType::ActiveLayer0) || layer == size_t(RenderType::ActiveLayer1))
				m_cacheMix.Set(item->getForeColor(), m_cacheMix.getBackColor());
			else
				m_cacheMix.Set(item->getForeColor(), item->getBackColor());
		}
	}

	void CacheMix::SetItem(RenderType layer, const RenderModel & model)
	{
		// 防止Map把自己没有画的点擦了然后上层的图形就漏下来了
		if (layer == RenderType::StaticLayer0 && model.getText() == EMPTY_MODEL_TEXT)
			return;
		if (m_renderModels[layer] == model) return;
		m_renderModels[layer] = model;
		MixItem();
		RenderLayer::m_instance->m_afreshRenderPoints.push_back(m_position);
	}

	bool CacheMix::RenderMix(RenderModel & target)
	{
		if (m_cacheMix == target) return false;
		target = m_cacheMix;
		return true;
	}

	const RenderModel & CacheMix::GetItem(RenderType layer)
	{
		return m_renderModels[layer];
	}

	void CacheMix::ClearItem()
	{
		m_cacheMix = RenderModel::Rendered;
		m_renderModels[RenderType::StaticLayer0] = RenderModel::Rendered;
		m_renderModels[RenderType::StaticLayer1] = RenderModel::Empty;
		m_renderModels[RenderType::ActiveLayer0] = RenderModel::Empty;
		m_renderModels[RenderType::ActiveLayer1] = RenderModel::Empty;
		m_renderModels[RenderType::UICanvas] = RenderModel::Empty;
	}

}
