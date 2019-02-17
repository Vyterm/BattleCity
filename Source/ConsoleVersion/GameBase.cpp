#include "GameBase.hpp"

constexpr size_t BASE_WIDTH = 3;
constexpr size_t BASE_HEIGHT = 3;

static const std::string BaseImage[BASE_HEIGHT] =
{
	"®é°Ô®ç",
	"®Ä®Ä®Ä",
	"®ç®Ä®é",
};

static const std::string BrokenBaseImage[BASE_HEIGHT] =
{
	"      ",
	"®ç  ®é",
	"®Ä®Ä®Ä",
};

Base::Base() : game::Renderer(BASE_WIDTH, BASE_HEIGHT, game::RenderType::StaticLayer0), game::Collider(true), m_position(GAME_WIDTH/2-1, 36)
{
	SetActive(true);
}

void Base::SetActive(bool isActive)
{
	m_isActive = isActive;
	for (int y = 0; y < BASE_HEIGHT; ++y)
		CacheString(0, y, m_isActive ? BaseImage[y] : BrokenBaseImage[y]);
}

bool Base::IsActive() const
{
	return m_isActive;
}

void Base::OnCollision(Collider & collider)
{
	if (collider.getType() == COLLIDER_TYPE_ENEMY_BULLET || collider.getType() == COLLIDER_TYPE_FRIEND_BULLET)
		SetActive(false);
}

bool Base::Contains(const Vector2 & position)
{
	return position.x - m_position.x < BASE_WIDTH && position.y - m_position.y < BASE_HEIGHT;
}

bool Base::SetPositionByIndex(size_t index, Vector2 & point)
{
	if (index >= 9) return false;
	point.x = m_position.x + index % 3;
	point.y = m_position.y + index / 3;
	return true;
}
