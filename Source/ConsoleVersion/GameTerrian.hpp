#ifndef GAME_TERRIAN_HPP_INCLUDED
#define GAME_TERRIAN_HPP_INCLUDED
#include "GameRender.hpp"
#include "GameCollider.hpp"
#include "GameModel.hpp"
#include "ColliderDefines.hpp"

#include <vector>
#include <map>

class TerrianCollider : game::Renderer, game::Collider
{
private:
	Vector2 m_position;
	E_StaticCellType m_type;
	std::string m_image;
	std::vector<Vector2> m_terrianPoints;
	std::map<Vector2, ConsoleColor> m_terrianLands;
public:
	const std::string& getType() const
	{
		switch (m_type)
		{
		case E_StaticCellType::JebelLand: return COLLIDER_TYPE_JEBEL_LANDSPACE;
		case E_StaticCellType::GrassLand: return COLLIDER_TYPE_GRASS_LANDSPACE;
		case E_StaticCellType::MagmaLand: return COLLIDER_TYPE_MAGMA_LANDSPACE;
		case E_StaticCellType::FrostLand: return COLLIDER_TYPE_FROST_LANDSPACE;
		case E_StaticCellType::EarthWall: return COLLIDER_TYPE_EARTH_LANDSPACE;
		default: throw std::out_of_range("TerrianCollider doesn't support such staticType");
		}
	}
	const Vector2& getPosition() const { return m_position; }
public:
	TerrianCollider(E_StaticCellType cellType)
		: m_type(cellType), m_image(StaticCellImages[int(cellType)]), game::Renderer(LAYER_WIDTH, LAYER_HEIGHT, game::RenderType::StaticLayer0), game::Collider(true)
	{
	}

	void OnCollision(Collider &collider)
	{
	}
	bool Contains(const Vector2 &position)
	{
		for (auto &land : m_terrianLands)
			if (land.first == position)
				return true;
		return false;
	}
	bool SetPositionByIndex(size_t index, Vector2 &point)
	{
		if (index >= m_terrianLands.size())
			return false;
		point = m_terrianPoints[index];
		return true;
	}
private:
	ConsoleColor GetColorByType(const Vector2 &position, const GameMapModel &model)
	{
		switch (m_type)
		{
		case E_StaticCellType::JebelLand: return DEFAULT_COLOR;
		case E_StaticCellType::GrassLand: return { DEFAULT_BACK_COLOR, E_4BitColor::LGreen };
		case E_StaticCellType::MagmaLand: return { DEFAULT_BACK_COLOR, E_4BitColor::LRed };
		case E_StaticCellType::FrostLand: return { DEFAULT_BACK_COLOR, E_4BitColor::LBlue };
		case E_StaticCellType::EarthWall: return { model.GetColor(position), DEFAULT_BACK_COLOR };
		default: throw std::out_of_range("TerrianCollider doesn't support such staticType");
		}
	}
	template <typename T>
	void VectorRemove(std::vector<T> &src, const T &dest)
	{
		for (auto iter = src.begin(); iter != src.end();)
			if (*iter == dest)
				iter = src.erase(iter);
			else
				++iter;
	}
	template <typename TKey, typename TValue>
	void MapRemoveByKey(std::map<TKey, TValue> &src, const TKey &dest)
	{
		for (auto iter = src.begin(); iter != src.end();)
			if (iter->first == dest)
				iter = src.erase(iter);
			else
				++iter;
	}
public:
	void RemoveLand(const Vector2 &position)
	{
		MapRemoveByKey(m_terrianLands, position);
		CacheString(position.x, position.y, StaticCellImages[int(E_StaticCellType::OpenSpace)], DEFAULT_COLOR);
	}
	void ReloadLand(const GameMapModel &model)
	{
		Vector2 position;
		for (position.x = 0; position.x < model.WIDTH; ++position.x)
		{
			for (position.y = 0; position.y < model.HEIGHT; ++position.y)
			{
				if (model.GetType(position) == m_type)
				{
					auto color = GetColorByType(position, model);
					if (m_terrianLands.find(position) == m_terrianLands.end())
					{
						m_terrianLands[position] = color;
						CacheString(position.x, position.y, m_image, color);
					}
					else
					{
						VectorRemove(m_terrianPoints, position);
						if (m_terrianLands[position] == color) continue;
					}
				}
			}
		}
		for (auto &invalidPoint : m_terrianPoints)
		{
			MapRemoveByKey(m_terrianLands, invalidPoint);
			CacheString(invalidPoint.x, invalidPoint.y, StaticCellImages[int(E_StaticCellType::OpenSpace)], DEFAULT_COLOR);
		}
		m_terrianPoints.erase(m_terrianPoints.begin(), m_terrianPoints.end());
		for (auto &land : m_terrianLands)
			m_terrianPoints.push_back(land.first);
	}
};

#endif