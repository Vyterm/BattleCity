#include "GameRender.hpp"
#include "vytAlgorithm.hpp"

#include <vector>

namespace game
{
#pragma region Renderer Manager

	static std::vector<Renderer*> m_renders;
	inline static void AppendRenderer(Renderer * renderer)
	{
		vyt::Append(m_renders, renderer);
	}

	inline static void RemoveRenderer(Renderer * renderer)
	{
		vyt::Remove(m_renders, renderer);
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
		if (m_type == RenderType::DymanicLayer0 || m_type == RenderType::DymanicLayer1)
			AppendRenderer(this);
	}

	Renderer::~Renderer()
	{
		if (m_type == RenderType::DymanicLayer0 || m_type == RenderType::DymanicLayer1)
			RemoveRenderer(this);
		RenderEmptyToLayer();
		delete[] m_items;
	}


	void Renderer::SetDrawActive(bool isActive)
	{
		if (m_isDrawActive == isActive) return;
		m_isDrawActive = isActive;
		if (m_isDrawActive)
			RenderItemsToLayer();
		else
			RenderEmptyToLayer();
	}

	bool Renderer::GetDrawActive() const { return m_isDrawActive; }

	void Renderer::CacheString(size_t x, size_t y, const string & text)
	{
		auto index = x + y * m_width;
		CacheString(index, text, m_items[index].getForeColor(), m_items[index].getBackColor());
	}

	void Renderer::CacheString(size_t x, size_t y, const string & text, const ConsoleColor & color)
	{
		auto index = x + y * m_width;
		CacheString(index, text, ToRealColor(color.fore), ToRealColor(color.back));
	}

	Vector2 Renderer::IndexToXY(size_t index)
	{
		return Vector2(m_renderPosition.x + int(index%m_width), m_renderPosition.y + int(index / m_width));
	}

	void Renderer::CacheString(size_t startPos, const string & text, const RenderColor & foreColor, const RenderColor & backColor)
	{
		auto textLength = text.size() / 2 + text.size() % 2;
#ifdef _DEBUG
		if ((startPos + textLength) > m_width * m_height)
			throw std::out_of_range("Drawable item index out of range");
#endif
		for (size_t index = 0; index < textLength; ++index)
		{
			m_items[startPos + index] = text.substr(index * 2, 2);
			m_items[startPos + index].Set(foreColor, backColor);
			if (m_renderPosition == getPosition() || m_type != RenderType::DymanicLayer0 && m_type != RenderType::DymanicLayer1)
				RenderToLayer(startPos + index);
		}
	}

	void Renderer::ClearCache()
	{
		for (size_t i = 0; i < m_width*m_height; ++i)
			m_items[i] = RenderModel::Empty;
		RenderEmptyToLayer();
	}

	void Renderer::RenderToLayer(size_t index)
	{
		m_renderPosition = getPosition();
		Vector2 position = IndexToXY(index);
		RenderLayer::getInstance().SetItem(m_type, position, m_items[index]);
	}

	void Renderer::RenderEmptyToLayer()
	{
		auto &renderLayer = RenderLayer::getInstance();
		for (size_t i = 0; i < m_width*m_height; ++i)
			renderLayer.SetItem(m_type, IndexToXY(i), RenderModel::Empty);
	}

	void Renderer::RenderItemsToLayer()
	{
		for (size_t i = 0; i < m_width*m_height; ++i)
			RenderToLayer(i);
	}

	void Renderer::RenderToLayer()
	{
		if (!m_isDrawActive || m_renderPosition == getPosition()) return;
		RenderEmptyToLayer();
		RenderItemsToLayer();
	}
}