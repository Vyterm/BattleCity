#ifndef GAME_DRAWABLE_HPP_INCLUDED
#define GAME_DRAWABLE_HPP_INCLUDED

#include "GameGraphic.hpp"

namespace game
{
	class Renderer
	{
	protected:
		const size_t m_width;
		const size_t m_height;
		virtual const Vector2& getPosition() const = NULL;
		void SetDrawActive(bool isActive) { m_isDrawActive = isActive; }
		bool GetDrawActive() const { return m_isDrawActive; }
	private:
		RenderModel *m_items;
		bool m_isDrawActive;
	protected:
		Renderer(size_t width, size_t height)
			: m_width(width), m_height(height), m_isDrawActive(true), m_items(new RenderModel[width*height])
		{
			RenderLayer::getInstance().m_renders.push_back(this);
		}
		virtual ~Renderer()
		{
			auto &drawables = RenderLayer::getInstance().m_renders;
			for (auto iter = drawables.begin(); iter != drawables.end(); ++iter)
			{
				if (*iter == this)
				{
					drawables.erase(iter);
					break;
				}
			}
			delete[] m_items;
		}
		void CacheString(size_t x, size_t y, const string &text)
		{
			auto startPos = x + y * m_width;
			auto textLength = text.size() / 2 + text.size() % 2;
#ifdef _DEBUG
			if ((startPos + textLength) > m_width * m_height)
				throw std::out_of_range("Drawable item index out of range");
#endif
			for (size_t index = 0; index < textLength; ++index)
				m_items[startPos++] = text.substr(index * 2, 2);
		}
		void CacheString(size_t x, size_t y, const string &text, const ConsoleColor &color)
		{
			CacheString(x, y, text);
			auto startPos = x + y * m_width;
			auto textLength = text.size() / 2 + text.size() % 2;
			for (size_t index = 0; index < textLength; ++index)
				m_items[startPos++].Set(ToRealColor(color.fore), ToRealColor(color.back));
		}
	public:
		void RenderToLayer()
		{
			if (!m_isDrawActive) return;
			auto &renderLayer = RenderLayer::getInstance();
			Vector2 position = getPosition();
			for (size_t i = 0; i < m_width*m_height; ++i)
			{
				++position.x; if (i!=0 && i%m_width == 0) { position.x -= m_width; ++position.y; }
				if (renderLayer.GetItem(position) != m_items[i])
					renderLayer.SetItem(position, m_items[i]);
			}
		}
	};
}

#endif // !GAME_DRAWABLE_HPP_INCLUDED
