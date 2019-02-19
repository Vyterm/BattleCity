#ifndef GAME_BASE_HPP_INCLUDED
#define GAME_BASE_HPP_INCLUDED

#include "ColliderDefines.hpp"
#include "GameRender.hpp"
#include "BoxCollider.hpp"

class Base : game::Renderer, game::BoxCollider
{
private:
	Vector2 m_position;
	bool m_isActive;
public:
	const std::string& getType() const { return COLLIDER_TYPE_PLAYER_BASE; }
	const Vector2& getPosition() const { return m_position; }
public:
	Base();
	void SetActive(bool isActive);
	bool IsActive() const;

public:
	void OnCollision(Collider &collider);
};

#endif