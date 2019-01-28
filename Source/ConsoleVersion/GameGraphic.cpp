#include "GameGraphic.hpp"

#include <iostream>

using namespace std;

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

game::RenderLayer game::RenderLayer::m_instance;

void game::RenderItem::Print(Vector2 position) const
{
	SetPosition(position.x, position.y);
	SetColor({ To4BitColor(m_foreColor), To4BitColor(m_backColor) });
	cout << m_text;
}

void game::RenderLayer::DrawCell(size_t x, size_t y, bool isForce)
{
	auto &item = m_items[x][y];
	if (!isForce && m_zCacheItems[x][y] == item) return;
	m_zCacheItems[x][y] = item;
	item.Print({ int(x), int(y) });
}

void game::RenderLayer::SetString(const Vector2 & position, const string & text, const RenderColor & color)
{
	size_t index = 0;
	while (index < text.size())
	{
		m_items[position.x + index/2][position.y] = text.substr(index, 2);
		m_items[position.x + index/2][position.y] = color;
		index += 2;
	}
}

void game::RenderLayer::Draw()
{
	Draw(false);
}

void game::RenderLayer::Draw(bool isForce)
{
	for (size_t x = 0; x < LAYER_WIDTH; ++x)
		for (size_t y = 0; y < LAYER_HEIGHT; ++y)
			DrawCell(x, y, isForce);
}
