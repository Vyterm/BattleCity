#ifndef GAME_BULLET_HPP_INCLUDED
#define GAME_BULLET_HPP_INCLUDED

#include "GameRender.hpp"
#include "GameCollider.hpp"
#include "GameMove.hpp"
#include <string>

enum class E_BulletType
{
	Normal,
};
static const std::string BulletImages[] = { "¨‘" };

class Bullet : game::Renderer, game::Collider, MoveCtrl
{
public:
	static void Create(E_BulletType type, int attack, Vector2 position, E_Direction direction);
private:
	int m_attack;
	bool m_isEnemy;
	Vector2 m_position;
	E_Direction m_direction;
	bool m_isActive;
	Bullet(E_BulletType type, int attack, Vector2 position, E_Direction direction);
public:
	const std::string& getType() const;
	virtual const Vector2& getPosition() const;
public:
	virtual void Process();
private:
	virtual void OnCollision(Collider &collider);
	virtual bool Contains(const Vector2 &position);
	virtual bool SetPositionByIndex(size_t index, Vector2 &point);
};

#endif