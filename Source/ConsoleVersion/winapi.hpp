#ifndef WINAPI_HPP_INCLUDED
#define WINAPI_HPP_INCLUDED

#include "GameModel.hpp"
#include <string>
#include <map>
#include <windows.h>

#include <Mmsystem.h>
#pragma comment(lib,"winmm.lib")

using std::string;

bool IsKey(int vKey);
bool IsKeyDown(int vKey);

void ResetCursor();

// Code from http://www.xuetang9.com
 // 设置控制台标题
void SetTitle(const char * title);
// 设置控制台颜色
void SetColor(ConsoleColor);
// 根据宽高设置控制台窗口的大小
void SetConsoleWindowSize();
// 设置光标的x、y坐标-正常坐标轴(x左至右，y上至下)
void SetPosition(int x, int y);

class FilterModel
{
private:
	LPTSTR m_filter;
	std::map<string, string> m_hintToSuffix;
	void CreateFilter();
	void ReleaseFilter();
public:
	FilterModel(string hint, string suffix);
	FilterModel(const std::map<string, string> &hintToSuffix);
	FilterModel(std::map<string, string> &&hintToSuffix);
	~FilterModel();
	LPCTSTR ToFilter() const;
};
extern const FilterModel FILTER_BATTLECITY_MAP;
// Code from https://blog.csdn.net/zuishikonghuan/article/details/47441163
string OpenFile(const FilterModel &filter = FILTER_BATTLECITY_MAP);
string SaveFile(const FilterModel &filter = FILTER_BATTLECITY_MAP);

#endif