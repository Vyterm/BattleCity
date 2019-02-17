#include "GameRender.hpp"

#include <vector>

namespace game
{
#pragma region Renderer Manager

	static std::vector<Renderer*> m_renders;
	inline static void AppendRenderer(Renderer * renderer)
	{
		m_renders.push_back(renderer);
	}

	inline static void RemoveRenderer(Renderer * renderer)
	{
		for (auto iter = m_renders.begin(); iter != m_renders.end(); ++iter)
		{
			if (*iter != renderer) continue;
			m_renders.erase(iter);
			return;
		}
	}

	void Renderer::ApplyRenderers()
	{
		for (auto&render : m_renders)
			render->RenderToLayer();
	}

#pragma endregion

	Renderer::Renderer(size_t width, size_t height, RenderType type, bool isActive)
		: m_width(width), m_height(height), m_isDrawActive(isActive), m_renderPosition(0, 0), m_items(new RenderModel[width*height]), m_type(type)
	{
		AppendRenderer(this);
	}

	Renderer::~Renderer()
	{
		RemoveRenderer(this);
		RenderEmptyToLayer();
		delete[] m_items;
	}


	void Renderer::SetDrawActive(bool isActive) { m_isDrawActive = isActive; if (m_isDrawActive) m_isAnyChange = true; else RenderEmptyToLayer(); }

	bool Renderer::GetDrawActive() const { return m_isDrawActive; }

	void Renderer::CacheString(size_t x, size_t y, const string & text)
	{
		auto startPos = x + y * m_width;
		auto textLength = text.size() / 2 + text.size() % 2;
#ifdef _DEBUG
		if ((startPos + textLength) > m_width * m_height)
			throw std::out_of_range("Drawable item index out of range");
#endif
		for (size_t index = 0; index < textLength; ++index)
			m_items[startPos++] = text.substr(index * 2, 2);
		m_isAnyChange = true;
	}

	void Renderer::CacheString(size_t x, size_t y, const string & text, const ConsoleColor & color)
	{
		CacheString(x, y, text);
		auto startPos = x + y * m_width;
		auto textLength = text.size() / 2 + text.size() % 2;
		for (size_t index = 0; index < textLength; ++index)
			m_items[startPos++].Set(ToRealColor(color.fore), ToRealColor(color.back));
		m_isAnyChange = true;
	}

	void Renderer::ClearCache()
	{
		for (size_t i = 0; i < m_width*m_height; ++i)
			m_items[i] = RenderModel::Empty;
		m_isAnyChange = true;
	}

	void Renderer::RenderEmptyToLayer()
	{
		Vector2 position = m_renderPosition;
		auto &renderLayer = RenderLayer::getInstance();
		for (size_t i = 0; i < m_width*m_height; ++i, ++position.x)
		{
			if (i != 0 && i%m_width == 0) { position.x -= m_width; ++position.y; }
			renderLayer.SetItem(m_type, position, RenderModel::Empty);
		}
	}

	void Renderer::RenderToLayer(Vector2 position)
	{
		auto &renderLayer = RenderLayer::getInstance();
		for (size_t i = 0; i < m_width*m_height; ++i, ++position.x)
		{
			if (i != 0 && i%m_width == 0) { position.x -= m_width; ++position.y; }
			if (renderLayer.GetItem(m_type, position) != m_items[i])
				renderLayer.SetItem(m_type, position, m_items[i]);
		}
	}

	void Renderer::RenderToLayer()
	{
		if (!m_isDrawActive) return;
		bool isPositionChanged = m_renderPosition != getPosition();
		if (!isPositionChanged && !m_isAnyChange) return;
		if (isPositionChanged)
			RenderEmptyToLayer();
		m_renderPosition = getPosition();
		RenderToLayer(m_renderPosition);
		m_isAnyChange = false;
	}
}