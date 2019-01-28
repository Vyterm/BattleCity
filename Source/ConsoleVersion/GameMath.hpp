#ifndef GAME_MATH_HPP_INCLUDED
#define GAME_MATH_HPP_INCLUDED

#include <iostream>

struct Vector2
{
public:
	int x, y;
	bool operator==(const Vector2 &rhs) const { return x == rhs.x && y == rhs.y; }
	bool operator!=(const Vector2 &rhs) const { return x != rhs.x || y != rhs.y; }
	bool operator<(const Vector2 &rhs) const { return x < rhs.x && y < rhs.y; }
	bool operator<=(const Vector2 &rhs) const { return x <= rhs.x && y <= rhs.y; }
	bool operator>(const Vector2 &rhs) const { return x > rhs.x && y > rhs.y; }
	bool operator>=(const Vector2 &rhs) const { return x >= rhs.x && y >= rhs.y; }
	void Set(const Vector2& point) { x = point.x; y = point.y; }
	friend std::ostream& operator<<(std::ostream& os, Vector2& point)
	{
		os << point.x << " " << point.y << " ";
		return os;
	}
	friend std::istream& operator>>(std::istream& is, Vector2& point)
	{
		is >> point.x >> point.y;
		return is;
	}
};

enum class E_Direction
{
	None = 0,
	Left = 1,
	Right = 2,
	Up = 3,
	Down = 4,
};

inline Vector2 GetPositionByDirection(Vector2 startPos, E_Direction direction)
{
	switch (direction)
	{
	case E_Direction::Left: startPos.x -= 1; break;
	case E_Direction::Right: startPos.x += 1; break;
	case E_Direction::Up: startPos.y -= 1; break;
	case E_Direction::Down: startPos.y += 1; break;
	case E_Direction::None:
	default: break;
	}
	return startPos;
}

inline E_Direction GetReverseDirection(E_Direction direction)
{
	switch (direction)
	{
	case E_Direction::Left: return E_Direction::Right;
	case E_Direction::Right: return E_Direction::Left;
	case E_Direction::Up: return E_Direction::Down;
	case E_Direction::Down: return E_Direction::Up;
	case E_Direction::None:
	default:
		return E_Direction::None;
	}
}

#endif