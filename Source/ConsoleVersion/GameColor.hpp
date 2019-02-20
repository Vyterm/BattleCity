#ifndef GAME_COLOR_HPP_INCLUDED
#define GAME_COLOR_HPP_INCLUDED

#include <iostream>

#pragma region Console 4bit Color

typedef unsigned long enumType;
/*
亮	红	绿	蓝
1	1	1	1
0x0~0xF表示暗黑色至亮白色
*/
enum class E_4BitColor : enumType
{
	Black = 0B0000,
	Blue = 0B0001,
	Green = 0B0010,
	Cyan = 0B0011,
	Red = 0B0100,
	Purple = 0B0101,
	Yellow = 0B0110,
	White = 0B0111,

	Gray = 0B1000,
	LBlue = 0B1001,
	LGreen = 0B1010,
	LCyan = 0B1011,
	LRed = 0B1100,
	LPurple = 0B1101,
	LYellow = 0B1110,
	LWhite = 0B1111,
};

inline E_4BitColor operator|(const E_4BitColor &lhs, const E_4BitColor &rhs)
{
	return static_cast<E_4BitColor>(static_cast<enumType>(lhs) | static_cast<enumType>(rhs));
}

inline E_4BitColor operator&(const E_4BitColor &lhs, const E_4BitColor &rhs)
{
	return static_cast<E_4BitColor>(static_cast<enumType>(lhs) & static_cast<enumType>(rhs));
}

inline E_4BitColor& operator|=(E_4BitColor &lhs, E_4BitColor &rhs)
{
	return lhs = lhs | rhs;
}

inline E_4BitColor& operator&=(E_4BitColor &lhs, E_4BitColor &rhs)
{
	return lhs = lhs & rhs;
}

constexpr E_4BitColor DEFAULT_FORE_COLOR = E_4BitColor::White;
constexpr E_4BitColor DEFAULT_BACK_COLOR = E_4BitColor::Black;

struct ConsoleColor
{
	ConsoleColor() : fore(DEFAULT_FORE_COLOR), back(DEFAULT_BACK_COLOR) { }
	ConsoleColor(E_4BitColor fore, E_4BitColor back) : fore(fore), back(back) { }
	E_4BitColor fore, back;
	bool operator==(const ConsoleColor &rhs) const { return fore == rhs.fore && back == rhs.back; }
	bool operator!=(const ConsoleColor &rhs) const { return fore != rhs.fore || back != rhs.back; }
	void Set(const ConsoleColor& color) { fore = color.fore; back = color.back; }
	friend std::ostream& operator<<(std::ostream& os, ConsoleColor& color)
	{
		os << int(color.fore) << " " << int(color.back) << " ";
		return os;
	}
	friend std::istream& operator>>(std::istream& is, ConsoleColor& color)
	{
		int fore, back;
		is >> fore >> back;
		color.fore = E_4BitColor(fore);
		color.back = E_4BitColor(back);
		return is;
	}
};
static const ConsoleColor DEFAULT_COLOR = { DEFAULT_FORE_COLOR, DEFAULT_BACK_COLOR };

#pragma endregion

#pragma region Real 32bit Color

class RenderColor
{
private:
	enumType m_red : 8;
	enumType m_green : 8;
	enumType m_blue : 8;
	enumType m_alpha : 8;
public:
	enumType get_red() const { return m_red; }
	void set_red(enumType red) { m_red = red; }
	enumType get_green() const { return m_green; }
	void set_green(enumType green) { m_green = green; }
	enumType get_blue() const { return m_blue; }
	void set_blue(enumType blue) { m_blue = blue; }
	enumType get_alpha() const { return m_alpha; }
	void set_alpha(enumType alpha) { m_alpha = alpha; }
public:
	RenderColor() : m_red(0), m_green(0), m_blue(0), m_alpha(0) {}
	RenderColor(enumType red, enumType green, enumType blue, enumType alpha) : m_red(red), m_green(green), m_blue(blue), m_alpha(alpha) { }
	bool operator==(const RenderColor &rhs) const
	{
		return
			m_red == rhs.m_red &&
			m_green == rhs.m_green &&
			m_blue == rhs.m_blue &&
			m_alpha == rhs.m_alpha;
	}
	bool operator!=(const RenderColor &rhs) const
	{
		return
			m_red != rhs.m_red ||
			m_green != rhs.m_green ||
			m_blue != rhs.m_blue ||
			m_alpha != rhs.m_alpha;
	}
	//friend std::ostream& operator<<(std::ostream& os, RenderColor& color)
	//{
	//	os << color.red << " " << color.green  << " " << color.blue << " " << color.alpha << " ";
	//	return os;
	//}
	//friend std::istream& operator>>(std::istream& is, RenderColor& color)
	//{
	//	is >> color.red >> color.green >> color.blue >> color.alpha;
	//	return is;
	//}
};

#pragma endregion

#endif