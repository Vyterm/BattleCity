#ifndef GAME_CTRL_HPP
#define GAME_CTRL_HPP

#include "GameTank.hpp"
#include "GameController.hpp"

using std::string;

constexpr auto SPEED_DELTA = 30;
constexpr auto ACCELERATING_FACTOR = 0.998;

struct TankState
{
	enum E_TankState { Idle = 0, MoveUp = 1, MoveLeft = 2, MoveDown = 4, MoveRight = 8, Fire = 16 };
	E_TankState eState;
	TankState() : eState(Idle) { }
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
private:
	Tank m_tank;
	Vector2 m_germPosition;
public:
	virtual void set_Color(const E_4BitColor &color) { m_tank.setColor(color); }
	virtual E_4BitColor get_Color() const { return m_tank.getColor(); }
	const Vector2& get_GermPosition() const { return m_germPosition; }
	void set_GermPosition(const Vector2 &germPosition) { m_germPosition = germPosition; }
public:
	TankController(E_TankType type, E_4BitColor color, bool isEnemy);

	void OnDisable();
	void OnEnable();

	void Process();
protected:
	virtual TankState IndirectDirection() = NULL;
	virtual TankState DirectDirection() = NULL;
};

#endif