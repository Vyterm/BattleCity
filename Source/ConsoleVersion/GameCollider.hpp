#ifndef GAME_COLLIDER_HPP_INCLUDED
#define GAME_COLLIDER_HPP_INCLUDED

#include "GameMath.hpp"

#include <string>
#include <functional>

namespace game
{
	class Collider
	{
		std::string m_type;
		bool m_isActive;
	protected:
		bool getColliderActive() const { return m_isActive; }
		void setColliderActive(bool isActive);
	protected:
		Collider(std::string type, bool isActive);
		virtual ~Collider();

		virtual void OnCollision(Collider &collider) = NULL;
		virtual bool Contains(const Vector2 &position) = NULL;
		virtual bool SetPositionByIndex(size_t index, Vector2 &point) = NULL;
	public:
		const std::string& getType() const { return m_type; }
		void SimulateMove();
	};
}

#endif