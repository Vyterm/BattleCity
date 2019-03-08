#ifndef GAME_DRAWABLE_HPP_INCLUDED
#define GAME_DRAWABLE_HPP_INCLUDED

#include "GameGraphic.hpp"

namespace game
{
	class Renderer
	{
	public:
		static void ApplyRenderers();
	protected:
		const size_t m_width;
		const size_t m_height;
		virtual const Vector2& getPosition() const = NULL;
		void SetDrawActive(bool isActive);
		bool GetDrawActive() const;
	private:
		RenderModel *m_items;
		RenderType m_type;
		Vector2 m_renderPosition;
		bool m_isDrawActive;
	private:
		Vector2 IndexToXY(size_t index);
		void CacheString(size_t index, const string &text, const RenderColor &foreColor, const RenderColor &backColor);
		void RenderToLayer(size_t index);
		void RenderEmptyToLayer();
		void RenderItemsToLayer();
	protected:
		Renderer(size_t width, size_t height, RenderType type, bool isActive = true);
		virtual ~Renderer();
		void CacheString(size_t x, size_t y, const string &text);
		void CacheString(size_t x, size_t y, const string &text, const ConsoleColor &color);
		void ClearCache();
	public:
		void RenderToLayer();
	};
}

#endif
