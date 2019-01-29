#ifndef GAME_COLLIDER_HPP_INCLUDED
#define GAME_COLLIDER_HPP_INCLUDED

#include "GameMath.hpp"

#include <string>
#include <functional>

namespace game
{
	class Collider
	{
		bool m_isActive;
	protected:
		bool getColliderActive() const { return m_isActive; }
		void setColliderActive(bool isActive);
	protected:
		Collider(bool isActive);
		virtual ~Collider();

		virtual void OnCollision(Collider &collider) = NULL;
		virtual bool Contains(const Vector2 &position) = NULL;
		virtual bool SetPositionByIndex(size_t index, Vector2 &point) = NULL;
	public:
		virtual const std::string& getType() const = NULL;
		void SimulateMove();
	};
}

#endif