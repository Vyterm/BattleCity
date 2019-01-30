#ifndef GAME_TERRIAN_HPP_INCLUDED
#define GAME_TERRIAN_HPP_INCLUDED
#include "GameRender.hpp"
#include "GameCollider.hpp"

class TerrianCollider : game::Renderer, game::Collider
{
private:
	Vector2 m_position;
	std::string m_type;
	std::string m_image;
	std::vector<Vector2> m_terrianLands;
public:
	const std::string& getType() const { return m_type; }
	const Vector2& getPosition() const { return m_position; }
public:
	TerrianCollider(const std::string &type, const std::string &image)
		: m_type(type), m_image(image), game::Renderer(LAYER_WIDTH, LAYER_HEIGHT, game::RenderType::StaticLayer0), game::Collider(true) { }
	void OnCollision(Collider &collider)
	{

	}
	bool Contains(const Vector2 &position)
	{
		for (auto &land : m_terrianLands)
			if (land == position)
				return true;
		return false;
	}
	bool SetPositionByIndex(size_t index, Vector2 &point)
	{
		if (index >= m_terrianLands.size())
			return false;
		point = m_terrianLands[index];
		return true;
	}
	void AppendLand(Vector2 position, ConsoleColor color)
	{
		CacheString(position.x, position.y, m_image, color);
		m_terrianLands.push_back(position);
	}
	void RemoveLand(Vector2 position)
	{
		for (auto iter = m_terrianLands.begin(); iter != m_terrianLands.end();)
		{
			if (*iter == position)
			{
				CacheString(position.x, position.y, StaticCellImages[int(E_StaticCellType::OpenSpace)], DEFAULT_COLOR);
				iter = m_terrianLands.erase(iter);
			}
			else
				++iter;
		}
	}
	void ClearLands()
	{
		m_terrianLands.erase(m_terrianLands.begin(), m_terrianLands.end());
		ClearCache();
	}
};

#endif