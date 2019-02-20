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
public:
	int getAttack() const { return m_tank.getAttack(); }
	bool isEnemy() const { return m_tank.isEnemy(); }
	virtual void set_Color(const E_4BitColor &color) { m_tank.setColor(color); }
	virtual E_4BitColor get_Color() const { return m_tank.getColor(); }
public:
	TankController(bool isEnemy);

	void OnDisable();
	void OnEnable();

	void Process();

	virtual bool AttackTo(Tank &tank);
	virtual void ApplyModel(TankModel model);
protected:
	virtual TankState IndirectDirection() = NULL;
	virtual TankState DirectDirection() = NULL;
};

#endif