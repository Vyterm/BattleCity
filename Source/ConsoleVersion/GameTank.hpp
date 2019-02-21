#ifndef GAME_TANK_HPP_INCLUDED
#define GAME_TANK_HPP_INCLUDED

#include "GameModel.hpp"
#include "GameRender.hpp"
#include "MeshCollider.hpp"

#include <map>

class Tank : game::Renderer, game::MeshCollider
{
	Vector2 m_position;
	TankModel m_model;
	Direction2D m_direction;
	bool m_moveable;
	bool m_isEnemy;
	int m_lifePoint, m_healthPoint;
public:
	const Vector2& getPosition() const override { return m_position; }
	void setPosition(const Vector2 &position) { m_position = position; }
	const TankModel& getModel() const { return m_model; }
	int getLifeCount() const { return m_lifePoint; }
	void setLifeCount(int lifePoint) { m_lifePoint = lifePoint; }
	int getHealth() const { return m_healthPoint; }
	void setHealth(int healthPoint) { m_healthPoint = healthPoint; }
	Direction2D getDirection() const { return m_direction; }
	void setDirection(Direction2D direction) { m_direction = direction; DrawTank(); }
	bool isEnemy() const;
	bool isAlive() const;
public:
	Tank(bool isEnemy);
	~Tank();

	void ApplyModel(const TankModel &model);

	void Reset();
	void Clear();

	void ReduceHealth(int attack);

	// This method only tests whether the target point can move or not.
	bool Moveable();
	bool Moveable(Direction2D direction);

	void Move(Vector2 target);

	void OnCollision(Collider &collider) override;
	bool Contains(const Vector2 &position) override;
	bool SetPositionByIndex(size_t index, Vector2 & point) override;

	const std::string& getType() const override;
private:
	// Because per RenderToLayer will reset all points, don't need clear tank.
	void DrawTank();
	void Regerm();
};

#endif