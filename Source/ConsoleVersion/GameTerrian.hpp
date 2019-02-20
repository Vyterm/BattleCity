#ifndef GAME_TERRIAN_HPP_INCLUDED
#define GAME_TERRIAN_HPP_INCLUDED
#include "GameRender.hpp"
#include "BoxCollider.hpp"
#include "MeshCollider.hpp"
#include "GameModel.hpp"
#include "ColliderDefines.hpp"
#include "vytVector.hpp"

class TerrianCollider;
class LandCollider : game::Renderer, game::BoxCollider
{
private:
	TerrianCollider &m_terrian;
	E_StaticCellType m_type;
	Vector2 m_position;
	bool m_isActive;
public:
	const std::string& getType() const;
	const Vector2& getPosition() const { return m_position; }
	bool IsActive() { return m_isActive; }
public:
	LandCollider(TerrianCollider &terrian, const Vector2 &position);
	~LandCollider();
	void Enable(E_StaticCellType type, const ConsoleColor &color);
	void Unable();
	void OnCollision(Collider &collider);
	void RemoveLand(const Vector2 &position);
};

class TerrianCollider
{
private:
	vyt::vector<LandCollider> m_lands;
public:
	TerrianCollider();
public:
	void RemoveLand(const Vector2 &position);
	void ReloadLand(const LevelModel &model);
	void ClearLands();
};

#endif