#ifndef GAME_BULLET_HPP_INCLUDED
#define GAME_BULLET_HPP_INCLUDED

#include "GameRender.hpp"
#include "BoxCollider.hpp"
#include "GameController.hpp"
#include <string>

enum class E_BulletType
{
	Normal,
};

class TankController;
class Bullet : game::Renderer, game::BoxCollider, game::Controller
{
public:
	static void Create(E_BulletType type, Vector2 position, Direction2D direction, TankController &pPlayer);
	static void Clear();
private:
	TankController &m_player;
	Vector2 m_position;
	Direction2D m_direction;
	bool m_isActive;
	bool m_isJustCreate;
	Bullet(E_BulletType type, Vector2 position, Direction2D direction, TankController &player);
	~Bullet();
	bool MoveAble();
public:
	const std::string& getType() const;
	virtual const Vector2& getPosition() const;
public:
	virtual void Process();
private:
	virtual void OnCollision(Collider &collider);
};

#endif