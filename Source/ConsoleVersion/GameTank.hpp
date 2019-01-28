#ifndef GAME_TANK_HPP_INCLUDED
#define GAME_TANK_HPP_INCLUDED

#include "GameRender.hpp"

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

class Tank : game::Renderer
{
	E_TankType m_type;
	Vector2 m_position;
	E_Direction m_direction;
public:
	const Vector2& getPosition() const { return m_position; }
	E_Direction getDirection() const { return m_direction; }
	void setDirection(E_Direction direction) { m_direction = direction; DrawTank(); }
public:
	Tank(E_TankType type);
	~Tank();

	void Reset(Vector2 position);
	void Clear();

	void Move(Vector2 target);
	bool Contains(Vector2 position);
private:
	// Because per RenderToLayer will reset all points, don't need clear tank.
	void DrawTank();
};

#endif