#ifndef GAME_MAP_ITEM_HPP_INCLUDED
#define GAME_MAP_ITEM_HPP_INCLUDED

#include "GameModel.hpp"

enum class E_CellType
{
	None = 0,
	Land = 1,
	Food = 2,
	Jump = 5,
	Tank = 6,
};

enum class E_SubType
{
	// ��Ϊʳ������ͨʳ���Ϊ������ɽ��
	SubType0,
	// ��Ϊʳ��ʹ�Է��䳤����Ϊ�����ǲݵ�
	SubType1,
	// ��Ϊʳ��ʹ�Լ���̣���Ϊ������
	SubType2,
	// ��Ϊʳ��ʹ�Լ����٣���Ϊ�������ҽ�
	SubType3,
	// ��Ϊʳ��ʹ�Է����٣���Ϊ�����Ǳ���
	SubType4,
	// ��Ϊʳ��ʹ�Է���ת����Ϊ������
	SubType5,
	// ��Ϊʳ���Լ�����壬��Ϊ������
	SubType6,
	// ��Ϊʳ��Է���ʧ�أ���Ϊ������
	SubType7,
	// ��Ϊʳ���Լ������飬��Ϊ������
	SubType8,
};

constexpr E_4BitColor SubTypeColors[] = { E_4BitColor::White, E_4BitColor::LGreen, E_4BitColor::LPurple, E_4BitColor::LRed, E_4BitColor::LBlue, E_4BitColor::Black, E_4BitColor::LYellow, E_4BitColor::Green, E_4BitColor::Cyan };

class MapItem
{
public:
	E_CellType type = E_CellType::None;
	E_SubType subType = E_SubType::SubType0;
	ConsoleColor color = { DEFAULT_FORE_COLOR, DEFAULT_BACK_COLOR };
	Vector2 jumpPoint;
	MapItem() { }
	MapItem(E_CellType type) { this->type = type; }
	MapItem(E_CellType type, ConsoleColor color) { this->type = type; this->color = color; }
	MapItem(E_CellType type, E_SubType subType, ConsoleColor color) { Set(type, subType, color); }
	MapItem& operator=(const MapItem &rhs)
	{
		this->type = rhs.type;
		this->subType = rhs.subType;
		this->color = rhs.color;
		this->jumpPoint = rhs.jumpPoint;
		return *this;
	}

	void SetQuote(const std::shared_ptr<void> &quote)
	{
		m_weakQuote = quote;
	}
	bool TryGetQuote(std::shared_ptr<void> &quote)
	{
		quote = m_weakQuote.lock();
		return m_weakQuote.expired();
	}
	/*
	��	��	��	��
	1	1	1	1
	0x0~0xF��ʾ����ɫ������ɫ
	*/
	void Set(ConsoleColor color) { Set(type, subType, color); }
	void Set(E_CellType type, ConsoleColor color) { Set(type, E_SubType::SubType0, color); }
	void Set(E_CellType type, E_SubType subType, ConsoleColor color)
	{
		this->type = type;
		this->subType = subType;
		this->color = color;
	}
	MapItem& operator=(E_CellType type)
	{
		this->type = type;
		return *this;
	}
	MapItem& operator=(E_SubType subType)
	{
		this->subType = subType;
		return *this;
	}
	MapItem& operator=(ConsoleColor color)
	{
		this->color = color;
		return *this;
	}
	bool operator==(const MapItem &rhs) const { return type == rhs.type && subType == rhs.subType && color == rhs.color; }
	bool operator!=(const MapItem &rhs) const { return type != rhs.type || subType != rhs.subType || color != rhs.color; }
	bool operator==(const E_CellType &rhs) const { return rhs == type; }
	bool operator!=(const E_CellType &rhs) const { return rhs != type; }
	bool operator==(const E_SubType &rhs) const { return rhs == subType; }
	bool operator!=(const E_SubType &rhs) const { return rhs != subType; }
	bool operator==(const ConsoleColor &rhs) const { return rhs == color; }
	bool operator!=(const ConsoleColor &rhs) const { return rhs != color; }
	friend bool operator==(const E_CellType &lhs, const MapItem &rhs) { return lhs == rhs.type; }
	friend bool operator==(const E_SubType &lhs, const MapItem &rhs) { return lhs == rhs.subType; }
	friend bool operator==(const ConsoleColor &lhs, const MapItem &rhs) { return lhs == rhs.color; }
private:
	std::weak_ptr<void> m_weakQuote;
};

#endif