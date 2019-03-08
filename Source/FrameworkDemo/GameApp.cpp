#include "GameRender.hpp"
#include "BoxCollider.hpp"
#include "vytTimer.hpp"
#include "winapi.hpp"

bool G_IsGameOver = false;

const string COLLIDER_TYPE_NONE = "CTN";
const string COLLIDER_TYPE_IRON = "CTI";
const string COLLIDER_TYPE_EARTHEN = "CTE";
const string COLLIDER_TYPE_BULLET = "CTB";
const string COLLIDER_TYPE_TANK = "CTT";

string LandImages[] = { "  ", "■", "▓" };
ConsoleColor LandColors[] = { { E_4BitColor::LWhite,	E_4BitColor::Black	}, { E_4BitColor::LWhite,	E_4BitColor::White	}, { E_4BitColor::LRed,	E_4BitColor::Yellow	} };
enum E_LandType
{
	// 空地
	Space,
	// 铁墙
	Iron,
	// 土墙
	Earthen,
};
class Map;

class Land : public game::Renderer, public game::BoxCollider
{
private:
	Map &m_map;
	Vector2 m_position;
	E_LandType m_type;
	void OnCollision(Collider &collider) override
	{

	}
public:
	const Vector2& getPosition() const { return m_position; }
	const std::string& getType() const
	{
		switch (m_type)
		{
		case Iron: return COLLIDER_TYPE_IRON;
		case Earthen: return COLLIDER_TYPE_EARTHEN;
		case Space:
		default: return COLLIDER_TYPE_NONE;
		}
	}
public:
	Land(Map &map, const Vector2 &position) : Renderer(1, 1, game::RenderType::StaticLayer0), game::BoxCollider(1, 1), m_map(map), m_position(position)
	{
		setColliderActive(true);
	}
	void SetType(E_LandType type)
	{
		m_type = type;
		CacheString(0, 0, LandImages[int(m_type)], LandColors[int(m_type)]);
	}
};

class Map
{
	vyt::vector<Land> m_lands;
public:
	Map()
	{
		//m_lands.AppendRange(GAME_WIDTH*GAME_HEIGHT, [&](size_t index) { return new Land(*this, { int(index%GAME_WIDTH),int(index / GAME_WIDTH) }); });
		Vector2 position;
		for (position.x = 0; position.x < GAME_WIDTH; ++position.x)
			for (position.y = 0; position.y < GAME_HEIGHT; ++position.y)
				m_lands.Append<Land>(*this, position);

		for (auto &land : m_lands)
		{
			if (land.getPosition().x == 0 || land.getPosition().x == GAME_WIDTH - 1 || land.getPosition().y == 0 || land.getPosition().y == GAME_HEIGHT - 1)
				land.SetType(E_LandType::Iron);
		}
	}
};

static const string TankParts[4][3][4] =
{
  {
	{"◢┃◣", "◢━◣", "◢┳◣", "◢┳◣"},
	{"┣●┫", "┣●┫", "━●┃", "┃●━"},
	{"◥━◤", "◥┃◤", "◥┻◤", "◥┻◤"}
  },
  {
	{"┏┃┓", "┏┳┓", "┏┳┓", "┏┳┓"},
	{"┣●┫", "┣●┫", "━●┫", "┣●━"},
	{"┗┻┛", "┗┃┛", "┗┻┛", "┗┻┛"}
  },
  {
	{"┏┃┓", "◢━◣", "┏┳◣", "◢┳┓"},
	{"┣●┫", "┣●┫", "━●┃", "┃●━"},
	{"◥━◤", "┗┃┛", "┗┻◤", "◥┻┛"}
  },
  {
	{"╔┃╗", "╔╦╗", "╔╦╗", "╔╦╗"},
	{"╠█╣", "╠█╣", "━█╣", "╠█━"},
	{"╚╩╝", "╚┃╝", "╚╩╝", "╚╩╝"}
  }
};

inline const string& GetStringByState(size_t y, Direction2D direction, size_t modelType = 0)
{
	auto directionIndex = 0;
	switch (direction)
	{
	case Direction2D::Left: directionIndex = 2; break;
	case Direction2D::Right: directionIndex = 3; break;
	case Direction2D::Up: directionIndex = 0; break;
	case Direction2D::Down: directionIndex = 1; break;
	case Direction2D::None:
	default: directionIndex = 0; break;
	}
	return TankParts[modelType][y][directionIndex];
}

class Bullet : public game::Renderer, public game::BoxCollider
{

};

class Tank : public game::Renderer, public game::BoxCollider
{
	bool m_isMoveable;
	Vector2 m_position;
	Direction2D m_direction;
	void OnCollision(Collider &collider) override
	{
		if (collider.getType() != COLLIDER_TYPE_NONE)
			m_isMoveable = false;
	}
public:
	const Vector2& getPosition() const { return m_position; }
	const std::string& getType() const { return COLLIDER_TYPE_TANK; }
public:
	Tank() : game::Renderer(3, 3, game::RenderType::DymanicLayer0), game::BoxCollider(3, 3), m_position(3,3)
	{
		setColliderActive(true);
		Draw();
	}
	void Draw()
	{
		for (size_t y = 0; y < 3; ++y)
			CacheString(0, y, GetStringByState(y, m_direction), { E_4BitColor::LWhite, E_4BitColor::Black });
	}
	void TryMove()
	{
		auto direction = Direction2D::None;
		if (IsKey('W')) direction = Direction2D::Up;
		if (IsKey('A')) direction = Direction2D::Left;
		if (IsKey('S')) direction = Direction2D::Down;
		if (IsKey('D')) direction = Direction2D::Right;
		if (direction == Direction2D::None) return;
		m_direction = direction;
		m_position += m_direction;
		m_isMoveable = true;
		TestStrikeToActiveCollider();
		if (!m_isMoveable)
			m_position -= m_direction;
		else
			StrikeToActiveColliders();
		Draw();
	}
};

int main()
{
	WindowManager::Get()->Initialize("窗口标题名", WIN_WIDTH, WIN_HEIGHT);
	Map map;
	Tank tank;
	while (!G_IsGameOver)
	{
		tank.TryMove();
		vyt::timer::get_instance().HandleClock();
		game::RenderLayer::getInstance().Draw();
	}

	return 0;
}