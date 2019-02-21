#include "GameBullet.hpp"
#include "TankController.hpp"
#include "GameTank.hpp"
#include "GameTerrian.hpp"
#include "GameMath.hpp"
#include "ColliderDefines.hpp"

static const std::string BulletImages[] = { "¨‘", "¡ò", "¡è" };

const std::string & Bullet::getType() const
{
	return m_player.isEnemy() ? COLLIDER_TYPE_ENEMY_BULLET : COLLIDER_TYPE_FRIEND_BULLET;
}

const Vector2 & Bullet::getPosition() const
{
	return m_position;
}

void Bullet::Process()
{
	m_position += m_direction;
	if (MoveAble()) return;
	delete this;
}

void Bullet::OnCollision(Collider & collider)
{
	if (collider.getType() == COLLIDER_TYPE_SPACE_OPENSPACE ||
		collider.getType() == COLLIDER_TYPE_GRASS_LANDSPACE ||
		collider.getType() == COLLIDER_TYPE_MAGMA_LANDSPACE ||
		collider.getType() == COLLIDER_TYPE_FROST_LANDSPACE) return;
	if ((collider.getType() == COLLIDER_TYPE_ENEMY_BULLET || collider.getType() == COLLIDER_TYPE_FRIEND_BULLET)
		&& ((Bullet*)&collider)->m_isJustCreate) return;
	else if (typeid(collider) == typeid(Tank))
		m_player.AttackTo(*((Tank*)(&collider)));
	else if (collider.getType() == COLLIDER_TYPE_EARTH_LANDSPACE)
	{
		//auto terrian = dynamic_cast<LandCollider*>(&collider);
		auto terrian = (LandCollider*)&collider;
		terrian->RemoveLand(m_position);
		m_direction = m_direction.Clockwise();
		terrian->RemoveLand(m_position + m_direction);
		m_direction = m_direction.Reverse();
		terrian->RemoveLand(m_position + m_direction);
	}
	m_isActive = false;
}

static std::set<Bullet*> activeBullets;

void Bullet::Create(E_BulletType type, Vector2 position, Direction2D direction, TankController &player)
{
	new Bullet(type, position, direction, player);
}

void Bullet::Clear()
{
	auto deleteBullets = activeBullets;
	for (auto &pBullet : deleteBullets)
		delete pBullet;
}

Bullet::Bullet(E_BulletType type, Vector2 position, Direction2D direction, TankController &player) :
	game::Renderer(1, 1, game::RenderType::DymanicLayer0), BoxCollider(1, 1),
	m_isActive(true), m_position(position), m_direction(direction), m_player(player)
{
	activeBullets.emplace(this);
	m_isJustCreate = true;
	set_Speed(SPEED_BULLET);
	if (!MoveAble())
		delete this;
	else
	{
		CacheString(0, 0, BulletImages[m_player.getAttack() - 3]);
		setColliderActive(true);
	}
	m_isJustCreate = false;
}

Bullet::~Bullet()
{
	activeBullets.erase(activeBullets.find(this));
}

bool Bullet::MoveAble()
{
	if (!m_isActive) return false;
	StrikeToActiveColliders();
	return m_isActive;
}
