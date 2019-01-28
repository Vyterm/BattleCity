#include "GameGraphic.hpp"
#include "GameRender.hpp"

#include <iostream>

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

	RenderLayer::RenderLayer()
	{
		m_zCacheItems = new ModelLayer();
		m_layers[RenderType::StaticLayer0] = new ModelLayer();
		m_layers[RenderType::ActiveLayer0] = new ModelLayer();
		m_layers[RenderType::StaticLayer1] = new ModelLayer();
		m_layers[RenderType::ActiveLayer1] = new ModelLayer();
		m_layers[RenderType::UICanvas] = new ModelLayer();
	}

	RenderLayer::~RenderLayer()
	{
		delete m_zCacheItems;
		delete m_layers[RenderType::StaticLayer0];
		delete m_layers[RenderType::ActiveLayer0];
		delete m_layers[RenderType::StaticLayer1];
		delete m_layers[RenderType::ActiveLayer1];
		delete m_layers[RenderType::UICanvas];
	}

	game::RenderModel game::RenderLayer::MixCell(size_t x, size_t y)
	{
		Vector2 position = { int(x), int(y) };
		RenderModel model;
		const RenderModel* item;
		for (size_t layer = size_t(RenderType::StaticLayer0); layer < size_t(RenderType::UICanvas); ++layer)
		{
			item = &(m_layers[static_cast<RenderType::E_Layer>(layer)]->GetItem(position));
			if ("  " == item->getText()) continue;
			model = item->getText();
			if (layer == size_t(RenderType::StaticLayer0) || layer == size_t(RenderType::StaticLayer1))
				model.Set(model.getForeColor(), item->getBackColor());
			else if (layer == size_t(RenderType::ActiveLayer0) || layer == size_t(RenderType::ActiveLayer1))
				model.Set(item->getForeColor(), model.getBackColor());
			else
				model.Set(item->getForeColor(), item->getBackColor());
		}

		return model;
	}

	void game::RenderLayer::SetString(const Vector2 & position, const string & text, const RenderColor &foreColor, const RenderColor &backColor)
	{
		size_t index = 0;
		while (index < text.size())
		{
			Vector2 pos = { position.x + int(index / 2), position.y };
			auto item = m_layers[RenderType::UICanvas]->GetItem(pos);
			item = text.substr(index, 2);
			item.Set(foreColor, backColor);
			m_layers[RenderType::UICanvas]->SetItem(pos, item);
			index += 2;
		}
	}

	void game::RenderLayer::DrawCell(size_t x, size_t y, bool isForce)
	{
		auto item = MixCell(x, y);
		Vector2 position = { int(x), int(y) };
		if (!isForce && m_zCacheItems->GetItem(position) == item) return;
		m_zCacheItems->SetItem(position, item);
		item.Print({ int(x), int(y) });
	}

	void game::RenderLayer::Draw()
	{
		Draw(false);
	}

	void game::RenderLayer::Draw(bool isForce)
	{
		Renderer::ApplyRenderers();
		for (size_t x = 0; x < LAYER_WIDTH; ++x)
			for (size_t y = 0; y < LAYER_HEIGHT; ++y)
				DrawCell(x, y, isForce);
	}

	const RenderModel & game::RenderLayer::GetItem(RenderType layer, const Vector2 & position)
	{
		return m_layers[layer]->GetItem(position);
	}

	void RenderLayer::SetItem(RenderType layer, const Vector2 & position, const RenderModel & model)
	{
		return m_layers[layer]->SetItem(position, model);
	}

}
