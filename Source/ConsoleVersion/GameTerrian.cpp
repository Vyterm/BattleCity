#include "GameTerrian.hpp"
#include "vytAlgorithm.hpp"

#pragma region Land Collider

const std::string & LandCollider::getType() const
{
	switch (m_type)
	{
	case E_StaticCellType::OpenSpace: return COLLIDER_TYPE_SPACE_OPENSPACE;
	case E_StaticCellType::JebelLand: return COLLIDER_TYPE_JEBEL_LANDSPACE;
	case E_StaticCellType::GrassLand: return COLLIDER_TYPE_GRASS_LANDSPACE;
	case E_StaticCellType::MagmaLand: return COLLIDER_TYPE_MAGMA_LANDSPACE;
	case E_StaticCellType::FrostLand: return COLLIDER_TYPE_FROST_LANDSPACE;
	case E_StaticCellType::EarthWall: return COLLIDER_TYPE_EARTH_LANDSPACE;
	default: throw std::out_of_range("TerrianCollider doesn't support such staticType");
	}
}

LandCollider::LandCollider(TerrianCollider & terrian, const Vector2 & position) :
	Renderer(1, 1, game::RenderType::StaticLayer0), BoxCollider(1, 1), m_terrian(terrian), m_position(position)
{
}

LandCollider::~LandCollider()
{
}

void LandCollider::Enable(E_StaticCellType type, const ConsoleColor & color)
{
	setColliderActive(true);
	SetDrawActive(true);
	m_isActive = true;
	type = int(type) <= int(E_StaticCellType::EarthWall) ? type : E_StaticCellType::OpenSpace;
	if (m_type == type) return;
	m_type = type;
	m_color = color;
	CacheString(0, 0, StaticCellImages[int(m_type)], m_color);
}

void LandCollider::Unable()
{
	setColliderActive(false);
	SetDrawActive(false);
	m_isActive = false;
}

void LandCollider::OnCollision(Collider & collider)
{
}

void LandCollider::RemoveLand(const Vector2 & position)
{
	m_terrian.RemoveLand(position);
}

#pragma endregion

#pragma region Terrian Collider

TerrianCollider::TerrianCollider()
{
	m_lands.AppendRange(GAME_WIDTH*GAME_HEIGHT, [&](size_t i) {return new LandCollider(*this, { int(i) % GAME_WIDTH,int(i) / GAME_WIDTH }); });
}

void TerrianCollider::RemoveLand(const Vector2 & position)
{
	for (auto &land : m_lands)
		if (land.getPosition() == position && land.getType() == COLLIDER_TYPE_EARTH_LANDSPACE)
			land.Enable(E_StaticCellType::OpenSpace, DEFAULT_COLOR);
}

void TerrianCollider::ReloadLand(const LevelModel & model)
{
	for (auto &land : m_lands)
		land.Enable(model.GetType(land.getPosition()), model.GetColor(land.getPosition()));
}

void TerrianCollider::ClearLands()
{
	for (auto &land : m_lands)
		land.Unable();
}

#pragma endregion

std::ostream & operator<<(std::ostream & os, const LandCollider & lc)
{
	os << enumType(lc.m_type) << " " << lc.m_color;
	return os;
}

std::istream & operator>>(std::istream & is, LandCollider & lc)
{
	enumType type;
	is >> type >> lc.m_color;
	lc.Enable(E_StaticCellType(type), lc.m_color);
	return is;
}

std::ostream & operator<<(std::ostream & os, const TerrianCollider & lc)
{
	for (size_t i = 0; i < lc.m_lands.size(); ++i)
		os << lc.m_lands[i];
	return os;
}

std::istream & operator>>(std::istream & is, TerrianCollider & lc)
{
	for (size_t i = 0; i < lc.m_lands.size(); ++i)
		is >> lc.m_lands[i];
	return is;
}
