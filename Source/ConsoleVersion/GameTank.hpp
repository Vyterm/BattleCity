#ifndef GAME_TANK_HPP_INCLUDED
#define GAME_TANK_HPP_INCLUDED

#include "GameRender.hpp"
#include "GameCollider.hpp"

enum class E_TankType
{
	// 轻型坦克
	Light,
	// 中型坦克
	Medium,
	// 突击坦克
	Assault,
	// 重型坦克
	Heavy,
};

class Tank : game::Renderer, game::Collider
{
	E_TankType m_type;
	E_4BitColor m_color;
	Vector2 m_position;
	Direction2D m_direction;
	bool m_moveable;
	bool m_isEnemy;
	int m_healthPoint, m_attack, m_defense;
public:
	const Vector2& getPosition() const { return m_position; }
	Direction2D getDirection() const { return m_direction; }
	void setDirection(Direction2D direction) { m_direction = direction; DrawTank(); }
public:
	Tank(E_TankType type, E_4BitColor color, bool isEnemy);
	~Tank();

	void Reset(Vector2 position);
	void Clear();

	void ReduceHealth(int attack) {}

	void Move(Vector2 target);

	void OnCollision(Collider &collider);
	bool Contains(const Vector2 &position);
	bool SetPositionByIndex(size_t index, Vector2 & point);

	const std::string& getType() const;
private:
	// Because per RenderToLayer will reset all points, don't need clear tank.
	void DrawTank();
};

#endif