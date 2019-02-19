#ifndef GAME_MESH_COLLIDER_HPP_INCLUDED
#define GAME_MESH_COLLIDER_HPP_INCLUDED

#include "GameCollider.hpp"

namespace game
{
	class MeshCollider : public Collider
	{
	protected:
		MeshCollider();
		bool Contains(Collider &collider) override final;
	protected:
		virtual bool SetPositionByIndex(size_t index, Vector2 &point) = NULL;
	};
}

#endif