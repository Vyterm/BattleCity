#include "AStar.hpp"
#include "GameRender.hpp"
#include <algorithm>
#include <math.h>
#include <conio.h>
//#define LOG_ASTAR_PATH

std::deque<AStarPoint> AStar::SpreadPoint(AStarPoint Point)
{
	// 1. 定义一个容器用于保存扩散出的结点
	std::deque<AStarPoint> Points;

	// 2. 定义一个数组保存的是周围四个点的偏移(注意坐标)
	std::deque<Direction2D> directions = { Direction2D::Left, Direction2D::Right, Direction2D::Up, Direction2D::Down };

	// 3. 使用循环计算周围的坐标
	for (auto &direction : directions)
	{
		// 获取坐标，偏移和当前位置相加
		AStarPoint TempPoint = { Point.position + direction, direction };
		// 检查传入的结点是否合理，可以进行扩散
		if (CheckPoint(TempPoint))
			Points.push_back(TempPoint);
	}

	// 4. 返回找到的容器
	return Points;
}

bool AStar::CheckPoint(AStarPoint & Point)
{
	// find(需要查找的起始迭代器， 需要查找的结束迭代器，需要查找的对象)
	// 当查找成功，返回指向数据的迭代器，没找到返回 end() 迭代

	// 1. 不在 Close 中
	if (CloseTable.end() != find(CloseTable.begin(), CloseTable.end(), Point))
		return false;

	// 2. 不在 Open 中
	if (OpenTable.end() != find(OpenTable.begin(), OpenTable.end(), Point))
		return false;

	// 3. 不是障碍物，如果成功就返回 true
	return m_virtualTank.IsMoveable(Point.position);
}

void AStar::CalcAndSave(AStarPoint & Parent, AStarPoint & Point)
{
	// 1. 设置父节点，用于最后的获取路径
	Point.parent = Parent.position;

	// 2. 设置 F G H
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
	// 0. 判断起点和终点是否相同
	if (StartPoint.position == EndPoint.position)
		return false;

	// 1. 将起点放入到 Open 表中
	OpenTable.push_back(StartPoint);

	// 2. 循环查找所有的待查找结点，知道没有结点可以查找
	while (!OpenTable.empty())
	{
		// 2.1 获取 F 值最小的结点， 使用 min_element 获取最小
		// 结点，参数 1 和参数 2 是起始位置和查找的结束位置，参
		// 数 3 实际是一个比较函数，用于进行比较
		auto MinPoint = min_element(OpenTable.begin(), OpenTable.end(), CompareF);

		////////////////////////////////////////////////////////////
#ifdef LOG_ASTAR_PATH
		game::RenderLayer::getInstance().SetString(MinPoint->position, "■", game::ToRealColor(E_4BitColor::LWhite), game::ToRealColor(DEFAULT_BACK_COLOR));
		game::RenderLayer::getInstance().Draw();
#endif // LOG_ASTAR_PATH
		//WriteChar(MinPoint->X, MinPoint->Y, "■", COLOR_H_WHITE);
		////////////////////////////////////////////////////////////

		// 2.2 从 Open 表中删除找到的结点并放入到 Close 表
		auto TempPoint = *MinPoint;
		OpenTable.erase(MinPoint);
		CloseTable.push_back(TempPoint);

		// 2.3 将该节点向四个方向进行扩散并返回成功扩散的结点
		auto Points = SpreadPoint(TempPoint);

		// 2.4 循环遍历到扩散出的所有结点
		for (size_t i = 0; i < Points.size(); ++i)
		{
			// 2.5 判断是否是终点
			if (Points[i].position == EndPoint.position)
				return true;

			// 2.6 计算所有结点的数值
			CalcAndSave(TempPoint, Points[i]);

			// 2.7 将结点保存到 Open 表中等待下次寻找 ********
			// OpenTable.push_back(Points[i]);
			OpenTable.push_front(Points[i]);

			////////////////////////////////////////////////////////////
#ifdef LOG_ASTAR_PATH
			game::RenderLayer::getInstance().SetString(Points[i].position, "□", game::ToRealColor(E_4BitColor::LWhite), game::ToRealColor(DEFAULT_BACK_COLOR));
			game::RenderLayer::getInstance().Draw();
			//WriteChar(Points[i].X, Points[i].Y, "□", COLOR_H_WHITE);
			////////////////////////////////////////////////////////////
			while (_getch() != 'g')
				continue;
#endif // LOG_ASTAR_PATH
		}
	}

	// 3. 当循环结束说明查找失败
	return false;
}

void AStar::GetShortestPath()
{
	// 不断的寻找父节点，直到找到起点位置
	size_t Index = CloseTable.size() - 1;
	PathTable.push_back(CloseTable[Index]);
#ifdef LOG_ASTAR_PATH
	game::RenderLayer::getInstance().SetString(CloseTable[Index].position, "●", game::ToRealColor(E_4BitColor::LGreen), game::ToRealColor(DEFAULT_BACK_COLOR));
	game::RenderLayer::getInstance().Draw();
#endif // LOG_ASTAR_PATH
	//WriteChar(CloseTable[Index].X, CloseTable[Index].Y, "●", COLOR_H_GREEN);

	// 当查找到的父节点不是我的起点时，就需要继续查找
	while (!(StartPoint.position == CloseTable[Index].parent))
	{
		// 不断查找父节点
		for (size_t i = CloseTable.size() - 1; i >= 0; --i)
		{
			// 如果找到的点是当前结点的父节点
			if (CloseTable[i].position == CloseTable[Index].parent)
			{
				PathTable.push_back(CloseTable[i]);
				Index = i;
#ifdef LOG_ASTAR_PATH
				game::RenderLayer::getInstance().SetString(CloseTable[i].position, "●", game::ToRealColor(E_4BitColor::LGreen), game::ToRealColor(DEFAULT_BACK_COLOR));
				game::RenderLayer::getInstance().Draw();
				//WriteChar(CloseTable[i].X, CloseTable[i].Y, "●", COLOR_H_GREEN);
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
