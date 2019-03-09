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
	// ��ֵ�����������д����
	void operator=(Vector2 position)
	{
		this->position = position;
	}

	// ���� == �����
	bool operator==(Vector2 position)
	{
		return this->position == position;
	}

	// ���� == �����
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

	// �����յ�
	AStarPoint EndPoint;
	AStarPoint StartPoint;

	// Open �� [�ȴ�]���ҵ����н��
	std::deque<AStarPoint> OpenTable;
	// Close �� [�Ѿ�]���ҵ����н��
	std::deque<AStarPoint> CloseTable;
	// Path ����������ղ��ҵ���·��[����]
	std::deque<AStarPoint> PathTable;
private:
	// �Ƚ� F ֵ�Ĵ�С�������Ǿ�̬��
	static bool CompareF(AStarPoint A, AStarPoint B)
	{
		return A.FValue < B.FValue;
	}

	// ͨ������Ľ����������ɢ
	std::deque<AStarPoint> SpreadPoint(AStarPoint Point);

	// �жϽ���Ƿ������ɢ
	bool CheckPoint(AStarPoint & Point);

	// �������ļ���ֵ
	void CalcAndSave(AStarPoint & Parent, AStarPoint & Point);
public:
	AStar() { }
	// ��ʼ���������Ϣ
	void InitPointInfo(Vector2 StartPoint, Vector2 EndPoint);

	// �������·��
	bool FindShortestPath();

	// ��ȡ���·��
	void GetShortestPath();

	// ��ȡ��һ������
	Direction2D PopNextDirection();
};

#endif