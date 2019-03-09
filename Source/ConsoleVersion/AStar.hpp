#ifndef ASTAR_HPP_INCLUDED
#define ASTAR_HPP_INCLUDED

#include "GameMath.hpp"
#include "GameTerrian.hpp"
#include "BoxCollider.hpp"
#include "ColliderDefines.hpp"
#include <deque>

struct AStarPoint
{
	Vector2 position;
	Direction2D direction = Direction2D::None;
	int GValue = 0;
	int HValue = 0;
	int FValue = 0;
	Vector2 parent;
	AStarPoint() { }
	AStarPoint(Vector2 position, Direction2D direction) : position(position), direction(direction) { }
	// 赋值运算符，不能写引用
	void operator=(Vector2 position)
	{
		this->position = position;
	}

	// 重载 == 运算符
	bool operator==(Vector2 position)
	{
		return this->position == position;
	}

	// 重载 == 运算符
	bool operator==(AStarPoint asp)
	{
		return this->position == asp.position;
	}
};

class AStar
{
private:
	class VirtualTank : game::BoxCollider
	{
		Vector2 m_position;
		bool m_moveable;
	public:
		const Vector2& getPosition() const { return m_position; }
		const std::string& getType() const { return COLLIDER_TYPE_SPACE_OPENSPACE; }
	private:
		void OnCollision(Collider &collider);
	public:
		VirtualTank() : game::BoxCollider(3, 3) { }
	public:
		bool IsMoveable(const Vector2 &position)
		{
			m_position = position;
			m_moveable = true;
			TestStrikeToActiveCollider();
			return m_moveable;
		}
	};
	VirtualTank m_virtualTank;

	// 起点和终点
	AStarPoint EndPoint;
	AStarPoint StartPoint;

	// Open 表： [等待]查找的所有结点
	std::deque<AStarPoint> OpenTable;
	// Close 表： [已经]查找的所有结点
	std::deque<AStarPoint> CloseTable;
	// Path 表：存放在最终查找到的路径[方向]
	std::deque<AStarPoint> PathTable;
private:
	// 比较 F 值的大小，必须是静态的
	static bool CompareF(AStarPoint A, AStarPoint B)
	{
		return A.FValue < B.FValue;
	}

	// 通过传入的结点向四周扩散
	std::deque<AStarPoint> SpreadPoint(AStarPoint Point);

	// 判断结点是否可以扩散
	bool CheckPoint(AStarPoint & Point);

	// 计算必须的几个值
	void CalcAndSave(AStarPoint & Parent, AStarPoint & Point);
public:
	AStar() { }
	// 初始化坐标的信息
	void InitPointInfo(Vector2 StartPoint, Vector2 EndPoint);

	// 查找最短路径
	bool FindShortestPath();

	// 获取最短路径
	void GetShortestPath();

	// 获取下一步方向
	Direction2D PopNextDirection();
};

#endif