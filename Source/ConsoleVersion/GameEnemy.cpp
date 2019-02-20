#include "GameEnemy.hpp"

TankState Enemy::IndirectDirection()
{
	auto ts = rand() % 6;
	switch (ts)
	{
	case 1: return TankState::Fire;
	case 2: return TankState::MoveLeft;
	case 3: return TankState::MoveUp;
	case 4: return TankState::MoveDown;
	case 5: return TankState::MoveRight;
	default: return TankState::Idle;
	}
}

TankState Enemy::DirectDirection()
{
	auto ts = rand() % 6;
	switch (ts)
	{
	case 1: return TankState::Fire;
	case 2: return TankState::MoveLeft;
	case 3: return TankState::MoveUp;
	case 4: return TankState::MoveDown;
	case 5: return TankState::MoveRight;
	default: return TankState::Idle;
	}
}

Enemy::Enemy() : TankController(true)
{
}

Enemy::~Enemy()
{

}
