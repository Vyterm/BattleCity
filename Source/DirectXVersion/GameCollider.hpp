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
		Collider();
		virtual ~Collider();

		virtual void OnCollision(Collider &collider) = NULL;
		virtual bool Contains(Collider &collider) = NULL;
	public:
		// The return value of this method indicates whether it contains minimum rectangle with (params:position) as upper-left coordinate point
		virtual bool Contains(const Vector2 &position) = NULL;
		virtual const std::string& getType() const = NULL;
		// This method only invoke this->OnCollision;
		void TestStrikeToActiveCollider();
		void StrikeToActiveColliders();
	};
}

#endif