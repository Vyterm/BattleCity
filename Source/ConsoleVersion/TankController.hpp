#ifndef GAME_CTRL_HPP
#define GAME_CTRL_HPP

#include "GameTank.hpp"
#include "GameController.hpp"

using std::string;

struct TankState
{
	enum E_TankState { Idle = 0, MoveUp = 1, MoveLeft = 2, MoveDown = 4, MoveRight = 8, Fire = 16 };
	E_TankState eState;
	TankState() : eState(Idle) { }
	TankState(E_TankState state) : eState(state) { }
	TankState(Direction2D direction, bool isFire) : eState(isFire ? Fire : Idle)
	{
		switch (direction.eDirection)
		{
		case Direction2D::Left: eState = E_TankState(eState | MoveLeft); break;
		case Direction2D::Right: eState = E_TankState(eState | MoveRight); break;
		case Direction2D::Up: eState = E_TankState(eState | MoveUp); break;
		case Direction2D::Down: eState = E_TankState(eState | MoveDown); break;
		}
	}
	operator Direction2D()
	{
		if ((eState & E_TankState::MoveUp) == E_TankState::MoveUp) return Direction2D::Up;
		if ((eState & E_TankState::MoveLeft) == E_TankState::MoveLeft) return Direction2D::Left;
		if ((eState & E_TankState::MoveDown) == E_TankState::MoveDown) return Direction2D::Down;
		if ((eState & E_TankState::MoveRight) == E_TankState::MoveRight) return Direction2D::Right;
		return Direction2D::None;
	}
	bool isFire() const { return (eState & E_TankState::Fire) == E_TankState::Fire; }
};

class TankController : public game::Controller
{
protected:
	Tank m_tank;
	clock_t m_fireTimer, m_fireDelta;
public:
	const TankModel& getModel() const { return m_tank.getModel(); }
	const Vector2& getPosition() const { return m_tank.getPosition(); }
	void setPosition(const Vector2 &position) { m_tank.setPosition(position); }
	int getAttack() const { return getModel().attack; }
	int getLifeCount() const { return m_tank.getLifeCount(); }
	void setLifeCount(int lifePoint) { m_tank.setLifeCount(lifePoint); }
	int getHealth() const { return m_tank.getHealth(); }
	void setHealth(int healthPoint) { m_tank.setHealth(healthPoint); }
	Direction2D getDirection() const { return m_tank.getDirection(); }
	void setDirection(Direction2D direction) { m_tank.setDirection(direction); }
	bool isEnemy() const { return m_tank.isEnemy(); }
public:
	TankController(bool isEnemy);

	void OnDisable();
	void OnEnable();

	void Process();

	virtual bool AttackTo(Tank &tank);
	virtual void ApplyModel(const TankModel &model);
protected:
	virtual TankState IndirectDirection() = NULL;
	virtual TankState DirectDirection() = NULL;
};

#endif