#ifndef GAME_TANK_HPP_INCLUDED
#define GAME_TANK_HPP_INCLUDED

#include "GameRender.hpp"
#include "MeshCollider.hpp"

#include <map>

class Tank : game::Renderer, game::MeshCollider
{
	E_TankType m_type;
	E_4BitColor m_color;
	Vector2 m_position;
	Vector2 m_germPosition;
	Direction2D m_direction;
	bool m_moveable;
	bool m_isEnemy;
	const int m_maxHealth, m_maxLife;
	int m_attack, m_defense;
	int m_lifePoint, m_healthPoint;
public:
	int getAttack() const { return m_attack; }
	E_TankType getTankType() const { return m_type; }
	E_4BitColor getColor() const { return m_color; }
	void setColor(E_4BitColor color) { m_color = color; DrawTank(); }
	const Vector2& get_GermPosition() const { return m_germPosition; }
	void set_GermPosition(const Vector2 &germPosition) { m_germPosition = germPosition; }
	const Vector2& getPosition() const { return m_position; }
	Direction2D getDirection() const { return m_direction; }
	void setDirection(Direction2D direction) { m_direction = direction; DrawTank(); }
	bool isEnemy() const;
	bool isAlive() const;
public:
	Tank(TankModel model, bool isEnemy);
	~Tank();

	void Reset();
	void Clear();

	void ReduceHealth(int attack);

	void Move(Vector2 target);

	void OnCollision(Collider &collider);
	bool Contains(const Vector2 &position);
	bool SetPositionByIndex(size_t index, Vector2 & point);

	const std::string& getType() const;
private:
	// Because per RenderToLayer will reset all points, don't need clear tank.
	void DrawTank();
	void Regerm();
};

#endif