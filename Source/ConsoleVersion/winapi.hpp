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
 // ���ÿ���̨����
void SetTitle(const char * title);
// ���ÿ���̨��ɫ
void SetColor(ConsoleColor);
// ���ݿ�����ÿ���̨���ڵĴ�С
void SetConsoleWindowSize();
// ���ù���x��y����-����������(x�����ң�y������)
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