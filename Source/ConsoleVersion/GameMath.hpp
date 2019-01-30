#ifndef GAME_MATH_HPP_INCLUDED
#define GAME_MATH_HPP_INCLUDED

#include <iostream>

struct Vector2
{
public:
	int x, y;
	Vector2() : x(0), y(0) { }
	Vector2(int x, int y) : x(x), y(y) { }
	bool operator==(const Vector2 &rhs) const { return x == rhs.x && y == rhs.y; }
	bool operator!=(const Vector2 &rhs) const { return x != rhs.x || y != rhs.y; }
	bool operator<(const Vector2 &rhs) const { return x < rhs.x || (x == rhs.x && y < rhs.y); }
	bool operator<=(const Vector2 &rhs) const { return x <= rhs.x && y <= rhs.y; }
	bool operator>(const Vector2 &rhs) const { return x > rhs.x || (x == rhs.x && y > rhs.y); }
	bool operator>=(const Vector2 &rhs) const { return x >= rhs.x && y >= rhs.y; }
	Vector2 operator+(const Vector2 &rhs) const { return { x + rhs.x, y + rhs.y }; }
	Vector2& operator+=(const Vector2 &rhs) { x += rhs.x; y += rhs.y; return *this; }
	Vector2 operator-(const Vector2 &rhs) const { return { x - rhs.x, y - rhs.y }; }
	Vector2& operator-=(const Vector2 &rhs) { x -= rhs.x; y -= rhs.y; return *this; }
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
static const Vector2 constVectors[] = { {0,0}, {-1,0}, {1,0}, {0,-1}, {0,1} };

struct Direction2D
{
	enum E_Direction {None, Left, Right, Up, Down};
	E_Direction eDirection;
	Direction2D() : eDirection(E_Direction::None) { }
	Direction2D(E_Direction direction) : eDirection(direction) { }
	operator const E_Direction&() { return eDirection; }
	operator const Vector2&() { return constVectors[eDirection]; }
	Direction2D Reverse() const
	{
		switch (eDirection)
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
	Direction2D Clockwise() const
	{
		switch (eDirection)
		{
		case E_Direction::Left: return E_Direction::Up;
		case E_Direction::Right: return E_Direction::Down;
		case E_Direction::Up: return E_Direction::Right;
		case E_Direction::Down: return E_Direction::Left;
		case E_Direction::None:
		default:
			return E_Direction::None;
		}
	}
};

#endif