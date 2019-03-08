#ifndef GAME_BOX_COLLIDER_HPP_INCLUDED
#define GAME_BOX_COLLIDER_HPP_INCLUDED

#include "GameCollider.hpp"

namespace game
{
	class BoxCollider : public Collider
	{
	protected:
		struct Rect { int left, top, right, bottom; };
	private:
		Rect m_rect;
	protected:
		virtual const Vector2& getPosition() const = NULL;
	protected:
		BoxCollider(int width, int height) { SetRect(width, height); }
		BoxCollider(const Rect &rect) : m_rect(rect) { }
		void SetRect(int width, int height) { m_rect = { 0, 0, width, height }; }
		void SetRect(const Rect &rect) { m_rect = rect; }
		bool Contains(Collider &collider) override final;
	public:
		bool Contains(const Vector2 &position) override final;
	};
}

#endif