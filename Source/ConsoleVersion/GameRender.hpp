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
		void SetDrawActive(bool isActive) { m_isDrawActive = isActive; }
		bool GetDrawActive() const { return m_isDrawActive; }
	private:
		RenderModel *m_items;
		bool m_isDrawActive;
	protected:
		Renderer(size_t width, size_t height);
		virtual ~Renderer();
		void CacheString(size_t x, size_t y, const string &text);
		void CacheString(size_t x, size_t y, const string &text, const ConsoleColor &color);
	public:
		void RenderToLayer();
	};
}

#endif // !GAME_DRAWABLE_HPP_INCLUDED
