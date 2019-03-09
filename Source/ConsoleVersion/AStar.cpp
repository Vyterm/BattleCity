#include "AStar.hpp"
#include "GameRender.hpp"
#include <algorithm>
#include <math.h>
#include <conio.h>
//#define LOG_ASTAR_PATH

std::deque<AStarPoint> AStar::SpreadPoint(AStarPoint Point)
{
	// 1. ����һ���������ڱ�����ɢ���Ľ��
	std::deque<AStarPoint> Points;

	// 2. ����һ�����鱣�������Χ�ĸ����ƫ��(ע������)
	std::deque<Direction2D> directions = { Direction2D::Left, Direction2D::Right, Direction2D::Up, Direction2D::Down };

	// 3. ʹ��ѭ��������Χ������
	for (auto &direction : directions)
	{
		// ��ȡ���꣬ƫ�ƺ͵�ǰλ�����
		AStarPoint TempPoint = { Point.position + direction, direction };
		// ��鴫��Ľ���Ƿ�������Խ�����ɢ
		if (CheckPoint(TempPoint))
			Points.push_back(TempPoint);
	}

	// 4. �����ҵ�������
	return Points;
}

bool AStar::CheckPoint(AStarPoint & Point)
{
	// find(��Ҫ���ҵ���ʼ�������� ��Ҫ���ҵĽ�������������Ҫ���ҵĶ���)
	// �����ҳɹ�������ָ�����ݵĵ�������û�ҵ����� end() ����

	// 1. ���� Close ��
	if (CloseTable.end() != find(CloseTable.begin(), CloseTable.end(), Point))
		return false;

	// 2. ���� Open ��
	if (OpenTable.end() != find(OpenTable.begin(), OpenTable.end(), Point))
		return false;

	// 3. �����ϰ������ɹ��ͷ��� true
	return m_virtualTank.IsMoveable(Point.position);
}

void AStar::CalcAndSave(AStarPoint & Parent, AStarPoint & Point)
{
	// 1. ���ø��ڵ㣬�������Ļ�ȡ·��
	Point.parent = Parent.position;

	// 2. ���� F G H
	Point.GValue = Parent.GValue + 1;
	auto offset = Point.position - EndPoint.position;
	Point.HValue = abs(offset.x) + abs(offset.y);
	Point.FValue = Point.GValue + Point.HValue;
}

void AStar::InitPointInfo(Vector2 StartPoint, Vector2 EndPoint)
{
	this->EndPoint.position = EndPoint;
	this->StartPoint.position = StartPoint;
}

bool AStar::FindShortestPath()
{
	OpenTable.clear();
	CloseTable.clear();
	PathTable.clear();
	// 0. �ж������յ��Ƿ���ͬ
	if (StartPoint.position == EndPoint.position)
		return false;

	// 1. �������뵽 Open ����
	OpenTable.push_back(StartPoint);

	// 2. ѭ���������еĴ����ҽ�㣬֪��û�н����Բ���
	while (!OpenTable.empty())
	{
		// 2.1 ��ȡ F ֵ��С�Ľ�㣬 ʹ�� min_element ��ȡ��С
		// ��㣬���� 1 �Ͳ��� 2 ����ʼλ�úͲ��ҵĽ���λ�ã���
		// �� 3 ʵ����һ���ȽϺ��������ڽ��бȽ�
		auto MinPoint = min_element(OpenTable.begin(), OpenTable.end(), CompareF);

		////////////////////////////////////////////////////////////
#ifdef LOG_ASTAR_PATH
		game::RenderLayer::getInstance().SetString(MinPoint->position, "��", game::ToRealColor(E_4BitColor::LWhite), game::ToRealColor(DEFAULT_BACK_COLOR));
		game::RenderLayer::getInstance().Draw();
#endif // LOG_ASTAR_PATH
		//WriteChar(MinPoint->X, MinPoint->Y, "��", COLOR_H_WHITE);
		////////////////////////////////////////////////////////////

		// 2.2 �� Open ����ɾ���ҵ��Ľ�㲢���뵽 Close ��
		auto TempPoint = *MinPoint;
		OpenTable.erase(MinPoint);
		CloseTable.push_back(TempPoint);

		// 2.3 ���ýڵ����ĸ����������ɢ�����سɹ���ɢ�Ľ��
		auto Points = SpreadPoint(TempPoint);

		// 2.4 ѭ����������ɢ�������н��
		for (size_t i = 0; i < Points.size(); ++i)
		{
			// 2.5 �ж��Ƿ����յ�
			if (Points[i].position == EndPoint.position)
				return true;

			// 2.6 �������н�����ֵ
			CalcAndSave(TempPoint, Points[i]);

			// 2.7 ����㱣�浽 Open ���еȴ��´�Ѱ�� ********
			// OpenTable.push_back(Points[i]);
			OpenTable.push_front(Points[i]);

			////////////////////////////////////////////////////////////
#ifdef LOG_ASTAR_PATH
			game::RenderLayer::getInstance().SetString(Points[i].position, "��", game::ToRealColor(E_4BitColor::LWhite), game::ToRealColor(DEFAULT_BACK_COLOR));
			game::RenderLayer::getInstance().Draw();
			//WriteChar(Points[i].X, Points[i].Y, "��", COLOR_H_WHITE);
			////////////////////////////////////////////////////////////
			while (_getch() != 'g')
				continue;
#endif // LOG_ASTAR_PATH
		}
	}

	// 3. ��ѭ������˵������ʧ��
	return false;
}

void AStar::GetShortestPath()
{
	// ���ϵ�Ѱ�Ҹ��ڵ㣬ֱ���ҵ����λ��
	size_t Index = CloseTable.size() - 1;
	PathTable.push_back(CloseTable[Index]);
#ifdef LOG_ASTAR_PATH
	game::RenderLayer::getInstance().SetString(CloseTable[Index].position, "��", game::ToRealColor(E_4BitColor::LGreen), game::ToRealColor(DEFAULT_BACK_COLOR));
	game::RenderLayer::getInstance().Draw();
#endif // LOG_ASTAR_PATH
	//WriteChar(CloseTable[Index].X, CloseTable[Index].Y, "��", COLOR_H_GREEN);

	// �����ҵ��ĸ��ڵ㲻���ҵ����ʱ������Ҫ��������
	while (!(StartPoint.position == CloseTable[Index].parent))
	{
		// ���ϲ��Ҹ��ڵ�
		for (size_t i = CloseTable.size() - 1; i >= 0; --i)
		{
			// ����ҵ��ĵ��ǵ�ǰ���ĸ��ڵ�
			if (CloseTable[i].position == CloseTable[Index].parent)
			{
				PathTable.push_back(CloseTable[i]);
				Index = i;
#ifdef LOG_ASTAR_PATH
				game::RenderLayer::getInstance().SetString(CloseTable[i].position, "��", game::ToRealColor(E_4BitColor::LGreen), game::ToRealColor(DEFAULT_BACK_COLOR));
				game::RenderLayer::getInstance().Draw();
				//WriteChar(CloseTable[i].X, CloseTable[i].Y, "��", COLOR_H_GREEN);
				while (_getch() != 'g')
					continue;
#endif // LOG_ASTAR_PATH
				break;
			}
		}
	}
}

inline Direction2D RandDirection()
{
	switch (rand() % 4)
	{
	case 0: return Direction2D::Left;
	case 1: return Direction2D::Up;
	case 2: return Direction2D::Down;
	case 3: return Direction2D::Right;
	default: return Direction2D::None;
	}
}

Direction2D AStar::PopNextDirection()
{
	if (PathTable.empty()) return RandDirection();
	auto direction = PathTable.back().direction;
	PathTable.pop_back();
	return direction;
}

void AStar::VirtualTank::OnCollision(Collider &collider)
{
	if (collider.getType() == COLLIDER_TYPE_JEBEL_LANDSPACE || 
		collider.getType() == COLLIDER_TYPE_FROST_LANDSPACE)
		m_moveable = false;
}
