#ifndef GAME_EDITOR_HPP
#define GAME_EDITOR_HPP

#include "GameDefines.hpp"

#include <map>
#include <vector>
#include <fstream>

#pragma region Cell Defines

enum class E_StaticCellType
{
	OpenSpace,
	JebelLand,
	GrassLand,
	MagmaLand,
	FrostLand,

	GermPoint,
	JumpPoint,
};

enum class E_FoodType
{
	/*������Ч��*/ NormalEffect,
	/*ʹ�Է��䳤*/ AppendLength,
	/*ʹ�Լ����*/ RemoveLength,
	/*ʹ�Լ�����*/ Acceleration,
	/*ʹ�Է�����*/ Deceleration,
	/*ʹ�Է���ת*/ Reverse,
	/*�Լ������*/ BuffStrong,
	/*�Է���ʧ��*/ BuffControl,
	/*�Լ�������*/ BuffGhost,
};

#pragma endregion

struct CellModel
{
	E_StaticCellType type;
	E_4BitColor foreColor;
	CellModel& operator=(E_StaticCellType type) { this->type = type; return *this; }
	CellModel& operator=(E_4BitColor foreColor) { this->foreColor = foreColor; return *this; }
	friend std::ostream& operator<<(std::ostream& os, CellModel& model)
	{
		os << int(model.type) << " ";
		os << int(model.foreColor) << " ";
		return os;
	}
	friend std::istream& operator>>(std::istream& is, CellModel& model)
	{
		int it, ifc;
		is >> it;
		is >> ifc;
		model.type = E_StaticCellType(it);
		model.foreColor = E_4BitColor(ifc);
		return is;
	}
};

struct JumpModel
{
	Vector2 src;
	Vector2 dest;
	ConsoleColor color;
	friend std::ostream& operator<<(std::ostream& os, JumpModel& model)
	{
		os << model.src << " ";
		os << model.dest << " ";
		os << model.color << " ";
		return os;
	}
	friend std::istream& operator>>(std::istream& is, JumpModel& model)
	{
		is >> model.src;
		is >> model.dest;
		is >> model.color;
		return is;
	}
};

struct PlayerModel
{
	Vector2 position;
	E_4BitColor foreColor;
	friend std::ostream& operator<<(std::ostream& os, PlayerModel& model)
	{
		os << model.position << " ";
		os << int(model.foreColor) << " ";
		return os;
	}
	friend std::istream& operator>>(std::istream& is, PlayerModel& model)
	{
		int foreColor;
		is >> model.position;
		is >> foreColor;
		model.foreColor = E_4BitColor(foreColor);
		return is;
	}
};

template <size_t Width, size_t Height>
class MapModelTemplate
{
private:
	CellModel m_cellModels[Width][Height];
	std::map<E_FoodType, size_t> m_foodWeights;
	std::vector<JumpModel> m_jumpPoints;
	std::vector<Vector2> m_germPoints;
	size_t m_foodCount = 1;
	//const CellModel& get_Index(size_t x, size_t y) const { return m_cellModels[x + y * m_width]; }
	//void set_Index(size_t x, size_t y, const CellModel &cell) { m_cellModels[x + y * m_width] = cell; }
	CellModel& Index(int x, int y) { return m_cellModels[x][y]; }
	const CellModel& Index(int x, int y) const { return m_cellModels[x][y]; }
	CellModel& Index(const Vector2 &position) { return m_cellModels[position.x][position.y]; }
	const CellModel& Index(const Vector2 &position) const { return m_cellModels[position.x][position.y]; }
public:
	static const size_t WIDTH = Width;
	static const size_t HEIGHT = Height;
	MapModelTemplate()
	{
		m_foodWeights[E_FoodType::NormalEffect] = 20;
		m_foodWeights[E_FoodType::AppendLength] = 10;
		m_foodWeights[E_FoodType::RemoveLength] = 10;
		m_foodWeights[E_FoodType::Acceleration] = 10;
		m_foodWeights[E_FoodType::Deceleration] = 10;
		m_foodWeights[E_FoodType::Reverse] = 10;
		m_foodWeights[E_FoodType::BuffStrong] = 10;
		m_foodWeights[E_FoodType::BuffControl] = 10;
		m_foodWeights[E_FoodType::BuffGhost] = 10;
	}
	~MapModelTemplate() { }

	void Clear()
	{
		for (int x = 0; x < Width; ++x)
			for (int y = 0; y < Height; ++y)
				m_cellModels[x][y] = E_StaticCellType::OpenSpace;
		m_jumpPoints.erase(m_jumpPoints.begin(), m_jumpPoints.end());
		m_germPoints.erase(m_germPoints.begin(), m_germPoints.end());
	}

	#pragma region Land Shape

	void SetHollowLand(Vector2 startPos, Vector2 endPos, E_StaticCellType staticType)
	{
		for (int y = startPos.y; y <= endPos.y; ++y)
			for (int x = startPos.x; x <= endPos.x; ++x)
				if (x == startPos.x || x == endPos.x || y == startPos.y || y == endPos.y)
					m_cellModels[x][y] = staticType;
				else
					m_cellModels[x][y] = E_StaticCellType::OpenSpace;
	}

	void SetCloseyLand(Vector2 startPos, Vector2 endPos, E_StaticCellType staticType)
	{
		for (int y = startPos.y; y <= endPos.y; ++y)
			for (int x = startPos.x; x <= endPos.x; ++x)
				m_cellModels[x][y] = staticType;
	}

	void SetCross(Vector2 position)
	{
		Index(position.x - 1, position.y) = Index(position.x, position.y) = Index(position.x + 1, position.y)
			= Index(position.x, position.y - 1) = Index(position.x, position.y + 1) = E_StaticCellType::JebelLand;
	}

	void SetType(Vector2 position, E_StaticCellType type, E_4BitColor foreColor = DEFAULT_FORE_COLOR)
	{
		if (type == E_StaticCellType::GermPoint)
			SetPlayer(position, foreColor);
		else
			Index(position.x, position.y) = { type, foreColor };
	}

	E_StaticCellType GetType(const Vector2 &position) const
	{
		return Index(position.x, position.y).type;
	}

	E_4BitColor GetColor(const Vector2 &position) const
	{
		return Index(position).foreColor;
	}

	#pragma endregion

	#pragma region Jump Vector2

	bool SetJumpPoint(Vector2 src, Vector2 dest, E_4BitColor foreColor)
	{
		for (auto &jpm : m_jumpPoints)
			if (jpm.src == src || jpm.src == dest || jpm.dest == src || jpm.dest == dest)
				return false;
		Index(src) = Index(dest) = { E_StaticCellType::JumpPoint, foreColor };
		m_jumpPoints.push_back({ src, dest, { foreColor, DEFAULT_BACK_COLOR } });
		return true;
	}

	void TryRemoveJumpPoint(Vector2 position)
	{
		for (auto iter = m_jumpPoints.begin(); iter != m_jumpPoints.end();)
			if (iter->src == position || iter->dest == position)
				iter = m_jumpPoints.erase(iter);
			else
				++iter;
	}

	#pragma endregion

	#pragma region Germ Vector2

	const std::vector<JumpModel>& GetJumpPoints() const
	{
		return m_jumpPoints;
	}

	void SetPlayer(Vector2 position, E_4BitColor foreColor)
	{
		Index(position) = { E_StaticCellType::GermPoint, foreColor };
		m_germPoints.push_back(position);
		if (PlayerCount() > MAX_PLAYER_COUNT)
		{
			auto invalidPlayer = m_germPoints[0];
			Index(invalidPlayer) = { E_StaticCellType::OpenSpace, DEFAULT_FORE_COLOR };
			m_germPoints.erase(m_germPoints.begin());
		}
	}
	const Vector2& GetPlayer(size_t index) const { return m_germPoints[index]; }
	size_t PlayerCount() const { return m_germPoints.size(); }

	#pragma endregion

	#pragma region Food Config

	size_t& FoodWeight(E_FoodType type) { return m_foodWeights[type]; }
	size_t get_FoodCount() const { return m_foodCount; }
	void set_FoodCount(size_t foodCount) { m_foodCount = foodCount; }

	#pragma endregion

	#pragma region Save & Load

	friend std::ostream& operator<<(std::ostream& os, MapModelTemplate &mapModel)
	{
		for (int x = 0; x < Width; ++x)
			for (int y = 0; y < Height; ++y)
				os << mapModel.m_cellModels[x][y];
		os << mapModel.m_foodWeights.size() << " ";
		for (auto iter = mapModel.m_foodWeights.begin(); iter != mapModel.m_foodWeights.end(); ++iter)
		{
			os << int(iter->first) << " ";
			os << iter->second << " ";
		}
		os << mapModel.m_jumpPoints.size() << " ";
		for (auto &jp : mapModel.m_jumpPoints) os << jp;
		os << mapModel.m_germPoints.size() << " ";
		for (auto &gp : mapModel.m_germPoints) os << gp;
		os << mapModel.m_foodCount << " ";
		return os;
	}

	friend std::istream& operator>>(std::istream& is, MapModelTemplate &mapModel)
	{
		for (int x = 0; x < Width; ++x)
			for (int y = 0; y < Height; ++y)
				is >> mapModel.m_cellModels[x][y];
		size_t size;
		is >> size;
		for (size_t i = 0; i < size; ++i)
		{
			int type;
			size_t weight;
			is >> type >> weight;
			mapModel.m_foodWeights[E_FoodType(type)] = weight;
		}
		is >> size;
		for (size_t i = 0; i < size; ++i)
		{
			JumpModel jm;
			is >> jm;
			mapModel.m_jumpPoints.push_back(jm);
		}
		is >> size;
		for (size_t i = 0; i < size; ++i)
		{
			Vector2 pm;
			is >> pm;
			mapModel.m_germPoints.push_back(pm);
		}
		is >> mapModel.m_foodCount;
		return is;
	}

	#pragma endregion

};

typedef MapModelTemplate<GAME_WIDTH + MAZE_WIDTH, GAME_HEIGHT> GameMapModel;

#endif